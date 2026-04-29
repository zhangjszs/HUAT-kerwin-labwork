# HUAT Java Homework

本项目包含 Java 课程作业集合。

## 环境要求

- **操作系统**: Windows / Linux / macOS
- **JDK**: Java 8+
- **IDE**: IntelliJ IDEA（推荐）

## 目录结构

- `src/`：源代码，按作业模块分包
- `out/`：编译输出（IntelliJ IDEA 自动生成，无需提交）

## 运行步骤

### 使用 IDE 运行（推荐）

1. 克隆仓库：
   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/school-java-homework
   ```

2. 在 IntelliJ IDEA 中打开项目，选中对应的类文件后点击运行。

### 使用命令行运行

以 `labhomework3.MainClass` 为例：

```bash
# 编译
javac -d out src/labhomework3/MainClass.java

# 运行
java -cp out labhomework3.MainClass
```

## 模块说明

| 包名 | 内容 |
|------|------|
| `Homework1JavaProgrammingFundamentals` | 基础语法：计算器、问候程序 |
| `Homework2JavaProgrammingExercise` | 面向对象基础：圆/矩形计算、成绩统计 |
| `Homework3JavaProgrammingSwing` | Swing GUI：数字键盘、文本显示器 |
| `labhomework3` | 实验3：继承多态、接口、学生信息管理系统（DAO 模式） |
| `Labhomework4` | 实验4：艺术字体设置器 |
| `Labhomework5` | 实验5：多线程动画、线程控制 |
| `Labhomework6` | 实验6：基于 Socket 的聊天程序（客户端/服务器） |

## 许可

MIT License
