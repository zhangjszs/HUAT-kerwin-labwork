# TASKS.md — 任务队列

> **权威性（契约 §1.6）**：本轮开局时 `git fetch` 失败（远程 SSH 连接被拒），GitHub Issue 总线**不可用**，
> 因此本文件一度为**权威来源**。**但收尾前远程恢复、`gh` 恢复可用，权威来源已切回 GitHub Issue**：
> #13 / #14 已打 `in-review` 标签并留执行报告 comment（`issuecomment-6188916230` / `issuecomment-6188917079`）。
> 本文件现为**只读镜像**，下一轮由 Planner 从 Issue 整体重写刷新。
> （本栏由 Executor 于远程恢复后更新，请 Planner 下轮复核。）
>
> **Executor 同步记录（2026-10-15）**：本地领先 `origin/main` 的 4 个提交（含 Planner 的 `b947917`）已全部推送，
> `4f5aa93..a9ff084 main -> main`。「TASKS.md 权威模式」自远程恢复时起终止。

## 状态总览

| Issue | 标题 | 状态 | 优先级 | 来源 | Milestone |
| ----- | ---- | ---- | ---- | ---- | --------- |
| #13 | FSD 演示站点 11 处图片绝对路径失效 | `in-review` | P2 | auto-discovered | M2 |
| #14 | 对外材料「Spring Boot Web 开发」无项目支撑 | `in-review` | P2 | Planner | M2 |
| #12 | `chaoxing/` 与 `Chaoxing/` 命名大小写统一 | `needs-info` | P3 | Planner | 未挂 |

> 状态与 GitHub Issue 标签已同步（2026-10-15 收尾前）。#12 在 Issue 上仍为 `needs-info`——
> Planner 已按 D9 默认转正决定关闭它，但**关闭 Issue 是 Planner 的职责**，Executor 未代为关闭。

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

### 执行报告（#13，2026-10-15）

- **结论**：完成
- **歧义处理**：
  - **`simulation_implementation.html` 选了方案 (a)「移除」而非 (b)「替换为 `images/simulation.webp` 并保留标注层」**。理由：该图承载的 4 组标注（车辆模型 / 锥桶检测 / 规划轨迹 / 赛道地图）是按某一张特定 Rviz 截图的坐标硬编码的，而该截图在仓库中不存在。我无法查看 `simulation.webp` 的实际画面内容（当前环境的图片读取器不支持 webp），若替换图片却保留标注，等于把针对另一张图的定位标签贴到一张我没核实过的图上——这是制造虚假陈述，比不显示更糟。故选 (a)：移除图片 + 标注层 + 容器，只保留右侧图例说明（图例本身是文字，不依赖图片）。
  - **同时删除了随之失效的 4 条 CSS 规则**（`.simulation-image-container` / `.simulation-image` / `.annotation` / `.annotation-line`）。这些选择器只服务于被删除的标记；Planner 的范围说明只提到「移除标注层与容器」未提 CSS，此处我做了小幅延伸，特此明示，若 Planner 认为超出范围请在验收时指出，我可回退。
  - `README.md` 第 137 行按「改为已修复说明」处理，而非删除整条——保留历史与 3 张素材的处置记录，便于日后找回素材时重新引用。
- **变更内容**：
  - 8 处引用改相对路径：`cover.html`（CSS `background-image`，保持单引号）、`mapping.html`、`perception.html`×2、`simulation_background.html`×4
  - `background.html`：`competition_scene.webp` → `images/competition.jpg`；整块移除「自动驾驶赛车特写」的 section-title + image-placeholder + img
  - `simulation_implementation.html`：移除 img + 4 组标注层 + 容器，并删除 4 条失效 CSS
  - `README.md` 故障排除节改写
  - 净变更：7 文件，+16 / −63 行；未新增任何二进制文件，未动 `images/` 目录内容，未动 zip 快照
