# PLAN.md — 开发路线图（规划者维护）

_滚动更新：2026-10-02（第二轮）_

## 当前方向

**展示联动**（M2）：让本仓库成为 zhangjszs/huat-showcase 作品集站点的有力素材源，服务求职/申研（D2 默认方向）。

## 里程碑

### M1：仓库收尾与一致性 —— ✅ 已完成（2026-10-02）

**结果**：#4 重命名收尾、#5 索引补齐、Mimosa 门禁解锁（60 高危 → 0，见 D5/D6）均由规划者按用户一次性授权直接完成。门禁剩余 6 medium + 31 low 转 #6 评估。

### M2：展示联动（核心项目 README 深化）—— 第一批完成（2026-10-03）

**第一批交付**（5 个核心项目）：

| 项目 | 动作 |
| ---- | ---- |
| `intelligent-connected-vehicle-course-design/yolov7_plate_UI_camera/` | 补两阶段流水线架构图、设计要点、真实克隆路径、徽章、作品集脚注 |
| `data-structures-course-design/` | 外层 README 重写：修正旧路径旧描述，对齐 MazeProject（5 算法）现状，算法对比表，链接内层文档 |
| `android-mobile-development/final_course_project/Company/` | **新建** README：双角色功能矩阵、MVVM+Room 架构、构建方式、重构计划文档引用 |
| `java-course-design/` | 重写：四游戏对比表（从 zip 提炼类结构与技术点）、增量快照工作法说明、修正旧路径 |
| `ml-practice/titanic-survival-prediction/` | 徽章、流水线一览、作品集脚注 |

另修复 `data-collection-preprocessing/README.md` 旧中文路径残留。所有 README 统一追加 huat-showcase 作品集脚注。

**第二批候选**（后续会话按需建 issue）：智能网联课设外层 README 的 FSD 网站部分、javaweb-course-design（ForestBlog）、microcomputer-course-design（汇编课设）。

## 远景（粗粒度）

- 2025 及以后学期课程作业持续归档入库（D1：此前请求已完成一轮）。
- showcase 站点与本仓库双向链接（README 徽章 ↔ 站点项目页）。
- 必要时按目录单独引入构建/检查脚本（不建仓库级 CI，见 D4）。

## 已放弃的方向

（暂无）
