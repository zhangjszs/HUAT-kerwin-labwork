# PLAN.md — 开发路线图（规划者维护）

_滚动更新：2026-10-07（第四轮）_

## 当前方向

**展示联动**（M2，**D2 已由用户确认**：2026-10-07 选择「继续」）：让本仓库成为 zhangjszs/huat-showcase 作品集站点的有力素材源，服务求职/申研。第二批 5 个 Issue 已全部验收通过；当前队列是**两个"对外材料可信度"修复**（#13 站点图片失效、#14 面试材料无支撑的技能主张），均已完成立项并 ready。

## 里程碑

### M1：仓库收尾与一致性 —— ✅ 已完成（2026-10-02）

**结果**：#4 重命名收尾、#5 索引补齐、Mimosa 门禁解锁（60 高危 → 0，见 D5/D6）均由规划者按用户一次性授权直接完成。门禁剩余 6 medium + 31 low 转 #6 处置，已关闭。

### M2：展示联动（核心项目 README 深化）—— 进行中（进度：5 / 7 已关闭）

数字由 `gh api repos/.../milestones` 实测：`open_issues=2`、`closed_issues=5`，共 7 个。

**第一批（2026-10-03，交付时未单独开 Issue）**：5 个核心项目 README 深化（`yolov7_plate_UI_camera/`、`data-structures-course-design/`、`android-mobile-development/final_course_project/Company/`、`java-course-design/`、`ml-practice/titanic-survival-prediction/`），并修复 `data-collection-preprocessing/README.md` 旧中文路径残留。

**第二批（2026-10-03 执行 · 2026-10-07 验收，全部通过并关闭）**：

| Issue | 交付 | 验收要点 |
| ---- | ---- | ---- |
| #7 | `javaweb-course-design/README.md` 重写 + 根 README 技术栈列 | 技术栈纠正为 SSM 4.3.19 / JSP / war / Java 8；`HUATJavaWebLab`、`javaweb课设` 失效路径清零；另修正 `docs/interview-project-statement.md` 第 54 行的 Thymeleaf 误述 |
| #8 | `microcomputer-course-design/README.md` 22 → 92 行 | 7/7 文件覆盖（含 819 KB ppt）；两程序技术要点附源码行号；`Countdown.asm` 实际 189 行（我立项时误写 146，已按实际纠正） |
| #9 | 智能网联外层 README + 内层 FSD 运行指引 | 15 页分组一览；内层去除 zip 依赖与占位符。**Executor 纠正了我"zip 未被跟踪"的错误前提**（zip 确被跟踪且走 LFS，位于目录上一级） |
| #10 | 标签体系收口 | 契约 1.3 五状态为唯一权威；旧三标签已从 GitHub 删除；`triage-labels.md`、`issue-tracker.md`、`.agent/ENV.md` 口径统一 |
| #11 | 根 README 目录性质标注 | 11 个条目加「（课堂实验）/（课程资料）/（课程设计）」括注（覆盖 5 组并列目录）；`AGENTS.md`、`CONTRIBUTING.md` 补索引规范 |

**第三批（当前队列，均 ready）**：

| Issue | 类型 | 内容 |
| ---- | ---- | ---- |
| #13 | `auto-discovered`（P2） | FSD 演示站点 11 处图片引用为开发机绝对路径 `/home/ubuntu/...`，克隆后全部 404；其中 3 张素材经我核实**在仓库与 tracked zip 中均不存在**，只能替换或移除 |
| #14 | Planner（P2） | `docs/interview-project-statement.md` 第 13 行"Spring Boot Web 开发"无任何仓库项目支撑（唯一 JavaWeb 项目是 SSM），对外材料可被证伪 |

## 执行队列（给 Executor 的建议顺序）

1. **#13** `fix: FSD 演示站点 11 处图片绝对路径失效（含 3 张素材不可找回）`（P2）—— 修一个**能看见的坏页面**，作品集访客第一眼就是它
2. **#14** `docs: 项目说明书中「Spring Boot Web 开发」无项目支撑，与仓库事实不符`（P2）—— 面试材料可信度，与刚验收的 #7 属同一类错误

排序理由：#13 是用户可见的呈现缺陷（图片全挂），优先于文字口径；#14 成本极低紧随其后。两者触及文件无重叠，可串行执行。

## 已知阻塞与依赖

- **无阻塞**：#13、#14 均通过 ready 门禁、无依赖，可立即领取。
- **#12（`needs-info`，P3，未挂 milestone）**：`chaoxing/` 与 `Chaoxing/` 命名统一（7 个目录 / 1132 个跟踪文件 / 63 个 LFS 对象，`core.ignorecase=true`）。属契约 §1.8 红线，**等用户决策**；本轮再次征询（D9 第 2 轮）未获答复，按默认**不改名**继续等待。
- 协作状态：`.agent/LOCK` 不存在 → Executor 本轮未在运行，队列全部待领取。
- **已澄清（勿再当问题处理）**：`fsd_presentation_website_html.zip` 确被 git 跟踪且由 LFS 管理，路径为 `intelligent-connected-vehicle-course-design/fsd_presentation_website_html.zip`（**目录上一级**）。上一轮 PLAN 中"待观察：LFS 声明无效、克隆者拿不到"一条基于我的错误路径，**已作废**。

## 下一阶段

第三批完成后：其余课程目录 README 深化（`operating-system/`、`database/`、`algorithm-design-analysis/` 等）→ showcase 站点与本仓库双向链接（README 徽章 ↔ 站点项目页）→ 按需引入目录级构建/检查脚本（不建仓库级 CI，见 D3）。

## 已放弃的方向

- 仓库级 CI（GitHub Actions）：D3 默认"暂不引入"，2026-10-02 起未再评估；个别目录有需要时按目录单独加。

## 给 Executor 的指令

按队列顺序做 #13 → #14。#13 的替代方案（哪张实存图替代哪张缺失图、标注层如何处置）已在该 Issue 正文逐条给定，照做即可；#12 在用户决策前不要动。
