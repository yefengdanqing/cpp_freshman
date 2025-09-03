# -*- coding: utf-8 -*-
"""
Train a Deep CTR/CVR model from Parquet using schema.yaml.

Features:
- Read Parquet (local / OSS via fsspec)
- schema.yaml-driven dense/sparse parsing
- Sparse features -> hashing + Embedding
- Dense features -> StandardScaler
- Multi-task heads: click_has_click & conv_label (可切换只训练其中一个)
- Class imbalance: pos_weight 自动估计
- Metrics: ROC-AUC, PR-AUC
- Save: model.pt, scaler.pkl, feature_info.json, training_metrics.json, a copy of schema.yaml
"""

import os
import io
import math
import json
import glob
import yaml
import argparse
import pickle
from typing import List, Dict, Any, Optional, Tuple

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import IterableDataset, DataLoader

import pandas as pd
import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import roc_auc_score, average_precision_score

import pyarrow as pa
import pyarrow.parquet as pq

import ossfs
# ossfs.set_endpoint("oss-ap-southeast-1-internal.aliyuncs.com")  # 替换为你的 OSS endpoint

# 可选：直连 OSS 需要 ossfs；若没有可将 OSS 路径先同步到本地或 HDFS
try:
    import fsspec  # noqa
except Exception:
    fsspec = None


# ---------------------------
# Utils
# ---------------------------

def list_parquet_files(data_path: str, oss_key=None, oss_secret=None) -> List[str]:
    """
    支持:
      - 本地/挂载路径: /path/**/*.parquet
      - OSS/S3 等: oss://bucket/prefix/*.parquet (需 fsspec+ossfs)
    """
    if data_path.startswith("oss://") or data_path.startswith("s3://"):
        if fsspec is None:
            raise RuntimeError("fsspec/ossfs 未安装，无法直接读取 OSS/S3。请先 pip install fsspec ossfs，或先将数据同步到本地/HDFS。")
        # 使用 fsspec.filesystem 获取文件系统对象
        scheme = data_path.split("://", 1)[0]
        fs = fsspec.filesystem(scheme, key=oss_key, secret=oss_secret)
        # 使用 glob 匹配所有 parquet 文件
        # 获取文件列表
        files = fs.glob(data_path)
        # 加上协议前缀
        return [f"{scheme}:/{f}" for f in files]
    else:
        return glob.glob(data_path, recursive=True)

def open_parquet_file(path: str, oss_key=None, oss_secret=None):
    if path.startswith("oss://") or path.startswith("s3://"):
        scheme = path.split("://", 1)[0]
        fs = fsspec.filesystem(scheme, key=oss_key, secret=oss_secret)
        f = fs.open(path, mode='rb')
        return pq.ParquetFile(f)
    else:
        return pq.ParquetFile(path)


def load_yaml(path: str) -> Dict[str, Any]:
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def ensure_dir(path: str):
    if not path:
        return
    if path.startswith("oss://") or path.startswith("s3://"):
        # 通过 fsspec 创建目录
        if fsspec is None:
            print(f"[warn] 目标路径 {path} 是对象存储，但未安装 fsspec/ossfs；将尝试本地保存。")
            return
        fs = fsspec.filesystem(path.split("://", 1)[0])
        # 对象存储通常是 key，不需要 mkdir，但我们保持一致
        # 这里不强行创建，下面保存文件时会使用 fsspec.open
        return
    os.makedirs(path, exist_ok=True)

def fs_upload_file(local_path: str, remote_path: str):
    """
    使用fsspec的put方法上传文件，这是最高效的方式。
    """
    if remote_path.startswith("oss://") or remote_path.startswith("s3://"):
        if fsspec is None:
            raise RuntimeError("未安装 fsspec/ossfs，无法写入对象存储。请 pip install fsspec ossfs，或改为本地路径。")
        scheme = remote_path.split("://", 1)[0]
        fs = fsspec.filesystem(
            scheme,
            key=os.environ["OSS_ACCESS_KEY_ID"],
            secret=os.environ["OSS_ACCESS_KEY_SECRET"],
            client_kwargs={"endpoint": "oss-ap-southeast-1.aliyuncs.com"}
        )
        # put方法会自动进行分块上传，是上传整个文件的最佳方式
        fs.put(local_path, remote_path)
    else:
        # 对于本地路径，简单的复制文件
        import shutil
        shutil.copy2(local_path, remote_path)

