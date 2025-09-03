import torch
from torch import nn, optim

# 检测可用设备
device = (
    "mps" if torch.backends.mps.is_available() else  # Apple Silicon (M1/M2/M3/M4)
    "cuda" if torch.cuda.is_available() else        # NVIDIA GPU
    "cpu"                                          # 普通CPU
)
print(f"Using device: {device}")

# 生成数据并移到对应设备
x = torch.rand(100, 1).to(device)
y = 3 * x + 2 + 0.1 * torch.randn(100, 1).to(device)

# 定义模型并移到设备
model = nn.Linear(1, 1).to(device)
loss_fn = nn.MSELoss()
opt = optim.SGD(model.parameters(), lr=0.1)

# 训练循环
for epoch in range(100):
    pred = model(x)
    loss = loss_fn(pred, y)
    opt.zero_grad()
    loss.backward()
    opt.step()
    print(f"Epoch {epoch}: loss={loss.item():.4f}")

# 打印最终参数
print("\nFinal parameters:")
print(f"Weight: {model.weight.data.item():.4f}")
print(f"Bias: {model.bias.data.item():.4f}")

# 保存模型（兼容所有设备）
torch.save(model.state_dict(), 'linear_regression_model.pth')
print("\nModel saved to 'linear_regression_model.pth'")

# 加载模型示例（需要先创建模型结构）
# loaded_model = nn.Linear(1, 1).to(device)
# loaded_model.load_state_dict(torch.load('linear_regression_model.pth'))

# # Print final parameters
# print("\nFinal parameters:")
# print(f"Weight: {model.weight.data.item():.4f}, Bias: {model.bias.data.item():.4f}")

# # Save the entire model
# torch.save(model, 'linear_regression_model.pth')

# # Save just the state_dict (recommended way)
# torch.save(model.state_dict(), 'linear_regression_state_dict.pth')

# print("\nModel files saved:")
# print("- linear_regression_model.pth (entire model)")
# print("- linear_regression_state_dict.pth (state dictionary)")