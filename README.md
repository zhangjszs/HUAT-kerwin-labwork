<div align="center">

# 🎓 HUAT 计科课程设计与实验代码库

**HUAT Computer Science Lab Work Repository**

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT) •
[![GitHub Stars](https://img.shields.io/github/stars/zhangjszs/HUAT-kerwin-labwork?style=social)](https://github.com/zhangjszs/HUAT-kerwin-labwork/stargazers) •
[![GitHub Forks](https://img.shields.io/github/forks/zhangjszs/HUAT-kerwin-labwork?style=social)](https://github.com/zhangjszs/HUAT-kerwin-labwork/network/members) •
[![GitHub Issues](https://img.shields.io/github/issues/zhangjszs/HUAT-kerwin-labwork)](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues)

*湖北汽车工业学院 计算机科学与技术专业 课程实验代码集合*

[简介](#-简介) • [快速开始](#-快速开始) • [目录](#-目录)

> 📝 **目录命名说明**：所有目录统一使用英文命名，便于 URL 访问和命令行操作。
> | 中文 | 英文 |
> |------|------|
> | 数据结构 | data-structures |
> | 数据结构课设 | data-structures-course-design |
> | 算法设计与分析 | algorithm-design-analysis |
> | 操作系统 | operating-system |
> | 计算机组成原理 | computer-organization |
> | 计算机网络 | computer-network |
> | 编译原理 | compiler-principles |
> | 数据库 | database |
> | Java课程设计 | java-course-design |
> | 汽车概论A | automotive-introduction |
> | 智能网联汽车技术 | intelligent-connected-vehicle |
> | 智能网联汽车技术课程设计 | intelligent-connected-vehicle-course-design |
> | 软件工程 | software-engineering |
> | 项目工程管理 | project-management |

---

</div>

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

## 📁 目录

### 计算机基础

| 课程                                          | 目录       | 技术栈 |
| --------------------------------------------- | ---------- | ------ |
| [数据结构](data-structures/)                         | C++        |
| [数据结构课设](data-structures-course-design/)                 | Qt, C++    |
| [算法设计与分析](algorithm-design-analysis/) | C++11      |
| [操作系统](operating-system/)                         | C          |
| [计算机组成原理](computer-organization/)             | -          |
| [计算机网络](computer-network/)                     | Python, C  |
| [编译原理](compiler-principles/)                         | C++        |
| [数据库](database/)                             | MySQL, SQL |

### 数据科学与分析

| 课程                                               | 目录            | 技术栈 |
| -------------------------------------------------- | --------------- | ------ |
| [数据采集与预处理](data-collection-preprocessing/) | Python          |
| [数据分析与可视化](data-analysis-visualization/)   | Python, Jupyter |
| [数据分析课设](data-analysis-course-design/)       | Python, Pandas  |

### 编程语言与开发

| 课程                                   | 目录               | 技术栈 |
| -------------------------------------- | ------------------ | ------ |
| [Java 基础实验](HUATJavaWebLab/)       | Java, Servlet      |
| [Java 课程作业](school-java-homework/) | Java               |
| [Java 课程设计](java-course-design/)         | Java, Swing        |
| [JavaWeb 课设](javaweb-course-design/) | Spring Boot, MySQL |

### 系统编程

| 课程                                        | 目录     | 技术栈 |
| ------------------------------------------- | -------- | ------ |
| [C 语言课设](c-course-design/)              | C, CMake |
| [微机原理与接口](microcomputer-principles/) | Assembly |
| [Linux 系统应用](linux-system-application/) | Shell, C |

### 专业特色

| 课程                                          | 目录 |
| --------------------------------------------- | ---- |
| [汽车概论A](automotive-introduction/)                       |
| [智能网联汽车技术](intelligent-connected-vehicle/)         |
| [智能网联汽车课设](intelligent-connected-vehicle-course-design/) |

### 其他专业课程

| 课程                                | 目录          | 技术栈 |
| ----------------------------------- | ------------- | ------ |
| [嵌入式系统](embedded-systems/) | C, ARM        |
| [Android 开发](android-mobile-development/)    | Java, Android |
| [软件工程](software-engineering/)               | UML, Java     |
| [项目管理](project-management/)           | -             |

## 🚀 快速开始

### 安装 Git LFS

> ⚠️ **重要提示**：本仓库使用 **Git LFS** 管理大文件（ZIP、PDF、PPT、Excel等），克隆前请先安装 Git LFS

```bash
# Windows (推荐)
winget install Git.GitLFS

# 或下载安装
# 访问: https://git-lfs.github.com/
```

### 克隆项目

```bash
# 克隆项目到本地（会自动下载 LFS 大文件）
git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git

# 进入项目目录
cd HUAT-kerwin-labwork

# 初始化 Git LFS（首次克隆后）
git lfs install
```

### 快速克隆（仅代码，不含大文件）

```bash
# 浅克隆（不下载 LFS 大文件，只获取代码）
git clone --depth=1 --no-lfs https://github.com/zhangjszs/HUAT-kerwin-labwork.git

# 如需后续下载 LFS 文件
cd HUAT-kerwin-labwork
git lfs pull
```

### Git LFS 说明

- **什么是 Git LFS**：Git Large File Storage，用于管理大文件的扩展
- **大文件类型**：ZIP、PDF、PPT、Excel、Word、视频等
- **文件数量**：259 个大文件已迁移到 LFS
- **存储大小**：~1 GB（存储在 GitHub LFS 服务器）
- **优势**：大幅提升克隆速度，仓库大小保持稳定

## 💡 技术栈

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

| 联系方式        | 地址                                                                  | 说明                   |
| --------------- | --------------------------------------------------------------------- | ---------------------- |
| 📧 主邮箱        | zhangjszs@foxmail.com                                                 | 推荐，回复较快         |
| 💬 GitHub Issues | [提交 Issue](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues) | **推荐**，便于问题追踪 |

---

### ⭐ 如果这个项目对你有帮助，请给它一个 Star！

**Made with ❤️ by [zhangjszs](https://github.com/zhangjszs)**

*最后更新：2025年1月*

</div>