def fs_write_bytes(path: str, data: bytes):
    if path.startswith("oss://") or path.startswith("s3://"):
        if fsspec is None:
            raise RuntimeError("未安装 fsspec/ossfs，无法写入对象存储。请 pip install fsspec ossfs，或改为本地路径。")
        scheme = path.split("://", 1)[0]
        fs = fsspec.filesystem(
            scheme,
            key=os.environ["OSS_ACCESS_KEY_ID"],
            secret=os.environ["OSS_ACCESS_KEY_SECRET"],
            client_kwargs={"endpoint": "oss-ap-southeast-1.aliyuncs.com"}
        )
        with fs.open(path, "wb") as f:
            f.write(data)
    else:
        with open(path, "wb") as f:
            f.write(data)


def fs_write_text(path: str, text: str):
    fs_write_bytes(path, text.encode("utf-8"))


def fs_write_json(path: str, obj: Any, indent: int = 2):
    fs_write_text(path, json.dumps(obj, indent=indent, ensure_ascii=False))


# ---------------------------
# Dataset
# ---------------------------

class ParquetIterableDataset(IterableDataset):
    """
    基于 Parquet 文件的 IterableDataset：
    - 按文件 -> 按 RowGroup 流式读取
    - 每次转换成 pandas.DataFrame -> 再到 batch tensors
    - 仅在 CPU 侧做轻预处理（hash、数值填充）
    """

    def __init__(
        self,
        files: List[str],
        schema: Dict[str, Any],
        dense_scaler: Optional[StandardScaler],
        hash_bucket_sizes: Dict[str, int],
        primary_label: str,
        secondary_label: Optional[str],
        batch_size: int = 2048,
        drop_remainder: bool = False,
        shuffle_rowgroups: bool = True,
        seed: int = 2025,
    ):
        super().__init__()
        self.files = files
        self.schema = schema
        self.dense_scaler = dense_scaler
        self.hash_bucket_sizes = hash_bucket_sizes
        self.primary_label = primary_label
        self.secondary_label = secondary_label
        self.batch_size = batch_size
        self.drop_remainder = drop_remainder
        self.shuffle_rowgroups = shuffle_rowgroups
        self.rng = np.random.default_rng(seed)

        self.dense_cols = [f["name"] for f in schema.get("dense_features", [])]
        self.sparse_cols = [f["name"] for f in schema.get("sparse_features", [])]

    def _hash_series(self, s: pd.Series, buckets: int) -> np.ndarray:
        # 简单一致哈希：缺失->"__NA__"
        vals = s.fillna("__NA__").astype(str).values
        # 使用 numpy 的快速哈希
        # （也可选择 mmh3 等库，这里保持纯依赖）
        hashed = np.frombuffer(pd.util.hash_pandas_object(pd.Series(vals), index=False).values.tobytes(),
                               dtype=np.uint64)
        return (hashed % buckets).astype(np.int64)

    def _extract_dense(self, df: pd.DataFrame) -> np.ndarray:
        if len(self.dense_cols) == 0:
            return np.zeros((len(df), 0), dtype=np.float32)
        X = df[self.dense_cols].astype("float32").replace([np.inf, -np.inf], np.nan).fillna(0.0).values
        if self.dense_scaler is not None:
            X = self.dense_scaler.transform(X)
        return X.astype(np.float32)

    def _extract_sparse(self, df: pd.DataFrame) -> Dict[str, np.ndarray]:
        out = {}
        for col in self.sparse_cols:
            buckets = self.hash_bucket_sizes[col]
            out[col] = self._hash_series(df[col], buckets)
        return out

    def _extract_labels(self, df: pd.DataFrame) -> Tuple[np.ndarray, Optional[np.ndarray]]:
        y1 = df[self.primary_label].astype("float32").values
        y2 = None
        if self.secondary_label and self.secondary_label in df.columns:
            y2 = df[self.secondary_label].astype("float32").values
        return y1, y2

    def __iter__(self):
        # 迭代所有文件的所有 rowgroups
        file_order = list(range(len(self.files)))
        if self.shuffle_rowgroups:
            self.rng.shuffle(file_order)

        for idx in file_order:
            pf = open_parquet_file(self.files[idx], oss_key=os.environ["OSS_ACCESS_KEY_ID"], oss_secret=os.environ["OSS_ACCESS_KEY_SECRET"])
            num_row_groups = pf.num_row_groups
            groups = list(range(num_row_groups))
            if self.shuffle_rowgroups:
                self.rng.shuffle(groups)

            for g in groups:
                table = pf.read_row_group(g)
                df = table.to_pandas()
                n = len(df)
                if n == 0:
                    continue

                # 构造 batch
                for start in range(0, n, self.batch_size):
                    end = start + self.batch_size
                    if end > n and self.drop_remainder:
                        break
                    batch_df = df.iloc[start:end]

                    X_dense = self._extract_dense(batch_df)
                    X_sparse = self._extract_sparse(batch_df)
                    y1, y2 = self._extract_labels(batch_df)

                    # 转 tensor
                    dense_t = torch.from_numpy(X_dense)  # [B, D]
                    sparse_t = {k: torch.from_numpy(v) for k, v in X_sparse.items()}
                    y1_t = torch.from_numpy(y1).view(-1, 1)
                    y_batch = {"primary": y1_t}
                    if y2 is not None:
                        y_batch["secondary"] = torch.from_numpy(y2).view(-1, 1)

                    yield dense_t, sparse_t, y_batch


