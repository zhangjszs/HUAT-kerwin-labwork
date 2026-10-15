# TASKS.md — 任务队列

> **权威性（契约 §1.6）**：本轮 `git fetch` 失败（远程不可达，SSH 连接被拒），GitHub Issue 总线**不可用**，
> 因此本文件为**权威来源**，Executor 直接在此改状态，本轮不提 Issue。网络恢复后由 Planner 下一轮
> 从 Issue 整体重写刷新本文件，并把本轮在本地产生的状态变更回灌到 Issue。
>
> **本轮刷新**：2026-10-15（第五轮规划）· 规划者整体重写，非增量补丁。

## 状态总览

| Issue | 标题 | 状态 | 优先级 | 来源 | Milestone |
| ----- | ---- | ---- | ---- | ---- | --------- |
| #13 | FSD 演示站点 11 处图片绝对路径失效 | `ready` | P2 | auto-discovered | M2 |
| #14 | 对外材料「Spring Boot Web 开发」无项目支撑 | `ready` | P2 | Planner | M2 |
| #12 | `chaoxing/` 与 `Chaoxing/` 命名大小写统一 | `done` | P3 | Planner | 未挂 |

## 今日队列（给 Executor 的建议顺序，2026-10-15）

1. **#13**（P2）— 修一个**访客第一眼就看见的坏页面**：克隆后站点 11 处图片全部 404。
2. **#14**（P2）— 面试材料可信度，与已验收的 #7 属同一类错误，成本极低。

排序理由：#13 是用户可见的呈现缺陷，优先于文字口径；#14 紧随其后。两者触及文件无重叠，可串行执行。

---

## #13 · FSD 演示站点 11 处图片绝对路径失效（含 3 张素材不可找回）

- labels：`P2` `ready` · milestone：M2 · 来源：`auto-discovered`（2026-10-03 Executor 于 #9 中上报）

### 背景

`intelligent-connected-vehicle-course-design/fsd_presentation_website_html/` 是智能网联汽车课程设计的演示站点（15 页静态 HTML）。Executor 在 #9 中用本地静态服务器实测发现：站点内图片引用使用原开发机绝对路径 `/home/ubuntu/fsd_presentation/images/...`，克隆后全部 404。

### 当前问题（2026-10-15 规划者复核，逐行取证）

6 个 HTML 文件共 **11 处**引用：

| 文件 | 行 | 引用图片 | 仓库内是否有该图 |
| ---- | --- | -------- | ---------------- |
| `background.html` | 162 | `competition_scene.webp` | **否** |
| `background.html` | 169 | `race_car_close_up.webp` | **否** |
| `cover.html` | 33 | `cover.webp` | 是（231 836 B） |
| `mapping.html` | 214 | `slam.gif` | 是（496 271 B） |
| `perception.html` | 164 | `lidar.jpg` | 是（102 762 B） |
| `perception.html` | 176 | `yolo.jpg` | 是（71 900 B） |
| `simulation_background.html` | 198 | `ros_logo.png` | 是（41 307 B） |
| `simulation_background.html` | 206 | `gazebo_logo.png` | 是（25 957 B） |
| `simulation_background.html` | 214 | `rviz_logo.png` | 是（50 739 B） |
| `simulation_background.html` | 245 | `simulation.webp` | 是（165 620 B） |
| `simulation_implementation.html` | 216 | `simulation_implementation.webp` | **否** |

缺失的 3 张图已核实**在仓库与 `fsd_presentation_website_html.zip` 快照中均不存在**：zip 内 `images/` 恰好是同样 9 个文件，缺的正是这 3 个，故无法从仓库恢复。

另发现：`images/competition.jpg`（61 077 B，LFS）是**被 git 跟踪但全站无任何引用的孤立资产**，可作为「赛事场景预览」的替代素材。

### 目标结果

克隆仓库后直接打开站点任一页面，所有图片均能加载；不存在 404 图片请求，也不存在因移除图片而留下的空占位块或漂浮标注层。

### 实现范围

