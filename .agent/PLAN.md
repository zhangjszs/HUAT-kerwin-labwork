# PLAN.md — 开发路线图（规划者维护）

_滚动更新：2026-10-15（第五轮）_

## 当前方向

**展示联动**（M2，**D2 已由用户确认**：2026-10-07 选择「继续」）：让本仓库成为 zhangjszs/huat-showcase 作品集站点的有力素材源，服务求职/申研。

**本轮最重要的发现不是任务，而是流程断档**：自 2026-10-07 第四轮规划至今 **8 天无任何提交**，且 `.agent/` 从未建过 `DIARY/` 目录、从未有过 `TASKS.md`——日报链路与任务台账双缺，本轮已补建（见下）。当前队列仍是两个"对外材料可信度"修复（#13 站点图片失效、#14 面试材料无支撑的技能主张），均 ready 且本轮已逐行复核取证。

## 里程碑

### M1：仓库收尾与一致性 —— ✅ 已完成（2026-10-02）

**结果**：#4 重命名收尾、#5 索引补齐、Mimosa 门禁解锁（60 高危 → 0，见 D5/D6）均由规划者按用户一次性授权直接完成。门禁剩余 6 medium + 31 low 转 #6 处置，已关闭。

### M2：展示联动（核心项目 README 深化）—— 进行中（进度：6 / 7 已关闭）

数字来源：GitHub milestone 实测值（2026-10-07：`open_issues=2`、`closed_issues=5`、共 7 个），叠加本轮 #12 关闭 → 本地台账 **6 已关闭 / 1 待做**。**本轮远程不可达，无法用 `gh` 复核，网络恢复后由下一轮规划者重新实测。**

**第一批（2026-10-03，交付时未单独开 Issue）**：5 个核心项目 README 深化（`yolov7_plate_UI_camera/`、`data-structures-course-design/`、`android-mobile-development/final_course_project/Company/`、`java-course-design/`、`ml-practice/titanic-survival-prediction/`），并修复 `data-collection-preprocessing/README.md` 旧中文路径残留。

**第二批（2026-10-03 执行 · 2026-10-07 验收，全部通过并关闭）**：

| Issue | 交付 | 验收要点 |
| ---- | ---- | ---- |
| #7 | `javaweb-course-design/README.md` 重写 + 根 README 技术栈列 | 技术栈纠正为 SSM 4.3.19 / JSP / war / Java 8；`HUATJavaWebLab`、`javaweb课设` 失效路径清零；另修正 `docs/interview-project-statement.md` 第 54 行的 Thymeleaf 误述 |
| #8 | `microcomputer-course-design/README.md` 22 → 92 行 | 7/7 文件覆盖（含 819 KB ppt）；两程序技术要点附源码行号；`Countdown.asm` 实际 189 行（我立项时误写 146，已按实际纠正） |
| #9 | 智能网联外层 README + 内层 FSD 运行指引 | 15 页分组一览；内层去除 zip 依赖与占位符。**Executor 纠正了我"zip 未被跟踪"的错误前提**（zip 确被跟踪且走 LFS，位于目录上一级） |
| #10 | 标签体系收口 | 契约 1.3 五状态为唯一权威；旧三标签已从 GitHub 删除；`triage-labels.md`、`issue-tracker.md`、`.agent/ENV.md` 口径统一 |
| #11 | 根 README 目录性质标注 | 11 个条目加「（课堂实验）/（课程资料）/（课程设计）」括注（覆盖 5 组并列目录）；`AGENTS.md`、`CONTRIBUTING.md` 补索引规范 |

**#12 —— 2026-10-15 按 D9 默认转正关闭**（详见 DECISIONS.md D9）：`chaoxing/` 与 `Chaoxing/` 大小写统一连续 3 轮征询无答复，默认「暂不改名」转正。红线事项，不实施。

**第三批（当前队列，均 ready）**：