# ---------------------------
# Model
# ---------------------------

class DeepFFM(nn.Module):
    """
    简化版：所有 sparse features 各自 Embedding，拼接后 + dense 输入 -> MLP
    Multi-task: 主任务(primary)，可选副任务(secondary)
    """

    def __init__(
        self,
        sparse_config: List[Dict[str, Any]],
        hash_bucket_sizes: Dict[str, int],
        dense_dim: int,
        hidden_dims: List[int] = [256, 128, 64],
        dropout: float = 0.1,
        activation: str = "relu",
        task_secondary: bool = True,
    ):
        super().__init__()

        act = nn.ReLU if activation.lower() == "relu" else nn.SiLU

        # Embedding tables for sparse features
        self.sparse_cols = [f["name"] for f in sparse_config]
        self.emb_layers = nn.ModuleDict()
        embed_total_dim = 0
        for f in sparse_config:
            name = f["name"]
            dim = int(f.get("embedding_dim", 8))
            buckets = int(hash_bucket_sizes[name])
            self.emb_layers[name] = nn.Embedding(num_embeddings=buckets, embedding_dim=dim)
            nn.init.normal_(self.emb_layers[name].weight, mean=0.0, std=0.01)
            embed_total_dim += dim

        input_dim = dense_dim + embed_total_dim

        layers = []
        prev = input_dim
        for h in hidden_dims:
            layers += [nn.Linear(prev, h), act(), nn.Dropout(dropout)]
            prev = h
        self.backbone = nn.Sequential(*layers)

        # heads
        self.head_primary = nn.Sequential(nn.Linear(prev, 1), nn.Sigmoid())
        self.task_secondary = task_secondary
        if self.task_secondary:
            self.head_secondary = nn.Sequential(nn.Linear(prev, 1), nn.Sigmoid())

    # def forward(self, dense_x: torch.Tensor, sparse_x: Dict[str, torch.Tensor]) -> Dict[str, torch.Tensor]:
    #     emb_list = []
    #     for name in self.sparse_cols:
    #         idx = sparse_x[name].long()
    #         emb = self.emb_layers[name](idx)  # [B, E]
    #         emb_list.append(emb)
    #     if len(emb_list) > 0:
    #         cat_sparse = torch.cat(emb_list, dim=1)
    #         x = torch.cat([dense_x, cat_sparse], dim=1)
    #     else:
    #         x = dense_x

    #     h = self.backbone(x) if len(self.backbone) > 0 else x
    #     out = {"primary": self.head_primary(h)}
    #     if self.task_secondary:
    #         out["secondary"] = self.head_secondary(h)
    #     return out
    
    def forward(
        self,
        dense_x: torch.Tensor,
        sparse_x: Dict[str, torch.Tensor]
    ) -> Dict[str, torch.Tensor]:
        emb_list: List[torch.Tensor] = []
        # ⚡ JIT 友好写法：遍历 items()
        for key, layer in self.emb_layers.items():
            idx = sparse_x[key].long()
            emb = layer(idx)
            emb_list.append(emb)

        if len(emb_list) > 0:
            cat_sparse = torch.cat(emb_list, dim=1)
            x = torch.cat([dense_x, cat_sparse], dim=1)
        else:
            x = dense_x

        h = self.backbone(x) if len(self.backbone) > 0 else x
        out: Dict[str, torch.Tensor] = {"primary": self.head_primary(h)}
        if self.task_secondary:
            out["secondary"] = self.head_secondary(h)
        return out



# ---------------------------
# Training
# ---------------------------

