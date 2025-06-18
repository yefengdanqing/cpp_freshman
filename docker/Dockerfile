# 使用阿里云 Ubuntu 22.04 基础镜像
FROM registry.cn-hangzhou.aliyuncs.com/acs/ubuntu:22.04

# 设置环境变量（避免交互式提示）
ENV DEBIAN_FRONTEND=noninteractive

# 1. 更新软件源并安装基础工具（包括 Git 和 SSH 相关依赖）
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    software-properties-common \
    gnupg \
    gpg-agent \
    dirmngr \
    wget \
    telnet \
    net-tools \
    libssl-dev \
    zlib1g-dev \
    iputils-ping \
    ca-certificates \
    git \
    openssh-client \
    && rm -rf /var/lib/apt/lists/*

# 2. 配置 Git（全局设置）
RUN git config --global user.name "" && \
    git config --global user.email "42533457@qq.com"

# 3. 添加 Ubuntu Toolchain PPA 并安装 GCC 12
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1E9377A2BA9EF27F && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
    gcc-12 \
    g++-12 \
    && rm -rf /var/lib/apt/lists/*

# 4. 设置 GCC 12 为默认编译器
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100 && \
    update-alternatives --set gcc /usr/bin/gcc-12 && \
    update-alternatives --set g++ /usr/bin/g++-12

# 5. 安装其他开发工具
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    python3 \
    python3-pip \
    cmake \
    gdb \
    curl \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# 6. 验证安装
RUN gcc --version && \
    g++ --version && \
    git --version && \
    python3 --version && \
    cmake --version && \
    gdb --version && \
    curl --version

# 7. 设置工作目录
WORKDIR /workspace

# 8. 添加 SSH 配置（如果需要克隆私有仓库）
RUN mkdir -p ~/.ssh && \
    chmod 700 ~/.ssh && \
    ssh-keyscan github.com >> ~/.ssh/known_hosts

# 9. 克隆仓库（可选，可以在运行时通过挂载SSH密钥）
# 注意：这里只是演示，实际使用时建议在运行时挂载SSH密钥
# RUN --mount=type=ssh git clone git@github.com:yefengdanqing/cpp_freshman.git /workspace

# 10. 重置环境变量
ENV DEBIAN_FRONTEND=



# $ docker login --username=4253****@qq.com crpi-zx3djchkgmxrf4q0.cn-zhangjiakou.personal.cr.aliyuncs.com
# $ docker tag [ImageId] crpi-zx3djchkgmxrf4q0.cn-zhangjiakou.personal.cr.aliyuncs.com/sunketo/yefengdanqing:[镜像版本号]
# $ docker push crpi-zx3djchkgmxrf4q0.cn-zhangjiakou.personal.cr.aliyuncs.com/sunketo/yefengdanqing:[镜像版本号]