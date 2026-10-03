# PLAN.md — 开发路线图（规划者维护）

_滚动更新：2026-10-03（第三轮）_

## 当前方向

**展示联动**（M2，D2 默认方向）：让本仓库成为 zhangjszs/huat-showcase 作品集站点的有力素材源，服务求职/申研。第一批 5 个核心项目已完成；本轮重点是 **第二批 5 个 Issue**——修 README 中对外可见的事实性错误、补仓库级导航、收口标签体系。其中 #7 含一处已扩散到根 README 的公开错误（把 SSM 写成 Spring Boot），优先修。

## 里程碑

### M1：仓库收尾与一致性 —— ✅ 已完成（2026-10-02）

**结果**：#4 重命名收尾、#5 索引补齐、Mimosa 门禁解锁（60 高危 → 0，见 D5/D6）均由规划者按用户一次性授权直接完成。门禁剩余 6 medium + 31 low 转 #6 处置，已关闭。

### M2：展示联动（核心项目 README 深化）—— 进行中（进度：0 / 5 已关闭）

GitHub milestone「M2：展示联动（核心项目 README 深化）」下共 **5 个 Issue，全部 open**：#7 / #8 / #9 / #10 / #11（均 ready）。数字由 `gh issue list --milestone` 实际标签状态推算，非记忆。另有 #12 为待用户决策事项，**刻意不挂 milestone、不打 ready**。

**第一批（2026-10-03 完成，交付时未单独开 Issue）**：`yolov7_plate_UI_camera/`、`data-structures-course-design/`、`android-mobile-development/final_course_project/Company/`、`java-course-design/`、`ml-practice/titanic-survival-prediction/` 五个核心项目 README 深化，并修复 `data-collection-preprocessing/README.md` 旧中文路径残留。

**第二批（本轮创建，全部 ready，立项理由均已核实证据）**：

| Issue | 范围 | 立项理由 |
| ---- | ---- | ---- |
| #7 | `javaweb-course-design/` + 根 README | README 写"Spring Boot"，实际是 SSM + JSP + 外部 Tomcat（pom：Spring 4.3.19、war、无 spring-boot 依赖）；**同一误述已扩散到根 README 第 101 行技术栈列**；另含 `javaweb课设` 等失效路径 |
| #8 | `microcomputer-course-design/` | 19 行 README 未解释两个汇编程序（146 / 460 行，8255/8253/AD0809、点歌系统），漏列 819 KB 的 `低版本…（本部）.ppt` |
| #9 | `intelligent-connected-vehicle-course-design/` | 外层 32 行、FSD 网站只有一句话，而实际是 15 个页面 / 142 KB 的站点；内层 README 的运行指引依赖**未被 git 跟踪**的 zip（`.gitignore` 第 48 行忽略、本地存在 1.2 MB、克隆者拿不到） |
| #10 | `docs/agents/` + `.agent/ENV.md` | 仓库内并存两套状态标签词表（契约 1.3 五状态 vs triage 技能词表），违反契约"同一时刻最多一个状态标签" |
| #11 | 根 README + `AGENTS.md` | 同一课程主题存在「课程」与「课程设计」两个平级目录（智能网联 / 微机原理 / Java 三组），根 README 并列展示且无关系说明，读者无法区分课堂资料与课设成果 |

**第三批候选（本批完成后按需立项）**：`algorithm-design-analysis/`、`operating-system/`、`database/` 等其余课程目录的 README 深化；`compiler-principles/`、`computer-network/` 与 `software-engineering/` 的课设/资料交叉引用。

## 执行队列（给 Executor 的建议顺序）

1. **#7** `docs: 深化 javaweb-course-design README（M2 第二批）`（P2）—— 修公开事实性错误（目录 + 根 README 两处）
2. **#11** `docs: 根 README 区分「课程」与「课程设计」并列目录`（P2）—— 纯导航，风险最低，先吃掉
3. **#8** `docs: 深化 microcomputer-course-design README（M2 第二批）`（P2）
4. **#9** `docs: 深化智能网联课设外层 README 并修复 FSD 站点运行指引（M2 第二批）`（P2）
5. **#10** `chore: 统一 Issue 标签体系到协作契约 1.3（五状态标签）`（P2）—— 独立于 README 工作，可随时插入

排序理由：#7 修对外可见的错误；#11 体量最小且与内容深化互不冲突；#9 体量最大放后；#10 与前三者完全独立。若 #7 与 #11 都要动根 README，按 #7 → #11 顺序执行（#11 只加结构性说明，冲突面小）。

## 已知阻塞与依赖

- **无阻塞执行**：#7 / #8 / #9 / #10 / #11 全部通过 ready 门禁，无相互依赖。
- #12（`needs-info`，P3）：超星资料目录命名大小写统一（`chaoxing` vs `Chaoxing`，7 个目录 / 1132 个跟踪文件 / 63 个 LFS 对象，本机 `core.ignorecase=true` 使仅大小写改名需两步操作）。属契约 §1.8 红线范畴，**等用户决策**；批准前不实施。已备三方案与风险（见 issue #12 正文）。
- 协作状态：`.agent/LOCK` 不存在、`STATE.md` 显示"正在处理的 issue：无"→ **Executor 尚未开始本轮**，执行队列全部待领取。
- **待观察（不新增 Issue，避免与 #9 范围重叠）**：`.gitattributes` 对 `fsd_presentation_website_html.zip` 声明了 LFS filter，而该文件被 `.gitignore` 第 48 行忽略且未被跟踪——声明无效，克隆者永远拿不到。若后续确认需要，再单独立项处理 `.gitattributes` 卫生。

## 下一阶段

第三批 README 深化（其余课程目录）→ showcase 站点与本仓库双向链接（README 徽章 ↔ 站点项目页）→ 按需引入目录级构建/检查脚本（不建仓库级 CI，见 D3）。

## 已放弃的方向

- 仓库级 CI（GitHub Actions）：D3 默认"暂不引入"，2026-10-02 起未再评估；个别目录有需要时按目录单独加。

## 给 Executor 的指令

按执行队列顺序做 #7 → #11 → #8 → #9 → #10；`docs/agents/` 与 `.agent/ENV.md` 的标签口径只在 #10 内改动，#12 在用户决策前不要动。