def estimate_pos_weight(files: List[str], primary_label: str, sample_groups: int = 64) -> float:
    """
    粗估正样本比例（用于 BCEWithLogitsLoss 的 pos_weight），
    这里我们数据是 Sigmoid 输出 + BCELoss；也可改用 BCEWithLogitsLoss。
    简单读取部分 rowgroups 估计。
    """
    total, pos = 0, 0
    if len(files) == 0:
        return 1.0

    picks = min(sample_groups, len(files))
    chosen = np.random.choice(files, size=picks, replace=False)

    for path in chosen:
        pf = open_parquet_file(path, oss_key=os.environ["OSS_ACCESS_KEY_ID"], oss_secret=os.environ["OSS_ACCESS_KEY_SECRET"])
        steps = min(pf.num_row_groups, 8)
        for g in range(steps):
            tbl = pf.read_row_group(g)
            col = tbl.column(primary_label).to_pandas().astype(float)
            total += len(col)
            pos += col.sum()

    if total == 0:
        return 1.0
    p = pos / total
    # 避免极端
    p = max(min(p, 0.99), 1e-6)
    # pos_weight ~ neg/pos
    return float((1 - p) / p)


def fit_dense_scaler(files: List[str], dense_cols: List[str], sample_groups: int = 128, max_rows: int = 2_000_000) -> StandardScaler:
    scaler = StandardScaler(with_mean=True, with_std=True)
    if not dense_cols:
        return scaler

    rows_used = 0
    for path in files[:sample_groups]:
        pf = open_parquet_file(path, oss_key=os.environ["OSS_ACCESS_KEY_ID"], oss_secret=os.environ["OSS_ACCESS_KEY_SECRET"])
        for g in range(pf.num_row_groups):
            tbl = pf.read_row_group(g)
            df = tbl.select(dense_cols).to_pandas()
            x = df.replace([np.inf, -np.inf], np.nan).fillna(0.0).astype("float32").values
            scaler.partial_fit(x)
            rows_used += len(df)
            if rows_used >= max_rows:
                break
        if rows_used >= max_rows:
            break
    return scaler


def collate_to_device(batch, device):
    dense, sparse, y = batch
    dense = dense.to(device)
    sparse = {k: v.to(device) for k, v in sparse.items()}
    y = {k: v.to(device) for k, v in y.items()}
    return dense, sparse, y


def train_one_epoch(model, loader, optimizer, device, loss_weight_primary=1.0, loss_weight_secondary=1.0,
                    pos_weight_primary=1.0, pos_weight_secondary=1.0):
    model.train()
    bce = nn.BCELoss(reduction="mean")
    total_loss = 0.0
    n_batches = 0

    for dense, sparse, y in loader:
        dense, sparse, y = collate_to_device((dense, sparse, y), device)
        out = model(dense, sparse)

        # 主任务
        loss_p = bce(out["primary"], y["primary"])
        # pos_weight 近似：用 F.binary_cross_entropy + sample weight 实现
        # 这里简单采用按照标签加权的方式（更细可改用 logits + BCEWithLogitsLoss 的 pos_weight）
        weight_p = torch.where(y["primary"] > 0.5, torch.tensor(pos_weight_primary, device=device), torch.tensor(1.0, device=device))
        loss_p = (weight_p * (-(y["primary"] * torch.log(out["primary"] + 1e-8) +
                                (1 - y["primary"]) * torch.log(1 - out["primary"] + 1e-8)))).mean()

        loss = loss_weight_primary * loss_p

        # 副任务
        if "secondary" in out and "secondary" in y:
            weight_s = torch.where(y["secondary"] > 0.5, torch.tensor(pos_weight_secondary, device=device), torch.tensor(1.0, device=device))
            loss_s = (weight_s * (-(y["secondary"] * torch.log(out["secondary"] + 1e-8) +
                                    (1 - y["secondary"]) * torch.log(1 - out["secondary"] + 1e-8)))).mean()
            loss = loss + loss_weight_secondary * loss_s

        optimizer.zero_grad()
        loss.backward()
        torch.nn.utils.clip_grad_norm_(model.parameters(), 5.0)
        optimizer.step()

        total_loss += float(loss.detach().cpu().item())
        n_batches += 1

    return total_loss / max(n_batches, 1)