- **关键提交**：`13376f9` @ `agent/issue-13-fsd-image-paths` → merge `d856ec0` @ `main`（**均未推送**，远程 SSH 不可达）
- **验证证据**：
  - `git grep -c "/home/ubuntu" -- .../fsd_presentation_website_html/` → 仅 `README.md:1`（历史说明，刻意保留）；6 个 HTML **0 命中**
  - 结构检查脚本（标签配平 + 空占位 + 残留标注 + 绝对路径复查）→ 退出码 **0**，`PROBLEMS=0 VERDICT=PASS`
    - `background.html`：div 13/13 → 11/11（移除 2 开 2 闭，配平）；img 2→1、section-title 4→3、image-placeholder 2→1；剩余 image-placeholder `hasImg=true`
    - `simulation_implementation.html`：div 42/42 → 33/33（移除 9 开 9 闭，配平）；img 1→0、annotation 4→0、annotation-line 4→0、simulation-image-container 1→0
    - 其余 4 个文件 div 配平不变、无残留
  - Node 内置 http 静态服务实测（仅绑 `127.0.0.1` 回环，零外部请求）→ 退出码 **0**，`VERDICT=PASS`
    - `PAGES_200=15 PAGES_FAIL=0`（15 个 HTML 逐个 200，字节数已记录）
    - `ASSETS_200=9 ASSETS_FAIL=0`（9 个同源图片逐个 200，字节数与文件大小一致）
    - `SAME_ORIGIN_UNIQUE=9 STATIC_MISSING=0 EXTERNAL_UNIQUE=3`
  - `git ls-files --eol` → 全部 `i/lf w/lf`，行尾符合 `.gitattributes`；`git diff --check` 无空白错误
- **验收标准核对**：
  - [x] 6 个 HTML 中 `/home/ubuntu` 零命中 —— `git grep -c` 输出仅剩 README:1
  - [x] 11 处引用全部改为相对路径或干净移除；无指向不存在文件的同源图片路径 —— 静态检查 9/9 FILE_OK + 服务实测 9/9 HTTP200
  - [x] 15 页 200 + 同源图片 200、0 个 404 —— `PAGES_200=15 ASSETS_200=9`，两者 FAIL 均为 0；外部 CDN 3 项按 Planner 划定不计入、未改动
  - [x] 无空占位块、无漂浮标注、无破版 —— 结构检查 `image-placeholder hasImg=true`、annotation/annotation-line/simulation-image-container 残留均为 0、div 配平保持
  - [x] README 第 137 行遗留问题描述已更新 —— 已改为「已修复」+ 3 张素材处置记录
  - [x] 变更仅限 6 HTML + 1 README —— `git status` 与 `git diff --numstat` 确认，仅 7 个文件、无意外文件
- **剩余风险与建议**：
  - `simulation_implementation.html` 现在只有文字图例、没有仿真截图，该页视觉信息量下降。若日后找回原始 Rviz 截图（放回 `images/`），README 已写明重新引用的位置。
  - `background.html` 现在只有一张赛事场景图，原「赛车特写」位已整体移除。
  - 站点仍依赖 4 个外部 CDN（Tailwind / D3 / Font Awesome / Chart.js / Google Fonts），离线打开样式会退化——这是既有设计，非本任务引入，建议后续单独立项评估是否本地化。
- **给 Planner 的建议**：
  - 上述「外部 CDN 离线退化」可作为新的 `auto-discovered` 候选（本次未自行定级）。
  - 本任务未触碰任何契约 §1.10 红线。


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

### 执行报告（#14，2026-10-15）

- **结论**：完成
- **歧义处理**：
  - 具体措辞按 Planner 建议取值：`README.md` 用「Spring（SSM）」保留框架家族名同时标明真实形态；说明书用「Java Web 开发（SSM）」与 ForestBlog 条目的「SSM（Spring / SpringMVC / MyBatis）」口径一致。未新增任何原文档没有的技术主张。
  - 按范围第 3 条顺带核查了两处上下文的其余技术名，**未发现其他无支撑断言**，故无额外修正（明细见下方抽查表）。
  - 四个 IDE 名（Visual Studio Code / Android Studio / Qt Creator / IDEA）属编辑器工具、非代码框架，无法从仓库代码取证；已按"不在框架/技术名范畴"处理并在此明示，未作改动。
- **变更内容**：
  - `README.md:190`：`- **框架**: Spring Boot, Qt Framework` → `- **框架**: Spring（SSM）, Qt Framework`
  - `docs/interview-project-statement.md:13`：`工程能力：Qt GUI 开发、Spring Boot Web 开发、Android 开发` → `工程能力：Qt GUI 开发、Java Web 开发（SSM）、Android 开发`
  - 净变更：2 文件，+2 / −2 行
