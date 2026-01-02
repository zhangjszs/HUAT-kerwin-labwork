# 操作系统课程实验

本项目包含操作系统课程的实验代码，包括进程管理、内存管理等核心概念的实现。

## 环境要求

- **操作系统**: WSL2 + Ubuntu 22.04 或 Linux/macOS
- **编译器**: GCC 11.4.0 或更高版本
- **Python**: 3.10.2 或更高版本
- **Java**: 17.0.2 或更高版本
- **C++**: C++17 支持

## 依赖安装

### C/C++ 项目
```bash
# 安装 GCC
sudo apt update
sudo apt install build-essential
```

### Python 项目
```bash
pip install psutil matplotlib
```

### Java 项目
```bash
# 安装 JDK
sudo apt install openjdk-17-jdk
```

## 运行步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/操作系统
   ```

2. 编译运行 C 代码：
   ```bash
   cd labwork1
   gcc labwork1.1.cpp -o process_demo
   ./process_demo
   ```

3. 运行 Python 脚本：
   ```bash
   python memory_simulation.py
   ```

## 目录结构

- `labwork1/`：进程管理实验
- `labwork2/`：内存管理实验
- `labwork3/`：文件系统实验
- ... 其他实验

## 实验说明

每个实验文件夹包含：
- 源代码文件
- 实验报告 (PDF/DOC)
- README.md (实验说明)

## 贡献

欢迎提交 Issue 或 Pull Request 改进代码。

## 许可

MIT License