@torch.no_grad()
def evaluate(model, loader, device) -> Dict[str, float]:
    model.eval()
    y_true_p, y_pred_p = [], []
    y_true_s, y_pred_s = [], []

    for dense, sparse, y in loader:
        dense, sparse, y = collate_to_device((dense, sparse, y), device)
        out = model(dense, sparse)

        y_true_p.append(y["primary"].cpu().numpy())
        y_pred_p.append(out["primary"].cpu().numpy())
        if "secondary" in out and "secondary" in y:
            y_true_s.append(y["secondary"].cpu().numpy())
            y_pred_s.append(out["secondary"].cpu().numpy())

    def _metrics(y_true_list, y_pred_list):
        if len(y_true_list) == 0:
            return {"auc": float("nan"), "pr_auc": float("nan")}
        y_true = np.vstack(y_true_list).ravel()
        y_pred = np.vstack(y_pred_list).ravel()
        try:
            auc = roc_auc_score(y_true, y_pred)
        except Exception:
            auc = float("nan")
        try:
            pr_auc = average_precision_score(y_true, y_pred)
        except Exception:
            pr_auc = float("nan")
        return {"auc": float(auc), "pr_auc": float(pr_auc)}

    m_primary = _metrics(y_true_p, y_pred_p)
    m_secondary = _metrics(y_true_s, y_pred_s) if len(y_true_s) > 0 else {"auc": float("nan"), "pr_auc": float("nan")}
    return {"primary_auc": m_primary["auc"], "primary_pr_auc": m_primary["pr_auc"],
            "secondary_auc": m_secondary["auc"], "secondary_pr_auc": m_secondary["pr_auc"]}