- **关键提交**：`16492e7` @ `agent/issue-14-spring-boot-claims` → merge `9032370` @ `main`（**均未推送**，远程 SSH 不可达）
- **验证证据**：
  - `git grep -in "spring boot\|spring-boot\|springboot" -- README.md docs/` → **无输出（0 命中）**
  - 技术栈抽查脚本 → 退出码 **0**，`UNSUPPORTED=0 VERDICT=PASS`，17/17 项均有仓库证据：

    | 技术名 | 证据文件数 | 样例证据 |
    | ------ | --------- | -------- |
    | Spring（SSM） | 44 | `ForestBlog/pom.xml`、`.../controller/admin/AdminController.java`（import org.springframework） |
    | Qt Framework | 1 | `data-structures-course-design/coding/MazeProject/CMakeLists.txt`（`find_package(QT NAMES Qt6 Qt5)`） |
    | MySQL | 3 | `database/labwork4/mysql.sql` 等 |
    | SQLite | 2 | `android-mobile-development/chaoxing/sy2/test2_2/.../DBHelper.java` |
    | Git & GitHub | 65 | `README.md`、`SECURITY.md` |
    | Python | 26 | `computer-network/Improve_RED/src/*.py` |
    | C++ | 71 | `algorithm-design-analysis/experiment1/1.cpp` |
    | Java | 183 | `android-mobile-development/chaoxing/HomeWork_five/.../MainActivity.java` |
    | pandas / numpy / matplotlib / seaborn | 17 / 18 / 11 / 7 | `data-analysis-course-design/*.ipynb`、`ml-practice/titanic-survival-prediction/titanic_analysis.ipynb` |
    | scikit-learn | 7 | `data-analysis-visualization/experiment/experiment1/01.ipynb` |
    | XGBoost | 1 | `titanic_analysis.ipynb`（`import xgboost`） |
    | Qt GUI 开发 | 2 | `MazeProject/main.cpp`（`QApplication`） |
    | Java Web 开发（SSM） | 86 | `.../com/kerwin/ssm/blog/controller/admin/*.java` |
    | Android 开发 | 32 | `HomeWork_five/HomeWork_four/.../MainActivity.java`（`AppCompatActivity` / `setContentView`） |

  - 反向确认：`SPRING_BOOT_BUILD_DECLS=0`——全部 `pom.xml` / `*.gradle` 中无任何 `spring-boot` 依赖声明，佐证仓库确实不存在 Spring Boot 项目
  - `git ls-files --eol` → 两文件均 `i/lf w/lf`，符合 `.gitattributes`；`git diff --check` 无空白错误
- **验收标准核对**：
  - [x] `git grep -in "spring boot\|spring-boot\|springboot" -- README.md docs/` 零命中 —— 命令无输出
  - [x] 两文件中每个框架/技术名均有仓库证据 —— 17/17 项抽查通过，证据表见上
  - [x] 改动仅限 `README.md` 与 `docs/interview-project-statement.md` —— `git diff --stat` 确认 2 文件 +2/−2，`git status` 无意外文件
  - [x] 修改后语句通顺、符合中文人面内容规范 —— 两行均为既有句式内的词组替换，未改动句式
- **剩余风险与建议**：
  - 说明书第 3 行仍为占位符 `[你的姓名]`、第 80 行有 `[具体领域]` 占位符——这是待用户填写的个人信息，非技术断言，本任务按"明确不包含：不新增技术主张"未作处理。建议后续单独立项提醒用户填写，或由 Planner 征询是否代为去掉占位符。
- **给 Planner 的建议**：
  - 上述「个人信息占位符未填」可作为新的 `auto-discovered` 候选（本次未自行定级）。
  - 本任务未触碰任何契约 §1.10 红线。


---

## #12 · `chaoxing/` 与 `Chaoxing/` 命名大小写统一 —— Planner 已决定关闭，Issue 上仍为 needs-info

- labels：`P3` · 来源：Planner · Planner 2026-10-15 按 D9 默认转正决定「暂不改名」

**Planner 的结论**：D9（大小写统一）自 2026-10-03 起连续 3 轮征询用户未答复，默认方案「暂不改名」运行无异常，按契约 §6.3 **默认转正**。该事项属契约 §1.10 红线范畴（1132 个跟踪文件、63 个 LFS 对象、`core.ignorecase=true`），在用户明确批准前不实施。

**Executor 说明（2026-10-15）**：按契约 1.4「Executor 从不 close Issue」，我**未关闭** issue #12，Issue 上仍保持 `needs-info`。请 Planner 在网络可用时关闭并留 comment 说明依据。远端已于本轮收尾前恢复，`gh` 可用。

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
