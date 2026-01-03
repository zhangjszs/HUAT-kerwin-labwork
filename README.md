<div align="center">

# 🎓 HUAT 计科课程设计与实验代码库

**HUAT Computer Science Lab Work Repository**

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![GitHub Stars](https://img.shields.io/github/stars/zhangjszs/HUAT-kerwin-labwork?style=social)](https://github.com/zhangjszs/HUAT-kerwin-labwork/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/zhangjszs/HUAT-kerwin-labwork?style=social)](https://github.com/zhangjszs/HUAT-kerwin-labwork/network/members)
[![GitHub Issues](https://img.shields.io/github/issues/zhangjszs/HUAT-kerwin-labwork)](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues)

*湖北汽车工业学院 计算机科学与技术专业 课程实验代码集合*

[简介](#-简介) • [技术栈](#️-技术栈与规范) • [目录结构](#-目录结构) • [快速开始](#-快速开始) • [贡献指南](#-贡献) • [联系方式](#-联系我)

---

</div>

## 📖 目录

- [👋 简介](#-简介)
- [🛠️ 技术栈与规范](#️-技术栈与规范)
- [📁 目录结构](#-目录结构)
  - [核心课程](#核心课程)
  - [编程语言与开发](#编程语言与开发)
  - [专业特色](#专业特色)
- [🚀 快速开始](#-快速开始)
- [📊 项目亮点](#-项目亮点)
- [🤝 贡献](#-贡献)
- [📜 许可](#-许可)
- [📫 联系我](#-联系我)

## 👋 简介

> 💡 记录学习轨迹，分享技术成长

欢迎来到 `HUAT-kerwin-labwork`！

本仓库收录了我在**湖北汽车工业学院 (HUAT) 计算机科学与技术专业 (2022级)** 学习期间完成的课程设计和实验代码。创建此仓库旨在：

- 📚 **记录学习过程**：系统化整理大学期间的编程实践
- 🎯 **分享学习成果**：为学弟学妹和相关课程同学提供参考
- 🔍 **持续改进**：通过开源协作不断完善代码质量

### ⚠️ 重要提示

- 📝 本仓库代码**仅供学习交流和参考**，欢迎通过 [Issues](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues) 提出建议
- 🏆 **课程成绩**：除 JavaWeb 课程设计（97分）外，其他课程设计均获得满分
- 🚧 **未来规划**：研究生阶段将上传本科期间的竞赛项目资料和代码

## 🛠️ 技术栈与规范

### 编程语言
![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Java](https://img.shields.io/badge/Java-ED8B00?style=flat&logo=openjdk&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Assembly](https://img.shields.io/badge/Assembly-654FF0?style=flat&logo=assemblyscript&logoColor=white)

### 开发工具与框架
- **IDE**: Visual Studio Code, Android Studio, Qt Creator, IDEA
- **框架**: Spring Boot, Qt Framework
- **数据库**: MySQL, SQLite
- **版本控制**: Git & GitHub

### 编码规范
- 📌 **C++ 标准**：自 2024年4月10日起，所有 C++ 代码遵循 **C++11** 标准
- 📌 **代码风格**：注重代码可读性、可维护性和注释完整性
- 📌 **命名规范**：使用有意义的变量名和函数名，遵循驼峰命名法

## 📁 目录结构

仓库按照课程名称进行组织，结构清晰，便于查阅。每个目录下都包含相应的 `README.md` 文件，详细介绍实验背景、要求和实现逻辑。

### 核心课程

#### 计算机基础

| 课程           | 目录                         | 主要内容               | 技术栈               |
| -------------- | ---------------------------- | ---------------------- | -------------------- |
| 数据结构       | `数据结构课程实验`           | 基础数据结构实验       | C++                  |
| 数据结构课设   | `数据结构课设`               | 图形化界面的算法可视化 | Qt, C++ (DFS/BFS/A*) |
| 算法设计与分析 | `算法设计与分析-c++语言描述` | 核心算法实现与分析     | C++11                |
| 操作系统       | `操作系统`                   | 进程管理、内存管理等   | C                    |
| 计算机组成原理 | `计算机组成原理`             | 硬件实验与仿真         | -                    |
| 计算机网络     | `计算机网络`                 | 网络协议实现           | Python, C            |
| 编译原理       | `编译原理`                   | 词法分析、语法分析     | C++                  |
| 数据库         | `数据库`                     | SQL实践与应用          | MySQL, SQL           |

#### 数据科学与分析

| 课程             | 目录                       | 主要内容          | 技术栈          |
| ---------------- | -------------------------- | ----------------- | --------------- |
| 数据采集与预处理 | `数据采集与预处理`         | 数据清洗与处理    | Python          |
| 数据分析与可视化 | `数据分析与可视化`         | 数据分析实验      | Python, Jupyter |
| 数据分析课设     | `数据分析与可视化课程设计` | 竞赛项目+课程设计 | Python, Pandas  |

### 编程语言与开发

#### Java 系列

| 课程          | 目录                 | 主要内容         | 技术栈             |
| ------------- | -------------------- | ---------------- | ------------------ |
| Java 基础实验 | `HUATJavaWebLab`     | JavaWeb 基础实验 | Java, Servlet      |
| Java 课程作业 | `schoolJavaHomework` | 课程作业集合     | Java               |
| Java 课程设计 | `java课程设计`       | Java 小游戏项目  | Java, Swing        |
| JavaWeb 课设  | `javaweb课程设计`    | 简单博客系统     | Spring Boot, MySQL |

#### 系统编程

| 课程           | 目录             | 主要内容         | 技术栈   |
| -------------- | ---------------- | ---------------- | -------- |
| C 语言课设     | `C语言课设`      | 车辆索赔管理系统 | C, CMake |
| 微机原理与接口 | `微机原理和接口` | 汇编语言编程     | Assembly |
| Linux 系统应用 | `linux系统应用`  | Linux 环境实验   | Shell, C |

### 专业特色

#### 汽车相关

| 课程             | 目录                       | 主要内容     |
| ---------------- | -------------------------- | ------------ |
| 汽车概论A        | `汽车概论A`                | 汽车基础知识 |
| 智能网联汽车技术 | `智能网联汽车技术`         | 智能汽车技术 |
| 智能网联汽车课设 | `智能网联汽车技术课程设计` | FSD展示项目  |

#### 其他专业课程

| 课程         | 目录                   | 主要内容     | 技术栈        |
| ------------ | ---------------------- | ------------ | ------------- |
| 嵌入式系统   | `嵌入式系统原理与应用` | 嵌入式开发   | C, ARM        |
| Android 开发 | `android移动开发`      | 移动应用开发 | Java, Android |
| 软件工程     | `软件工程`             | 软件开发流程 | UML, Java     |
| 项目工程管理 | `项目工程管理`         | 项目管理实践 | -             |

## 🚀 快速开始

### 克隆仓库

```bash
# 克隆项目到本地
git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git

# 进入项目目录
cd HUAT-kerwin-labwork

# 初始化 Git LFS（首次克隆后）
git lfs install
```

### 浏览课程代码

```bash
# 查看数据结构课程
cd 数据结构课程实验

# 查看 Java 课程设计
cd javaweb课程设计
```

### 运行示例项目

#### C/C++ 项目（以 C 语言课设为例）

```bash
cd C语言课设
mkdir build && cd build
cmake ..
cmake --build .
./VehicleClaimManagement
```

#### Java 项目

```bash
cd javaweb课程设计
# 使用 Maven/Gradle 构建
mvn clean install
mvn spring-boot:run
```

#### Python 项目

```bash
cd 数据分析与可视化
pip install -r requirements.txt
jupyter notebook
```

## 📊 项目亮点

### 🎯 数据结构课设
- ✨ 使用 **Qt** 框架开发的图形化界面
- 🔍 实现了 **DFS、BFS、A*** 三种经典搜索算法的可视化
- 🎨 直观的动画演示，便于理解算法执行过程

### 💻 JavaWeb 课程设计
- 🌐 完整的**博客系统**实现
- 🔐 用户认证与权限管理
- 📝 文章发布、编辑、评论功能
- 🎯 **成绩：97分**

### 🚗 C 语言课设
- 📋 **车辆索赔管理系统**
- 💾 文件持久化存储
- 🔍 多条件查询与统计
- ⚙️ 使用 CMake 构建系统

### 🤖 智能网联汽车课设
- 🚘 自动驾驶系统展示
- 🌐 基于 Web 的可视化界面
- 📱 响应式设计

## 💡 学习建议

如果你是 HUAT 的同学，建议按以下顺序学习：

1. **基础阶段**：C语言课设 → 数据结构 → 算法设计
2. **进阶阶段**：操作系统 → 计算机网络 → 数据库
3. **应用阶段**：JavaWeb → Android 开发 → 数据分析
4. **综合阶段**：课程设计与竞赛项目

## 🤝 贡献

非常欢迎大家参与到本仓库的维护和改进中！

### 如何贡献

1. **🐛 报告问题**
   - 发现 Bug 或有改进建议？请通过 [Issues](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues) 提交
   - 提交 Issue 时请详细描述问题和复现步骤

2. **💡 贡献代码**
   ```bash
   # Fork 本仓库
   # 克隆你的 fork
   git clone https://github.com/YOUR_USERNAME/HUAT-kerwin-labwork.git
   
   # 创建新分支
   git checkout -b feature/your-feature
   
   # 提交更改
   git commit -m "Add: 你的功能描述"
   
   # 推送到你的 fork
   git push origin feature/your-feature
   
   # 创建 Pull Request
   ```

3. **📝 完善文档**
   - 补充实验说明
   - 改进代码注释
   - 添加使用示例

### 贡献者

感谢所有为本项目做出贡献的同学！

[![Contributors](https://contrib.rocks/image?repo=zhangjszs/HUAT-kerwin-labwork)](https://github.com/zhangjszs/HUAT-kerwin-labwork/graphs/contributors)

## ⚠️ 免责声明

- 📚 本仓库仅供**学习交流**使用
- 🚫 请勿直接抄袭用于课程作业，鼓励独立思考
- ✅ 欢迎参考代码思路，但请理解后用自己的方式实现
- 💼 代码质量参差不齐，使用时请注意甄别

## 📜 许可

本项目采用 **MIT 许可证** - 详见 [LICENSE](LICENSE) 文件

```
MIT License

Copyright (c) 2024 zhangjszs

您可以自由地：
✓ 使用本项目代码
✓ 复制、修改和分发
✓ 用于商业目的

前提是：
• 保留版权声明
• 保留许可证声明
```

## 📫 联系我

<div align="center">

### 💬 欢迎交流讨论！

[![GitHub](https://img.shields.io/badge/GitHub-zhangjszs-181717?style=flat&logo=github)](https://github.com/zhangjszs)
[![Email](https://img.shields.io/badge/Email-zhangjszs@foxmail.com-EA4335?style=flat&logo=gmail)](mailto:zhangjszs@foxmail.com)
[![QQ](https://img.shields.io/badge/QQ-2171139160-EB1923?style=flat&logo=tencentqq)](http://wpa.qq.com/msgrd?v=3&uin=2171139160&site=qq&menu=yes)

</div>

| 联系方式        | 地址                                                                  | 说明                   |
| --------------- | --------------------------------------------------------------------- | ---------------------- |
| 📧 主邮箱        | zhangjszs@foxmail.com                                                 | 推荐，回复较快         |
| 📮 备用邮箱      | 2171139160@qq.com                                                     | 备用联系方式           |
| 💬 GitHub Issues | [提交 Issue](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues) | **推荐**，便于问题追踪 |
| 🌐 个人博客      | [即将上线]                                                            | 技术博客与学习笔记     |

---

<div align="center">

### ⭐ 如果这个项目对你有帮助，请给它一个 Star！

**Made with ❤️ by [zhangjszs](https://github.com/zhangjszs)**

*最后更新：2025年10月*

</div>
