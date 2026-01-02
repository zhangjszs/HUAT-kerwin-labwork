# 计算机网络课程实验

本项目包含计算机网络课程的实验代码，包括网络协议实现和网络编程。

## 环境要求

- **操作系统**: Linux/macOS (推荐)，或Windows with WSL
- **编译器**: GCC for C, Python 3.10+
- **网络工具**: Wireshark, tcpdump

## 依赖安装

### C 程序
```bash
sudo apt install build-essential
```

### Python 程序
```bash
pip install scapy socket
```

## 运行步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/计算机网络
   ```

2. 编译运行 C 代码：
   ```bash
   cd experiment1
   gcc client.c -o client
   ./client
   ```

3. 运行 Python 脚本：
   ```bash
   python network_scanner.py
   ```

## 目录结构

- `experiment1/`：Socket编程基础
- `experiment2/`：TCP/UDP实现
- `experiment3/`：HTTP协议
- `experiment4/`：网络安全
- `experiment5/`：路由算法
- ... 其他实验
- `Improve_RED/`：RED算法改进
- `review material/`：复习资料

## 实验说明

每个实验包含源代码和实验报告。

## 贡献

欢迎添加网络编程实验。

## 许可

MIT License