# ---------------------------
# Main
# ---------------------------

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--data_path", type=str, required=True,
                        help="Parquet 路径（支持 glob），如 oss://.../dt=2025-08-17-11-now/*.parquet 或 /data/**/*.parquet")
    parser.add_argument("--schema_path", type=str, required=True, help="schema.yaml 路径")
    parser.add_argument("--output_dir", type=str, required=True, help="模型与工件输出目录（本地或 oss://）")
    parser.add_argument("--epochs", type=int, default=2)
    parser.add_argument("--batch_size", type=int, default=4096)
    parser.add_argument("--lr", type=float, default=1e-3)
    parser.add_argument("--hidden", type=str, default="256,128,64")
    parser.add_argument("--dropout", type=float, default=0.1)
    parser.add_argument("--activation", type=str, default="relu")
    parser.add_argument("--hash_buckets", type=int, default=1_000_000, help="默认每个稀疏特征的哈希桶大小")
    parser.add_argument("--valid_ratio", type=float, default=0.1)
    parser.add_argument("--secondary_task", action="store_true", help="是否同时训练 conv_label 作为副任务")
    parser.add_argument("--primary_label", type=str, default="click_has_click")
    parser.add_argument("--secondary_label", type=str, default="conv_label")
    parser.add_argument("--seed", type=int, default=2025)
    args = parser.parse_args()

    np.random.seed(args.seed)
    torch.manual_seed(args.seed)

    schema = load_yaml(args.schema_path)
    dense_cols = [f["name"] for f in schema.get("dense_features", [])]
    sparse_cfg = schema.get("sparse_features", [])
    sparse_cols = [f["name"] for f in sparse_cfg]
    labels_cfg = schema.get("labels", [])
    label_names = [l["name"] for l in labels_cfg]

    assert args.primary_label in label_names, f"primary_label {args.primary_label} 不在 schema.labels 中: {label_names}"
    use_secondary = args.secondary_task and (args.secondary_label in label_names)

    # 列出数据文件
    files = list_parquet_files(args.data_path, oss_key=os.environ["OSS_ACCESS_KEY_ID"], oss_secret=os.environ["OSS_ACCESS_KEY_SECRET"])
    if len(files) == 0:
        raise RuntimeError(f"未找到 Parquet 文件: {args.data_path}")
    print(f"发现 {len(files)} 个 Parquet 文件。")
    for f in files:
        print(f"  - {f}")
    # 训练/验证文件划分（按文件级别简单切分）
    n_valid = max(1, int(len(files) * args.valid_ratio))
    valid_files = files[:n_valid]
    train_files = files[n_valid:]
    print(f"训练文件: {len(train_files)}，验证文件: {len(valid_files)}")

    # 拟合 dense scaler
    scaler = fit_dense_scaler(train_files, dense_cols) if len(dense_cols) > 0 else None

    # 稀疏哈希桶大小（可对部分高频特征单独设更大桶）
    hash_bucket_sizes = {name: int(s.get("hash_buckets", args.hash_buckets)) for name, s in
                         ((f["name"], f) for f in sparse_cfg)}

    # 估计类别不平衡权重（仅主任务）
    pos_w_primary = estimate_pos_weight(train_files, args.primary_label)
    # 副任务（若启用）也估计
    pos_w_secondary = estimate_pos_weight(train_files, args.secondary_label) if use_secondary else 1.0
    print(f"pos_weight (primary={args.primary_label}): {pos_w_primary:.3f}")
    if use_secondary:
        print(f"pos_weight (secondary={args.secondary_label}): {pos_w_secondary:.3f}")

    # DataLoader
    train_ds = ParquetIterableDataset(
        files=train_files,
        schema=schema,
        dense_scaler=scaler,
        hash_bucket_sizes=hash_bucket_sizes,
        primary_label=args.primary_label,
        secondary_label=args.secondary_label if use_secondary else None,
        batch_size=args.batch_size,
        shuffle_rowgroups=True,
        seed=args.seed
    )
    valid_ds = ParquetIterableDataset(
        files=valid_files,
        schema=schema,
        dense_scaler=scaler,
        hash_bucket_sizes=hash_bucket_sizes,
        primary_label=args.primary_label,
        secondary_label=args.secondary_label if use_secondary else None,
        batch_size=args.batch_size,
        shuffle_rowgroups=False,
        seed=args.seed
    )
    train_loader = DataLoader(train_ds, batch_size=None)
    valid_loader = DataLoader(valid_ds, batch_size=None)

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    hidden_dims = [int(x) for x in args.hidden.split(",") if x.strip()]
    model = DeepFFM(
        sparse_config=sparse_cfg,
        hash_bucket_sizes=hash_bucket_sizes,
        dense_dim=len(dense_cols),
        hidden_dims=hidden_dims,
        dropout=args.dropout,
        activation=args.activation,
        task_secondary=use_secondary
    ).to(device)

    optimizer = optim.Adam(model.parameters(), lr=args.lr)

    # 训练
    # 组织成字典
    # 用户特征
    user_dense = torch.tensor([[0.1, -0.3, 1.2]])
    user_sparse = torch.tensor([[123, 45, 678]])  # 哈希后的 ID

    # 广告特征
    ad_dense = torch.tensor([[0.2, 0.7]])
    ad_sparse = torch.tensor([[987, 654]])

    # 组织成字典
    example_input = {
        "user_dense": user_dense,
        "user_sparse": user_sparse,
        "ad_dense": ad_dense,
        "ad_sparse": ad_sparse,
    }

    best_auc = -1.0
    metrics_log = []
    for epoch in range(1, args.epochs + 1):
        tr_loss = train_one_epoch(
            model, train_loader, optimizer, device,
            pos_weight_primary=pos_w_primary,
            pos_weight_secondary=pos_w_secondary
        )
        val_metrics = evaluate(model, valid_loader, device)
        metrics = {"epoch": epoch, "train_loss": tr_loss, **val_metrics}
        metrics_log.append(metrics)
        print(f"[Epoch {epoch}] loss={tr_loss:.5f} | "
              f"AUC(p)={val_metrics['primary_auc']:.4f} PR(p)={val_metrics['primary_pr_auc']:.4f} "
              f"| AUC(s)={val_metrics['secondary_auc']:.4f} PR(s)={val_metrics['secondary_pr_auc']:.4f}")

        # 简单以主任务 AUC 作为挑选最佳
        if not math.isnan(val_metrics["primary_auc"]) and val_metrics["primary_auc"] > best_auc:
            best_auc = val_metrics["primary_auc"]
            # 保存 best
            # save_all_v2(model, example_input, scaler, schema, hash_bucket_sizes, metrics_log, args.output_dir, tag="best")

    # 训练完成后再存一份 last
    save_all_v2(model, example_input, scaler, schema, hash_bucket_sizes, metrics_log, args.output_dir, tag="last")
    print(f"训练完成。best_primary_auc={best_auc:.4f}. 工件已保存到: {args.output_dir}")


