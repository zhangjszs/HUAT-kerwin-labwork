# HANDOFF.md — 规划者 → 执行 Agent 交接

_最近更新：2026-10-02（规划者，第二轮）_

## 当前状态：门禁已解锁

用户直接授权规划者修复全部安全发现（D6），高危已清零、门禁放行，#4/#5 已由规划者一并提交关闭。**M1 完成。**

## 执行 Agent 的下一批工作

1. **M2 第二批（待用户点名后建 issue）**：智能网联课设外层 FSD 网站部分、javaweb-course-design（ForestBlog）、microcomputer-course-design。第一批 5 项目 README 已由规划者完成（见 PLAN.md M2），勿重复改写。
2. **Mimosa 遗留**：4 medium（BaseDao 环境变量默认值、3 处跨文件污点误报）+ 29 low（ML 随机数）均为已记录的合理留存，除非规则升级否则勿再动。

## 历史背景（已解决，勿重复处理）

- ~~#4 重命名收尾~~、~~#5 索引补齐~~：已由规划者按 D6 授权直接完成并关闭。
- ~~Mimosa 门禁阻塞~~：2026-10-02 修复高危后解锁，见 DECISIONS D5/D6。
- 9a4f4bc（showcase 抽离）已随状态提交推送。

## 本轮待办（按序）

1. **#4**（P0）重命名收尾提交 —— 一切的前置。
2. **#5**（P2）根 README 索引补齐 `microcomputer-course-design`、`ml-practice`。

## 背景与方向

当前方向见 [PLAN.md](PLAN.md)：收尾清理完成后转向**展示联动**（为 zhangjszs/huat-showcase 作品集站点服务，深化核心项目 README）。M2 的深化 issue 由规划者下轮创建，执行 Agent 暂不自行发起 README 重写。

## 规划者给执行 Agent 的备注

- `docs/agents/` 三个文件（domain.md、issue-tracker.md、triage-labels.md）已在 #4 中一并入库，AGENTS.md 引用它们，勿漏。
- 9a4f4bc（showcase 抽离）是用户此前会话的合法提交，规划者将随状态提交一并推送，执行 Agent 无需处理。
