<!--
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-03-15 09:24:54
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-03-15 11:59:05
 * @FilePath: \HUAT-kerwin-labwork\数据结构课设\README.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
## 数据结构课程项目：迷宫求解器
该项目旨在解决一个由 m×n 矩阵表示的迷宫问题，其中 0 和 1 分别表示迷宫中的通路和障碍物。目标是设计一个程序，找到从入口到出口的路径，或确定没有可行路径。
## 要求
首先实现一个使用链表作为存储结构的栈数据结构，然后编写一个非递归程序来解决迷宫问题。  
找到的路径应以三元组 (i, j, d) 的形式输出，其中 (i, j) 表示迷宫中的一个坐标，d 表示前往下一个坐标的方向。例如，对于给定的迷宫数据，可能的路径输出为：(1, 1, 1), (1, 2, 2), (2, 2, 2), (3, 2, 3), (3, 1, 2), ...  
编写递归算法，找到迷宫中所有可能的路径。
以方阵的形式输出迷宫及其路径。  
除了基本要求，我还添加了以下功能：  
使用**广度优先搜索（BFS）**、**深度优先搜索（DFS）** 和 **Astart**算法来解决迷宫问题。
使用 **Qt** 进行迷宫解决过程的可视化。
## 目录
- castar.h castar.cpp A*算法和bfs算法的实现
- widget.h widget.cpp qt界面的组件的实现
- constants.h 一些常量的定义
- main.cpp 主函数
- widget.ui qt界面的ui文件
- cmakeLists.txt cmake文件
- report目录下是要求和最后的报告
## 项目要求
- qt version: 5.15.2 or 6.6.2
## 贡献

欢迎各种形式的贡献，包括但不限于代码、文档和建议。

## 许可
本项目基于 MIT 许可证进行许可，详见 LICENSE 文件。

