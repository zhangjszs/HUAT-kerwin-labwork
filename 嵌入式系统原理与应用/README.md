# 嵌入式系统原理与应用

本项目包含嵌入式系统课程的实验代码和作业。

## 环境要求

- **操作系统**: Linux (推荐)
- **编译器**: GCC for ARM, Keil MDK
- **硬件**: STM32开发板 (可选)

## 依赖安装

```bash
# 安装交叉编译工具链
sudo apt install gcc-arm-none-eabi
```

## 运行步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/嵌入式系统原理与应用
   ```

2. 编译代码：
   ```bash
   arm-none-eabi-gcc main.c -o firmware.elf
   ```

## 目录结构

- `大作业/`：课程大作业
- `chaoxing/`：超星学习资料

## 实验说明

包含嵌入式编程和硬件控制实验。

## 贡献

欢迎添加嵌入式实验代码。

## 许可

MIT License