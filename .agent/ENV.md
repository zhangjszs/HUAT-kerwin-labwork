# ENV.md — 环境与协作约定

- 仓库：`zhangjszs/HUAT-kerwin-labwork`（GitHub，SSH remote）。MIT 协议。
- 平台：Windows + Git Bash；`gh` CLI 可用；Git LFS 已启用（265 个文件）。
- 仓库性质：**课程作业存档**，31 个课程目录各自独立，无根构建系统、无根测试、无 CI（D4：暂不引入）。
- 人面内容用中文（README/commit 描述/issue），代码标识符用英文。
- commit 类型：`feat|fix|docs|chore|refactor`；`.agent/` 变更单独 commit，类型 `chore(agent)`。
- 二进制（`*.doc(x)`、`*.pdf`、`*.ppt(x)`、`*.xls(x)`、`*.jpg`、`*.apk`）必须走 LFS；`*.zip/*.rar/*.7z` 既 LFS 又 gitignore，**永不 `git add -f`**。
- 永不提交临时/AI 状态文件：`*~`、`*.bak`、`__pycache__/`、`tmp/`、`.omc/`、`.claude/`、`.trae/`、`.zcode/`（#4 会补上 `.zcode/` 的 ignore 规则）。
- 标签体系：优先级 `P0`–`P3`；状态 `ready-for-agent` / `ready-for-human` / `needs-triage` / `needs-info` / `blocked` / `auto-discovered`（2026-10-02 由规划者创建）。
- 特殊流程输入：`.agent/ISSUE_DRAFTS/`（执行 Agent 的 gh 不可用草稿）、`auto-discovered` 标签 issue、HANDOFF.md 的执行 Agent 留言。