def save_all(model, scaler, schema, hash_bucket_sizes, metrics_log, output_dir: str, tag: str = "best"):
    ensure_dir(output_dir)

    # model
    model_path = os.path.join(output_dir, f"model_{tag}.pt")
    buf = io.BytesIO()
    torch.save(model.state_dict(), buf)
    fs_write_bytes(model_path, buf.getvalue())

    # scaler
    import pickle
    scaler_path = os.path.join(output_dir, f"scaler_{tag}.pkl")
    fs_write_bytes(scaler_path, pickle.dumps(scaler))

    # schema copy
    schema_path = os.path.join(output_dir, f"schema_{tag}.yaml")
    fs_write_text(schema_path, yaml.safe_dump(schema, allow_unicode=True, sort_keys=False))

    # feature info
    feat_info = {
        "dense_features": [f["name"] for f in schema.get("dense_features", [])],
        "sparse_features": {
            f["name"]: {
                "embedding_dim": int(f.get("embedding_dim", 8)),
                "hash_buckets": int(hash_bucket_sizes[f["name"]]),
            } for f in schema.get("sparse_features", [])
        }
    }
    feat_info_path = os.path.join(output_dir, f"feature_info_{tag}.json")
    fs_write_json(feat_info_path, feat_info)

    # metrics
    metrics_path = os.path.join(output_dir, f"training_metrics_{tag}.json")
    fs_write_json(metrics_path, metrics_log)


def save_all_v1(model, scaler, schema, hash_bucket_sizes, metrics_log, output_dir: str, tag: str = "best"):
    ensure_dir(output_dir)

    # 本地输出目录
    output_dir_local = "./local_model_out"
    ensure_dir(output_dir_local)

    # ================== 保存模型 ==================
    local_model_path = os.path.join(output_dir_local, f"model_{tag}.pt")
    torch.save(model.state_dict(), local_model_path)  # 优先保存本地

    # 再保存到远程
    model_path = os.path.join(output_dir, f"model_{tag}.pt")
    fs_upload_file(local_model_path, model_path)
    # with open(local_model_path, "rb") as f:
    #     buf = f.read()
    # fs_write_bytes(model_path, buf)
    

    # ================== 保存 scaler ==================
    local_scaler_path = os.path.join(output_dir_local, f"scaler_{tag}.pkl")
    with open(local_scaler_path, "wb") as f:
        pickle.dump(scaler, f)

    scaler_path = os.path.join(output_dir, f"scaler_{tag}.pkl")
    with open(local_scaler_path, "rb") as f:
        fs_write_bytes(scaler_path, f.read())

    # ================== 保存 schema ==================
    local_schema_path = os.path.join(output_dir_local, f"schema_{tag}.yaml")
    with open(local_schema_path, "w", encoding="utf-8") as f:
        yaml.safe_dump(schema, f, allow_unicode=True, sort_keys=False)

    schema_path = os.path.join(output_dir, f"schema_{tag}.yaml")
    with open(local_schema_path, "r", encoding="utf-8") as f:
        fs_write_text(schema_path, f.read())

    # ================== 保存 feature_info ==================
    feat_info = {
        "dense_features": [f["name"] for f in schema.get("dense_features", [])],
        "sparse_features": {
            f["name"]: {
                "embedding_dim": int(f.get("embedding_dim", 8)),
                "hash_buckets": int(hash_bucket_sizes[f["name"]]),
            } for f in schema.get("sparse_features", [])
        }
    }

    local_feat_info_path = os.path.join(output_dir_local, f"feature_info_{tag}.json")
    with open(local_feat_info_path, "w", encoding="utf-8") as f:
        json.dump(feat_info, f, indent=2, ensure_ascii=False)

    feat_info_path = os.path.join(output_dir, f"feature_info_{tag}.json")
    with open(local_feat_info_path, "r", encoding="utf-8") as f:
        fs_write_json(feat_info_path, json.load(f))

    # ================== 保存 metrics ==================
    local_metrics_path = os.path.join(output_dir_local, f"training_metrics_{tag}.json")
    with open(local_metrics_path, "w", encoding="utf-8") as f:
        json.dump(metrics_log, f, indent=2, ensure_ascii=False)

    metrics_path = os.path.join(output_dir, f"training_metrics_{tag}.json")
    with open(local_metrics_path, "r", encoding="utf-8") as f:
        fs_write_json(metrics_path, json.load(f))