1. **8 处有实存图片的引用**：绝对路径改为相对路径 `images/<文件名>`。`cover.html` 是 CSS `background-image: url('...')`，同样改为 `url('images/cover.webp')`，保持原有单引号风格。
2. **3 处缺失图片**，逐点按下列默认处置（Executor 若采用不同方案，须在执行报告中写明理由）：
   - `competition_scene.webp` → 用 `images/competition.jpg` 替代（同为赛事场景素材，且该图当前无任何引用）。
   - `race_car_close_up.webp`（`background.html:169`）→ 移除该 `<img>`，并**连同外层 `.image-placeholder` 容器与上方 `.section-title`「自动驾驶赛车特写」一并移除**，不得留下 200px 灰色空块。
   - `simulation_implementation.webp`（`simulation_implementation.html:216`）→ 该图承载一组绝对定位标注层（车辆模型 / 锥桶检测 / 规划轨迹 / 赛道地图，见 219–229 行）。**不得只删 `<img>`**：容器 `.simulation-image-container` 为 `flex:3; position:relative`，删图后容器高度归零，标注线与标签会漂浮到右侧文字列上方造成破版。请二选一，并在报告中说明选了哪个：
     - (a) 移除图片的同时移除标注层与容器，让左侧文字列占满 `.simulation-content` 宽度；
     - (b) 改用语义相近的实存图（如 `images/simulation.webp`，仿真平台效果预览）并保留标注层。
3. 同步修正 `fsd_presentation_website_html/README.md` 第 137 行：该行当前声称绝对路径是"站点当前的遗留问题"，修复后应改为已修复的说明或删除该条。
4. 不新增二进制文件；不改 `images/` 目录内容（除引用指向变化外）。

### 明确不包含

- 不改站点的视觉设计、配色、文案、页面结构（移除失效图片所必需的最小改动除外）。
- 不重写或重构 HTML/CSS。
- 不动 `fsd_presentation_website_html.zip` 历史快照。
- 不给站点加构建系统、包管理器或 CI。

### 依赖

- 无。

### 验收标准

- [ ] `git grep "/home/ubuntu" -- intelligent-connected-vehicle-course-design/fsd_presentation_website_html/` 在 6 个 HTML 中**零命中**。
- [ ] 原 11 处引用全部改为相对路径或被干净移除；站点内不存在指向不存在文件的**同源**图片路径。
- [ ] 本地静态服务器实测：15 个 HTML 页面逐个 GET 返回 200，且页面引用的每个**同源**（`images/...`）路径 GET 返回 200，**0 个 404**；命令与输出写入执行报告。
  - 注意：站点每页已预置 4 个外部 CDN 引用（jsdelivr / d3js.org / cdnjs.cloudflare.com / fonts.googleapis.com），**不属于本任务范围，不要改动、也不要因它们离线而判定失败**。验收只统计同源图片。
- [ ] `background.html`、`simulation_implementation.html` 中因移除图片而调整的结构，无空占位块、无漂浮标注、无破版（附截图或 DOM 结构说明）。
- [ ] `README.md` 第 137 行的遗留问题描述已同步更新，不再声称存在未修复的绝对路径问题。
- [ ] 变更仅限 6 个 HTML + 1 个 README；`git status` 无意外文件。

### 测试要求

- 静态服务实测（Node 或 `python -m http.server` 均可），逐页 + 逐**同源**图片记录状态码。
- 不得引入新的绝对路径，不得新增外部 CDN 依赖（既有 4 个 CDN 保持原样）。

### 执行报告（Executor 填写，纯本地模式按契约 §1.7 写在此处）

_未开始_

---

## #14 · 对外材料声称「Spring Boot Web 开发」但仓库无任何 Spring Boot 项目

- labels：`P2` `ready` · milestone：M2 · 来源：Planner

### 背景

面试 / 求职材料必须经得起证伪。#7 已修正根 README 课程表（L101）与 `docs/interview-project-statement.md` 中 ForestBlog 条目的技术栈，但漏了两处**总述性断言**——它们比具体项目条目更显眼，也更容易被当场证伪。

### 当前问题（2026-10-15 规划者复核）

