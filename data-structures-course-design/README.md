<!--
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-03-15 09:24:54
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-03-15 11:59:05
 * @FilePath: \HUAT-kerwin-labwork\数据结构课设\README.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
# 数据结构课程设计：迷宫求解器

该项目旨在解决一个由 m×n 矩阵表示的迷宫问题，其中 0 和 1 分别表示迷宫中的通路和障碍物。目标是设计一个程序，找到从入口到出口的路径，或确定没有可行路径。

## 环境要求

- **操作系统**: Windows/Linux/macOS
- **Qt**: 5.15.2 或 6.6.2
- **编译器**: GCC/MinGW
- **构建工具**: CMake

## 依赖安装

```bash
# 安装 Qt
# 下载 Qt Creator 或使用包管理器
sudo apt install qtcreator

# 安装 CMake
sudo apt install cmake
```

## 运行步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/数据结构课设
   ```

2. 构建项目：
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. 运行程序：
   ```bash
   ./MazeSolver
   ```

## 目录结构

- `coding/`：C++源码
- `python版本/`：Python实现
- `report/`：实验报告

## 功能说明

- 使用链表实现的栈数据结构
- 非递归迷宫求解
- 递归算法寻找所有路径
- BFS、DFS、A*算法实现
- Qt可视化界面

## 贡献

欢迎各种形式的贡献，包括但不限于代码、文档和建议。

## 许可

本项目基于 MIT 许可证进行许可，详见 LICENSE 文件。

