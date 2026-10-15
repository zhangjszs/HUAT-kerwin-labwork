# ENV.md — 环境与协作约定

- 仓库：`zhangjszs/HUAT-kerwin-labwork`（GitHub，SSH remote）。MIT 协议。
- 平台：**Windows，且本会话的 shell 是 `cmd.exe`，不是 bash**（2026-10-15 实测修正，旧版此栏误写「Windows + Git Bash」）。`gh` CLI 可用且已认证（keyring，账号 zhangjszs）；Git LFS 已启用。
- **shell 限制（踩过坑，务必遵守）**：
  - heredoc 不可用：`git commit -F - <<"EOF"` 报「此时不应有 <<」。提交信息写临时文件（放 `%TEMP%`，**不要放仓库内**）再用 `git commit -F <文件>`；或 `git commit --amend --no-edit`。
  - `2>$null` 会被 cmd 当作字面文件名，在仓库根目录建一个名为 `$null` 的空文件，污染 `git status`。重定向交给 PowerShell 或不用。
  - `... | head` / `| tail` 报「不是内部或外部命令」。
  - 传 `powershell -NoProfile -Command "..."` 时，脚本里的裸 `|` 在个别引号嵌套下仍被 cmd 拆掉；脚本较长或含正则/管道时，写成 `.ps1` / `.cjs` 放 `%TEMP%` 再 `-File` / `node` 执行。
  - `;` 不是 cmd 的命令分隔符（用 `&`），否则会被当成 git 子命令参数。
- 仓库性质：**课程作业存档**，31 个课程目录各自独立，无根构建系统、无根测试、无 CI（D4：暂不引入）。
- 人面内容用中文（README/commit 描述/issue），代码标识符用英文。
- commit 类型：`feat|fix|docs|chore|refactor`；`.agent/` 变更单独 commit，类型 `chore(agent)`。
- 二进制（`*.doc(x)`、`*.pdf`、`*.ppt(x)`、`*.xls(x)`、`*.jpg`、`*.apk`）必须走 LFS；`*.zip/*.rar/*.7z` 既 LFS 又 gitignore，**永不 `git add -f`**。
- 永不提交临时/AI 状态文件：`*~`、`*.bak`、`__pycache__/`、`tmp/`、`.omc/`、`.claude/`、`.trae/`、`.zcode/`。
- 标签体系（契约 1.3 为唯一权威，2026-10-03 起）：状态 `ready` / `in-progress` / `in-review` / `blocked` / `needs-info`（同一时刻最多一个）；优先级 `P0`–`P4`；来源 `auto-discovered`。旧 triage 词表（`ready-for-agent` / `ready-for-human` / `needs-triage`）已废弃并删除，勿用。
- 分支策略：契约 1.9 默认「每任务一分支」`agent/issue-<N>-<短slug>` → `main`（`--no-ff` merge）→ 删临时分支。ENV 未设 `WORKFLOW=trunk`，故按默认执行。
- 特殊流程输入：`.agent/ISSUE_DRAFTS/`（执行 Agent 的 gh 不可用草稿）、`auto-discovered` 标签 issue、HANDOFF.md 的执行 Agent 留言。

## 本机可用工具链（2026-10-15 实测）

| 用途 | 命令 / 结论 |
| ---- | ---- |
| 静态站点实测 | `node <script.cjs>`（Node **v24.19.0**，内置 `http` 模块可直接起静态服务，无需依赖） |
| Python | **不在 PATH**，`python -m http.server` 不可用 |
| 仓库级 build / test / lint / typecheck | **不存在**（无根构建系统、无 CI）——不要编造看似能跑的命令 |
| 单目录构建 | 查各课程目录自己的 `README.md`（C/C++ 用 CMake ≥3.10；Android 用 Gradle；Java 用 Maven/Gradle/IDE） |
| 图片目视核查 | **webp 不可读**（当前图片读取器会拒绝转码），jpg/png 可读 |

## 已知限制

- ~~**远程不可达（2026-10-15 起）**~~ —— **已恢复**：本轮开局时 `git fetch` / `pull --rebase` / `push` 全部失败（`Connection closed by 20.205.243.166 port 22`），曾按契约 1.6 降级为 TASKS.md 权威模式；收尾前最后一次 push 成功（`4f5aa93..a9ff084`）。若再次遇到同类失败，仍按契约 1.6 降级并在当轮产出中说明，不得静默跳过。
- 无头浏览器不可用，HTML 改动无法做真实渲染截图；以「HTTP 状态码 + DOM 结构配平检查」替代。
- `git grep` 的三个坑（2026-10-15 踩过）：① 检索的是文件**内容**而非文件名（找 `AndroidManifest.xml` 要用内容特征如 `AppCompatActivity`）；② 默认 BRE 不支持 `|` alternation，要用 `-E`；③ 路径规格 `CMakeLists.txt` 只匹配仓库根目录，嵌套目录要用 `:(glob)**/CMakeLists.txt`。
