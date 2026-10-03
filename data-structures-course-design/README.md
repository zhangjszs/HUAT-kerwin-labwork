# 数据结构课程设计：迷宫求解与寻路算法可视化

![Qt](https://img.shields.io/badge/Qt-6.7+-green.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Python](https://img.shields.io/badge/Python-PyQt5-yellow.svg)

以「迷宫寻路」为载体的数据结构课程设计：从链表栈、递归搜索出发，最终实现一个支持 **A\* / BFS / DFS / Dijkstra / JPS** 五种寻路算法的 Qt 可视化系统，并提供 Python/PyQt5 对照实现。

![界面截图](./81A5F206980373F0A3F35A7F230_C305AE68_27744.png)

## 项目亮点

- **五种寻路算法**：A\*、BFS、DFS、Dijkstra、JPS（跳点搜索），可切换对比搜索过程与性能
- **工程化 C++ 结构**：`core`（算法）/ `models`（迷宫网格）/ `services`（文件 IO）三层分离，算法与界面解耦
- **可视化演示**：逐步渲染搜索前沿与最终路径，支持 4/8 方向移动、多种启发函数（曼哈顿/欧几里得/切比雪夫/Octile）
- **双语言对照实现**：C++/Qt6 为主实现，`python版本/` 提供同等功能的 PyQt5 实现，便于交叉验证
- **保证可解的迷宫生成** 与地图导入/导出（`map.txt`）、暗黑/明亮主题

## 快速开始

### C++ / Qt6（主实现）

```bash
git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
cd HUAT-kerwin-labwork/data-structures-course-design/coding/MazeProject

mkdir build && cd build
cmake ..
make
./MazeSolver
```

依赖：Qt 6.7+、CMake ≥ 3.10、C++17 编译器。

### Python / PyQt5（对照实现）

```bash
cd HUAT-kerwin-labwork/data-structures-course-design/python版本
pip install PyQt5
python Maze.py
```

## 目录结构

| 路径 | 说明 |
| ---- | ---- |
| [`coding/MazeProject/`](./coding/MazeProject/) | C++/Qt6 主实现（约 1700 行算法核心 + Qt 界面），详见其 [README](./coding/MazeProject/README.md) |
| [`python版本/`](./python%E7%89%88%E6%9C%AC/) | Python/PyQt5 对照实现（约 440 行单文件） |
| [`report/`](./report/) | 课程设计报告（含设计思路与算法分析） |

## 算法说明

| 算法 | 最优性 | 特点 |
| ---- | ------ | ---- |
| BFS | 保证最短路径（等权网格） | 逐层扩展，搜索前沿呈环形 |
| DFS | 不保证 | 快速深入，适合演示搜索过程差异 |
| Dijkstra | 保证最优 | 统一代价扩展，是 A\* 的 h=0 特例 |
| A\* | 启发函数可采纳时最优 | f = g + h，效率与最优性的平衡 |
| JPS | 保证最优 | 网格专用跳点剪枝，大地图上远快于 A\* |

课程设计报告中对各算法的时间/空间复杂度与实验对比有完整分析。

---

> 📁 本项目是 [HUAT-kerwin-labwork](https://github.com/zhangjszs/HUAT-kerwin-labwork) 课程作品集的一部分，项目展示页见 [huat-showcase](https://github.com/zhangjszs/huat-showcase)。