`git grep -in "spring boot\|spring-boot\|springboot"` 全仓（排除 vendored ForestBlog 与 `.agent/`）仅 2 处用户可见命中：

- `README.md:190` — `- **框架**: Spring Boot, Qt Framework`
- `docs/interview-project-statement.md:13` — `- **工程能力：** Qt GUI 开发、Spring Boot Web 开发、Android 开发`

而仓库唯一的 JavaWeb 项目是 `javaweb-course-design/ForestBlog-master/`：包名为 `com.kerwin.ssm.blog`，且 `git grep -ril "spring-boot\|springboot"` 在所有 `*.java` `*.xml` `*.properties` `*.yml` `*.gradle` `*.kts` 中**零命中**。它是 SSM（Spring 4.3.19 / SpringMVC / MyBatis），不是 Spring Boot。对外材料写 Spring Boot 属于无支撑的虚假主张。

### 目标结果

两处材料的技术栈陈述与仓库事实一致，不再出现无项目支撑的框架名。

### 实现范围

1. `README.md:190`：把 `Spring Boot` 改为与仓库事实一致的表述（建议 `Spring（SSM）`）；`Qt Framework` 保留。
2. `docs/interview-project-statement.md:13`：把 `Spring Boot Web 开发` 改为有项目支撑的表述（建议 `Java Web 开发（SSM）`）。
3. 顺带核查同两处上下文是否还有其他无支撑的框架 / 技术断言（只查这两处上下文，不做全仓审计）；发现则一并修正并在报告中列出。

### 明确不包含

- **不新增技术主张**——只做"删除 / 改正不实陈述"，不借机给材料加码。
- 不改 `docs/interview-project-statement.md` 中具体项目章节的技术栈（#7 已修正 ForestBlog 条目）。
- 不动 vendored ForestBlog 源码。

### 依赖

- 无。#7 已 done，无需其重新开启。

### 验收标准

- [ ] `git grep -in "spring boot\|spring-boot\|springboot" -- README.md docs/` **零命中**。
- [ ] 上述两个文件中出现的每个框架 / 技术名，均能在仓库中找到对应项目或代码证据；抽查表写入执行报告。
- [ ] 改动仅限 `README.md` 与 `docs/interview-project-statement.md`；`git status` 无意外文件。
- [ ] 修改后语句通顺，中文表述符合仓库人面内容规范。

### 测试要求

- 检索取证：`git grep` 修改前后对比输出写入执行报告。

### 执行报告（Executor 填写，纯本地模式按契约 §1.7 写在此处）

_未开始_

---

## #12 · `chaoxing/` 与 `Chaoxing/` 命名大小写统一 —— 已按 D9 默认转正关闭

- labels：`P3` · 来源：Planner · 2026-10-15 转 `done`

**关闭理由**：D9（大小写统一）自 2026-10-03 起连续 3 轮征询用户未答复，默认方案「暂不改名」运行无异常，按契约 §6.3 **默认转正**。该事项属契约 §1.10 红线范畴（1132 个跟踪文件、63 个 LFS 对象、`core.ignorecase=true`），在用户明确批准前不实施。

**GitHub 待办**：网络恢复后由 Planner 关闭 issue #12 并留 comment 说明依据（本轮远程不可达，无法操作）。

---

## 已关闭 / 已完成（历史，不再行动）

| Issue | 交付 | 验收结论 |
| ----- | ---- | -------- |
| #7 | `javaweb-course-design/README.md` 重写 + 根 README 技术栈列 | 2026-10-07 通过并关闭 |
| #8 | `microcomputer-course-design/README.md` 22 → 92 行 | 2026-10-07 通过并关闭 |
| #9 | 智能网联外层 README + 内层 FSD 运行指引 | 2026-10-07 通过并关闭 |
| #10 | 标签体系收口（契约 1.3 为唯一权威） | 2026-10-07 通过并关闭 |
| #11 | 根 README 目录性质标注 | 2026-10-07 通过并关闭 |
| #4 #5 #6 | M1 仓库收尾与一致性 | 2026-10-03 由规划者按用户一次性授权完成 |
| M2 第一批 | 5 个核心项目 README 深化 | 2026-10-03 完成 |