| Issue | 类型 | 本轮复核结论 |
| ---- | ---- | ---- |
| #13 | `auto-discovered`（P2） | **未开工**（8 天无提交）。6 个 HTML 共 11 处 `/home/ubuntu/...` 绝对路径；8 处引用的图片在 `images/` 有实存文件，3 处（`competition_scene.webp`、`race_car_close_up.webp`、`simulation_implementation.webp`）在仓库与 zip 快照中均不存在，无法恢复。另发现 `images/competition.jpg`（61 KB，LFS）是被跟踪但全站零引用的孤立资产，可作替代素材 |
| #14 | Planner（P2） | **未开工**。`git grep` 全仓（排除 vendored ForestBlog）仅 2 处用户可见命中：`README.md:190`、`docs/interview-project-statement.md:13`。#7 只修了根 README L101 与说明书 ForestBlog 条目，**漏了这两处总述性断言**——#14 范围已据此扩大 |

## 执行队列（给 Executor 的建议顺序）

1. **#13** `fix: FSD 演示站点 11 处图片绝对路径失效（含 3 张素材不可找回）`（P2）—— 修一个**能看见的坏页面**，作品集访客第一眼就是它
2. **#14** `docs: 根 README 与项目说明书中「Spring Boot」无项目支撑，与仓库事实不符`（P2）—— 面试材料可信度，与刚验收的 #7 属同一类错误

排序理由：#13 是用户可见的呈现缺陷（图片全挂），优先于文字口径；#14 成本极低紧随其后。两者触及文件无重叠，可串行执行。

两个任务的完整背景、逐行取证、实现范围、明确不包含与验收标准均已写入 `.agent/TASKS.md`，可直接照做。

## 已知阻塞与依赖

- **无任务阻塞**：#13、#14 均通过 ready 门禁、无依赖，可立即领取。
- **环境阻塞（本轮新增）**：`git fetch` / `git pull --rebase` / `git push` 均失败——远程 SSH 连接被拒（`Connection closed by 20.205.243.166 port 22`）。按契约 §1.6 降级为 **TASKS.md 权威模式**：Executor 本轮直接在 `TASKS.md` 改状态、不发 Issue comment；网络恢复后由规划者下一轮回灌。这不是"跳过"，是如实降级。
- **未推送提交**：本轮规划提交（subject `chore(agent): 第五轮规划——补建 TASKS.md 与 DIARY、#13/#14 逐行复核、D9 默认转正`，hash 以 `git log --oneline -1` 为准）**本地已提交、推送失败**（契约 §1.9 允许的重试一次已执行，仍失败）。下一轮网络恢复后第一件事就是 `git pull --rebase && git push`，并把本地 TASKS.md 状态回灌到 GitHub Issue。
- **流程缺口（本轮新增，需 Executor 优先补）**：`.agent/DIARY/` 目录此前不存在，`TASKS.md` 此前不存在。契约 §1.8 规定日报是规划者次日验收的唯一输入——缺了它，跨天链路从 2026-10-03 起就是断的。目录已由本轮按 §7 补建，**Executor 本轮收尾必须写入 `.agent/DIARY/2026-10-15.md`**（内容含：本轮做了什么、卡在哪、验证情况、明天第一步）。
- 协作状态：`.agent/LOCK` 不存在 → 无串行冲突。
- **已澄清（勿再当问题处理）**：`fsd_presentation_website_html.zip` 确被 git 跟踪且由 LFS 管理，路径为 `intelligent-connected-vehicle-course-design/fsd_presentation_website_html.zip`（**目录上一级**）。上一轮 PLAN 中"待观察：LFS 声明无效、克隆者拿不到"一条基于我的错误路径，**已作废**。

## 下一阶段

第三批完成后：其余课程目录 README 深化（本轮已核查：28 个课程目录**全部有 README**、根 README 索引 28 条链接**全部可达**、无重复或明显残缺条目，暂无新的可立项缺陷）→ showcase 站点与本仓库双向链接（README 徽章 ↔ 站点项目页）→ 按需引入目录级构建/检查脚本（不建仓库级 CI，见 D3）。

## 已放弃的方向

- 仓库级 CI（GitHub Actions）：D3 默认"暂不引入"，2026-10-02 起未再评估；个别目录有需要时按目录单独加。

## 给 Executor 的指令

按队列顺序做 #13 → #14，任务细节以 `.agent/TASKS.md` 为准（Issue 本轮不可用）。#13 中 3 张缺失图片的处置方式已在 TASKS.md 逐点给定默认方案，照做即可；若改方案，在执行报告里写明理由。**本轮收尾务必写 `.agent/DIARY/2026-10-15.md`**。