def save_all_v2(
    model,
    example_input,
    scaler,
    schema,
    hash_bucket_sizes,
    metrics_log,
    output_dir: str,
    tag: str = "best",
    model_type: str = "script"  # 可选: "state_dict", "trace", "script"
):
    """
    保存模型 + scaler + feature_info + schema + metrics
    支持本地优先保存，并同步到远程
    """
    # ================== 目录准备 ==================
    os.makedirs(output_dir, exist_ok=True)
    output_dir_local = "./local_model_out"
    os.makedirs(output_dir_local, exist_ok=True)

    # ================== 保存模型 ==================
    local_model_path = os.path.join(output_dir_local, f"model_{tag}.pt")
    model.eval()
    if model_type == "state_dict":
        torch.save(model.state_dict(), local_model_path)
    elif model_type == "trace":
        traced_model = torch.jit.trace(model, example_input)
        traced_model.save(local_model_path)
    elif model_type == "script":
        scripted_model = torch.jit.script(model)
        scripted_model.save(local_model_path)
    else:
        raise ValueError(f"Unsupported model_type: {model_type}")
    print(f"✅ 模型已保存本地 ({model_type}): {local_model_path}")

    # 同步到远程
    model_path = os.path.join(output_dir, f"model_{tag}.pt")
    fs_upload_file(local_model_path, model_path)

    # ================== 导出 dense scaler JSON ==================
    dense_features = [f["name"] for f in schema.get("dense_features", [])]
    mean_list = [float(scaler.mean_[i]) for i in range(len(dense_features))]
    scale_list = [float(scaler.scale_[i]) for i in range(len(dense_features))]

    scaler_json = {
        "dense_features": dense_features,
        "mean": mean_list,
        "scale": scale_list
    }
    local_scaler_json_path = os.path.join(output_dir_local, f"scaler_{tag}.json")
    with open(local_scaler_json_path, "w", encoding="utf-8") as f:
        json.dump(scaler_json, f, indent=2, ensure_ascii=False)
    scaler_json_path = os.path.join(output_dir, f"scaler_{tag}.json")
    fs_write_json(scaler_json_path, scaler_json)
    print(f"✅ Scaler 已保存本地及远程 JSON: {scaler_json_path}--{local_scaler_json_path}")

    # ================== 保存 schema ==================
    local_schema_path = os.path.join(output_dir_local, f"schema_{tag}.yaml")
    with open(local_schema_path, "w", encoding="utf-8") as f:
        yaml.safe_dump(schema, f, allow_unicode=True, sort_keys=False)
    schema_path = os.path.join(output_dir, f"schema_{tag}.yaml")
    fs_write_text(schema_path, yaml.safe_dump(schema, allow_unicode=True, sort_keys=False))
    print(f"✅ Schema 已保存本地及远程 YAML: {schema_path}--{local_schema_path}")

    # ================== 导出 feature_info JSON ==================
    sparse_features = {
        f["name"]: {
            "embedding_dim": int(f.get("embedding_dim", 8)),
            "hash_buckets": int(hash_bucket_sizes[f["name"]])
        } for f in schema.get("sparse_features", [])
    }
    feature_info = {
        "dense_features": dense_features,
        "sparse_features": sparse_features
    }
    local_feat_info_path = os.path.join(output_dir_local, f"feature_info_{tag}.json")
    with open(local_feat_info_path, "w", encoding="utf-8") as f:
        json.dump(feature_info, f, indent=2, ensure_ascii=False)
    feat_info_path = os.path.join(output_dir, f"feature_info_{tag}.json")
    fs_write_json(feat_info_path, feature_info)
    print(f"✅ Feature info 已保存本地及远程 JSON: {feat_info_path}--{local_feat_info_path}")

    # ================== 保存 metrics ==================
    local_metrics_path = os.path.join(output_dir_local, f"training_metrics_{tag}.json")
    with open(local_metrics_path, "w", encoding="utf-8") as f:
        json.dump(metrics_log, f, indent=2, ensure_ascii=False)
    metrics_path = os.path.join(output_dir, f"training_metrics_{tag}.json")
    fs_write_json(metrics_path, metrics_log)
    print(f"✅ 训练指标已保存本地及远程 JSON: {metrics_path}--{local_metrics_path}")

    print("✅ 模型及相关文件已全部保存")


if __name__ == "__main__":
    main()

# dt=2025-08-26-16-now
'''
python3 nova_train_dnn.py \
  --data_path "oss://novadata-emr/ad_ml/sunketo/test/sample_features/dt=2025-08-26-16-now/*.parquet" \ 
  --schema_path ./nova_schema.yaml \
  --output_dir "oss://novadata-emr/ad_ml/sunketo/test/model_out/SktLocalCtrJobV1/" \
  --epochs 3 \
  --batch_size 8192 \
  --lr 1e-3 \
  --hidden "256,128,64" \
  --dropout 0.1 \
  --hash_buckets 1000000 \
  --primary_label click_has_click \
  --secondary_task \
  --secondary_label conv_label
'''
