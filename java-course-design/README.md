# Java 课程设计：Swing 小游戏合集

![Java](https://img.shields.io/badge/Java-8+-orange.svg)
![GUI](https://img.shields.io/badge/GUI-Swing-green.svg)

Java 课程设计作品：四个基于 **Swing + AWT** 的桌面小游戏。每个游戏独立成档，从窗口创建、主循环、键盘交互到碰撞检测与音效，完整覆盖 Java 图形化编程的核心知识点。

## 游戏一览

| 游戏 | 核心类（包名） | 技术要点 |
| ---- | -------------- | -------- |
| **大鱼吃小鱼** | `com.sxt.GameWin / GameUtils / Bg` | 分步骤增量开发快照（01 创建窗口 → 02 背景 → 03 启动封面 → 04 点击事件 …），完整留档了课设迭代过程 |
| **贪食蛇** | `com.linstack.testsnake.GamePanel / LaunchGame / Images` | 键盘方向控制、定时刷新主循环、图片资源集中管理 |
| **吞食鱼** | `com.rt.GameWin / MyFish / Enamy / Bg / GameUtils` | 多实体（敌方鱼群）移动与碰撞判定、难度随体格成长 |
| **超级玛丽** | `Mario / BackGround / Enemy / Obstacle / Music / StaticValue / MyFrame` | 横版关卡地图（`StaticValue` 集中管理瓦片与关卡数据）、双缓冲绘制、角色物理（跳跃/重力）、敌人 AI 与障碍碰撞、背景音乐播放 |

## 环境要求

- JDK 8+
- 任意 IDE（IntelliJ IDEA / Eclipse）或直接 `javac` 命令行

## 运行方式

源码以 zip 分档保存（含图片/音效资源），解压后即可编译运行，以贪食蛇为例：

```bash
git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
cd HUAT-kerwin-labwork/java-course-design
unzip 项目2贪食蛇.zip
cd 源码/src
javac com/linstack/testsnake/*.java
java com.linstack.testsnake.LaunchGame
```

其余游戏同理：解压对应 zip，定位 `src` 目录后 `javac` 编译、`java` 运行主类（超级玛丽主类为 `MyFrame`，吞食鱼/大鱼吃小鱼为 `GameWin`）。

## 设计说明

- **增量快照工作法**：大鱼吃小鱼按功能步骤保留每一阶段的可运行版本，体现了从最小可运行程序逐步生长的课设过程，适合作为教学参考
- **资源与逻辑分离**：各游戏均将图片/音效路径收敛到工具类（`GameUtils` / `Images` / `StaticValue`），避免魔法字符串散落
- **面向对象建模**：角色（Mario/MyFish）、敌人（Enamy/Enemy）、背景（Bg/BackGround）各自封装，游戏窗口只负责调度

---

> 📁 本项目是 [HUAT-kerwin-labwork](https://github.com/zhangjszs/HUAT-kerwin-labwork) 课程作品集的一部分，项目展示页见 [huat-showcase](https://github.com/zhangjszs/huat-showcase)。
