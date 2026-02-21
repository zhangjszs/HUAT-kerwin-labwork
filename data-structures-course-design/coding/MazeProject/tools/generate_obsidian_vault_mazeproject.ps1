param(
  [Parameter(Mandatory = $true)][string]$VaultRoot,
  [Parameter(Mandatory = $true)][string]$TemplateDir,
  [Parameter(Mandatory = $true)][string]$RepoDir,
  [Parameter(Mandatory = $true)][string]$Short,
  [Parameter(Mandatory = $true)][string]$ProjectName
)

$ErrorActionPreference = "Stop"

function Ensure-Dir([string]$dir) {
  if (-not (Test-Path -LiteralPath $dir)) {
    New-Item -ItemType Directory -Path $dir -Force | Out-Null
  }
}

function Write-FileWithBackup([string]$path, [string]$content) {
  $parent = Split-Path -Parent $path
  Ensure-Dir $parent
  if (Test-Path -LiteralPath $path) {
    Copy-Item -LiteralPath $path -Destination ($path + ".backup") -Force
  }
  Set-Content -LiteralPath $path -Value $content -Encoding utf8
}

function Get-UserBlock([string]$text) {
  $m = [regex]::Match($text, "(?s)<!--\\s*USER-START\\s*-->(.*?)<!--\\s*USER-END\\s*-->")
  if ($m.Success) { return $m.Groups[1].Value.Trim("`r","`n") }
  return ""
}

function Get-Frontmatter([string]$text) {
  $m = [regex]::Match($text, "(?s)^---\\s*\\r?\\n(.*?)\\r?\\n---\\s*\\r?\\n")
  if (-not $m.Success) { return $null }
  return $m.Groups[1].Value
}

function Get-BodyWithoutFrontmatter([string]$text) {
  return [regex]::Replace($text, "(?s)^---\\s*\\r?\\n.*?\\r?\\n---\\s*\\r?\\n", "")
}

function Replace-RootKeyBlock([string]$yaml, [string]$key, [string]$replacementBlock) {
  $pattern = "(?ms)^(?<indent>){0}:(?:\\s*\\[\\]|\\s*\\{{\\}}|\\s*\\S.*)?\\s*$\\r?\\n(?:(?!^[A-Za-z_][A-Za-z0-9_]*\\s*:).*$\\r?\\n)*" -f [regex]::Escape($key)
  if ([regex]::IsMatch($yaml, $pattern)) {
    return [regex]::Replace($yaml, $pattern, $replacementBlock + "`n", 1)
  }
  # key not found; append at end (should not happen if template respected)
  return ($yaml.TrimEnd() + "`n" + $replacementBlock + "`n")
}

function Update-ProjectCardYaml([string]$existingYaml, [string]$newHighlightsBlock, [string]$newMetricsBlock, [string]$newRisksBlock, [string]$newTradeoffsBlock, [string]$newNextImproveBlock) {
  $yaml = $existingYaml
  $yaml = Replace-RootKeyBlock $yaml "highlights" $newHighlightsBlock
  $yaml = Replace-RootKeyBlock $yaml "metrics" $newMetricsBlock
  $yaml = Replace-RootKeyBlock $yaml "risks" $newRisksBlock
  $yaml = Replace-RootKeyBlock $yaml "tradeoffs" $newTradeoffsBlock
  $yaml = Replace-RootKeyBlock $yaml "next_improve" $newNextImproveBlock
  return $yaml.TrimEnd()
}

function Require-Template([string]$path) {
  if (-not (Test-Path -LiteralPath $path)) {
    throw "Missing template: $path"
  }
}

Require-Template (Join-Path $TemplateDir "T1_Project.md")
Require-Template (Join-Path $TemplateDir "T2_Module.md")
Require-Template (Join-Path $TemplateDir "T3_Topic.md")
Require-Template (Join-Path $TemplateDir "T5_Interview_QA.md")

$projectLink = "[[P__$Short]]"
$projectDir = Join-Path $VaultRoot ("01_Projects\\P__{0}" -f $Short)
$modulesDir = Join-Path $projectDir "Modules"
$qaDir = Join-Path $projectDir "QA"
$topicsDir = Join-Path $VaultRoot "03_Topics"

$projectCardPath = Join-Path $projectDir ("P__{0}.md" -f $Short)

$newHighlightsBlock = @"
highlights:
  - A*/BFS/DFS 三算法可视化 + 性能对比：访问节点、路径长度、执行时间(ms)（widget.cpp:L22-L31; widget.cpp:L632-L663）
  - A* 支持 8 邻域 + 欧式启发式，优先队列维护 open set（core/algorithms.cpp:L103-L103; core/algorithms.cpp:L322-L322; core/algorithms.cpp:L20-L20）
  - 逐步执行采用 QTimer 单次定时器，支持滑块调速（core/algorithms.h:L82-L82; core/algorithms.cpp:L46-L46; widget.cpp:L530-L534）
  - 随机迷宫生成支持障碍率控制，可复现实验对比（widget.cpp:L541-L553; core/algorithms.cpp:L428-L449）
"@.TrimEnd()

$newMetricsBlock = @"
metrics:
  latency_ms:
  hz:
  accuracy:
  error:
"@.TrimEnd()

$newRisksBlock = @"
risks:
  - UI线程阻塞风险：算法比较在 UI 线程同步执行（widget.cpp:L632-L663）
  - A* 对角移动代价/启发式一致性需确认（core/algorithms.cpp:L103-L103; core/algorithms.cpp:L111-L116; core/algorithms.cpp:L322-L322）
  - 地图尺寸/边界映射易出错：像素↔网格坐标依赖 offset 与 cellSize（mazewidget.cpp:L11-L16; mazewidget.cpp:L79-L80）
"@.TrimEnd()

$newTradeoffsBlock = @"
tradeoffs:
  - 逐步可视化（QTimer） vs 纯算法速度：可视化需要节拍延迟与频繁刷新（core/algorithms.cpp:L46-L46; widget.cpp:L507-L517）
  - 8邻域更短路径 vs 代价模型简化：对角代价固定 1.414，可能不满足某些约束（core/algorithms.cpp:L103-L103; core/algorithms.cpp:L111-L116）
  - 交互绘制网格 vs 可复现实验：需要稳定的地图导入/导出与固定随机种子（services/fileservice.cpp:L11-L23; widget.cpp:L538-L542）
"@.TrimEnd()

$newNextImproveBlock = @"
next_improve:
  - 把 compareAlgorithms/solver 迁移到工作线程，避免 UI 卡顿；把结果通过信号回传（widget.cpp:L632-L663; core/algorithms.h:L56-L60）
  - 补齐评测基线：同一张地图、多次运行统计均值/方差，并把 raw 结果落盘到 Evidence/（widget.cpp:L632-L663）
  - 为 A* 增加约束（禁止穿角/代价可配置），并在 UI 中提供开关（core/algorithms.cpp:L103-L103; core/algorithms.cpp:L111-L116）
  - 把“网格坐标→世界坐标→控制量”的链路做成可扩展接口，为后续 FSD 控制模块铺路（mazewidget.cpp:L79-L80; core/algorithms.h:L61-L66）
  - 【最该补的 5 项指标】Hz/延迟/误差/资源/鲁棒性：写清测量方法并记录到 Evidence/perf_*.txt（见正文第 7 节）
"@.TrimEnd()

$projectCardBody = @"
# $ProjectName（Project Card）

## 1. 一句话简介
- `widget.cpp:L60-L62` “Qt Widgets 桌面端迷宫寻路系统：交互式画墙/设起终点，支持 A*/BFS/DFS。”

## 2. 我负责什么（边界要清晰）
<!-- USER-START -->
- （在这里补充：你负责的子模块/具体贡献/你改过的 bug / 你做过的性能优化）
<!-- USER-END -->
- `core/algorithms.h:L19-L23` “实现并组织 AStar/BFS/DFS 三类搜索算法能力（算法层）。”
- `core/algorithms.h:L56-L60` “用 signals 输出 step/path/finished，把算法过程可视化（工程落地）。”
- `widget.cpp:L98-L122` “把 UI 控件与算法/画布事件连接起来，形成可操作系统（工程集成）。”

## 3. 系统/模块总览（链接到模块卡）
- [[M__感知]]
- [[M__定位]]
- [[M__规划]]
- [[M__控制]]
- [[M__工程]]
- `CMakeLists.txt:L15-L35` “工程按 core/models/services + UI 文件组织，算法/数据/服务分层明显。”

## 4. 核心技术亮点（可量化）
- `widget.cpp:L22-L31` “性能对比维度：访问节点/路径长度/执行时间(ms)，便于做工程化评测与复试表达。”
- `widget.cpp:L632-L663` “对 BFS/DFS/A* 逐个计时（chrono）并汇总成 AlgorithmResult。”
- `widget.cpp:L507-L517` “逐步可视化：每一步刷新网格并累计访问节点数。”
- `core/algorithms.cpp:L46-L46` “算法 runner 通过 m_timer.start(m_stepDelay) 控制节拍，支持实时调速。”

## 5. 关键证据链（我如何证明我做过、做对了）
- `CMakeLists.txt:L9-L13` “C++17 + Qt Widgets + CMake 工程配置（技术栈证据）。”
- `main.cpp:L6-L9` “QApplication 事件循环启动（工程入口证据）。”
- `core/algorithms.cpp:L11-L41` “A* 初始化：清理状态、设置启发式、open set 入队（算法实现证据）。”
- `core/algorithms.cpp:L67-L131` “A* 每步扩展邻居 + 调度下一步（可视化/实时性证据）。”
- `widget.cpp:L211-L238` “UI 触发 A* 并调用 startAStar（端到端链路证据）。”
- `widget.cpp:L596-L675` “compareAlgorithms：统一地图输入 + BFS/DFS/A* 计时与指标汇总（性能评测证据）。”

## 6. 复试追问高危点（提前准备）
- `core/algorithms.h:L88-L90` “A* open set 用 priority_queue：时间复杂度如何分析？为什么比 BFS 更快/更稳？”
- `core/algorithms.cpp:L103-L103` “8 邻域 + 对角代价：是否允许穿角？代价与启发式是否一致/可采纳？”
- `widget.cpp:L632-L663` “算法比较在 UI 线程：为什么会卡顿？如何用线程/任务队列改造？”
- `mazewidget.cpp:L79-L80` “像素坐标到网格坐标映射：边界与 offset/cellSize 变化时怎么保证正确？”

## 7. 下一步改进（面试可以讲""未来工作""）
- `core/algorithms.h:L56-L60` “把耗时计算迁移到工作线程，主线程只做渲染与交互（更像真实车端实时系统分层）。”
- 【最该补的 5 项指标】（当前仓库尚未形成稳定、可复现的 benchmark 证据链）
  1. 频率（Hz）：记录 stepExecuted 触发频率（steps/s）→ `Evidence/perf_hz.txt`（参考：widget.cpp:L507-L517）
  2. 延迟（ms）：记录一次搜索从点击到 finished 的端到端延迟 → `Evidence/perf_latency.txt`（参考：widget.cpp:L211-L238; widget.cpp:L519-L528）
  3. 准确率/误差：对固定地图集统计最短路径是否正确（与 BFS 最短对齐）→ `Evidence/accuracy.txt`（参考：widget.cpp:L666-L673）
  4. 资源占用：CPU/内存峰值（采样工具 + 运行日志）→ `Evidence/resource.txt`
  5. 鲁棒性测试：起终点非法/无解/极端障碍率的表现与提示 → `Evidence/robustness.txt`（参考：widget.cpp:L215-L218; widget.cpp:L597-L600）
"@

$projectCardContentNew = @"
---
type: project
status: active
time:
direction: 交通运输
scene: 智能交通/算法工程训练（迷宫寻路）
repo: $RepoDir
stack: [C++17, Qt Widgets, CMake]
skills: []
topics:
  - [[T__AStar]]
  - [[T__BFS]]
  - [[T__DFS]]
  - [[T__优先队列_堆]]
  - [[T__Qt_Signals_Slots]]
  - [[T__QTimer_事件驱动]]
  - [[T__性能测量_chrono]]
  - [[T__栅格地图_Grid2D]]
  - [[T__文件持久化_QFile_QTextStream]]
$newMetricsBlock
$newHighlightsBlock
evidence: []
$newRisksBlock
$newTradeoffsBlock
$newNextImproveBlock
---

$projectCardBody
"@

if (Test-Path -LiteralPath $projectCardPath) {
  $existing = Get-Content -LiteralPath $projectCardPath -Raw
  $existingYaml = Get-Frontmatter $existing
  if ($null -eq $existingYaml) {
    # If no frontmatter, fall back to overwrite with backup.
    Write-FileWithBackup $projectCardPath $projectCardContentNew
  } else {
    $userBlock = Get-UserBlock (Get-BodyWithoutFrontmatter $existing)
    if ($userBlock -ne "") {
      $projectCardBody = [regex]::Replace($projectCardBody, "(?s)<!--\\s*USER-START\\s*-->.*?<!--\\s*USER-END\\s*-->", "<!-- USER-START -->`n$userBlock`n<!-- USER-END -->")
    }
    $updatedYaml = Update-ProjectCardYaml $existingYaml $newHighlightsBlock $newMetricsBlock $newRisksBlock $newTradeoffsBlock $newNextImproveBlock
    $updatedContent = @"
---
$updatedYaml
---

$projectCardBody
"@
    Write-FileWithBackup $projectCardPath $updatedContent
  }
} else {
  Write-FileWithBackup $projectCardPath $projectCardContentNew
}

function Module-Card([string]$moduleName, [string]$layer, [string[]]$topics, [string]$body) {
  $topicsYaml = if ($topics.Count -gt 0) { ("topics:`n" + ($topics | ForEach-Object { "  - [[{0}]]" -f $_ } | Out-String).TrimEnd()) } else { "topics: []" }
  return @"
---
type: module
status: draft
time:
project: $projectLink
layer: $layer
inputs: []
outputs: []
interfaces: []
stack: []
$topicsYaml
metrics:
  latency_ms:
  hz:
evidence: []
risks: []
tradeoffs: []
---

# $moduleName（Module）

$body
"@
}

$modulePerceptionBody = @"
## 1. 模块职责（只做什么，不做什么）
- `models/mazegrid.h:L8-L15` “本项目的‘感知’等价为：构建/维护栅格占据状态（Wall/Start/End/Visited 等）。”
- `widget.cpp:L62-L62` “障碍物/起终点来自用户交互（画墙/右键设点），不是实际传感器输入。”

## 2. 输入输出与接口（话题/数据结构/文件）
- 输入：
- `mazewidget.h:L36-L37` “MazeWidget 通过 clicked/dragged 信号把网格坐标(i,j)传入主窗口。”
- 输出：
- `widget.cpp:L205-L209` “左键画墙后刷新画布（占据栅格更新可视化）。”
- 关键接口：
- `core/algorithms.cpp:L428-L449` “随机迷宫生成：按 obstacleRatio 把栅格置为 Wall（数据产生接口）。”
- `services/fileservice.cpp:L11-L23` “地图保存：QFile + QTextStream 按行写入 0/1（数据落盘接口）。”
- `services/fileservice.cpp:L28-L46` “地图加载：逐行解析整数矩阵（数据回放接口）。”

## 3. 关键算法/数据结构（链接到 Topic）
- [[T__栅格地图_Grid2D]]
- [[T__文件持久化_QFile_QTextStream]]
- [[T__Qt_Signals_Slots]]

## 4. 性能与瓶颈
- 当前：
- `mazewidget.cpp:L26-L66` “渲染按双层 for 遍历 Grid2D，每格绘制颜色与边框。”
- 瓶颈：
- `mazewidget.cpp:L26-L66` “格子越大、刷新越频繁，paintEvent 开销越高。”
- 优化手段（缓存/并行/降采样/鲁棒性）：
- `constants.h:L33-L33` “拖拽刷新有节流阈值 MOUSE_MOVE_THROTTLE_MS，可进一步做脏矩形/增量刷新。”

## 5. 证据（实验/日志/参数/截图）
- `models/mazegrid.cpp:L8-L20` “MazeGrid 初始化：为每个格子分配 APoint 并存入二维结构。”
"@

$moduleLocalizationBody = @"
## 1. 模块职责（只做什么，不做什么）
- `mazewidget.cpp:L11-L16` “维护 cellSize 与 offset，实现窗口尺寸变化时的网格布局。”
- `mazewidget.cpp:L79-L80` “把鼠标像素坐标映射成网格索引(i,j)，形成‘定位’意义上的坐标变换。”

## 2. 输入输出与接口（话题/数据结构/文件）
- 输入：
- `mazewidget.cpp:L74-L85` “mousePressEvent 读取 event->position() 并计算(i,j)。”
- 输出：
- `mazewidget.h:L36-L37` “输出 clicked(i,j,button) / dragged(i,j) 信号给上层决策/算法。”
- 关键接口：
- `mazewidget.cpp:L9-L16` “updateSize() 根据 MAX_X/MAX_Y 计算 cellSize、offset 与最小窗口尺寸。”

## 3. 关键算法/数据结构（链接到 Topic）
- [[T__栅格地图_Grid2D]]
- [[T__Qt_Signals_Slots]]

## 4. 性能与瓶颈
- 当前：
- `mazewidget.cpp:L19-L24` “resizeEvent 触发 updateSize + update，保证布局实时更新。”
- 瓶颈：
- `mazewidget.cpp:L19-L24` “频繁 resize 会导致高频重绘；可考虑限制刷新频率/延后重绘。”
- 优化手段（缓存/并行/降采样/鲁棒性）：
- `mazewidget.cpp:L100-L102` “拖拽绘制通过时间阈值节流，减少无效刷新。”

## 5. 证据（实验/日志/参数/截图）
- `constants.h:L23-L25` “MAX_X/MAX_Y/gridSize 通过 inline getter 可动态调整，影响坐标映射与渲染。”
"@

$modulePlanningBody = @"
## 1. 模块职责（只做什么，不做什么）
- `core/algorithms.h:L19-L23` “规划层提供 A*/BFS/DFS 三种搜索策略。”
- `widget.cpp:L211-L238` “规划从 UI 触发：点击按钮后检查起终点并启动对应算法。”

## 2. 输入输出与接口（话题/数据结构/文件）
- 输入：
- `widget.cpp:L259-L267` “BFS/DFS 输入：把 Grid2D 墙/空转换成 0/1 maze 矩阵。”
- 输出：
- `core/algorithms.h:L56-L60` “输出：stepExecuted/pathFound/searchFinished 信号驱动可视化与提示。”
- 关键接口：
- `core/algorithms.h:L61-L66` “AlgorithmRunner::startAStar/startBFS/startDFS 是统一启动接口。”
- `core/algorithms.h:L107-L116` “MazeSolver 提供 findPathBFS/findPathDFS/compareAllAlgorithms 便于离线评测。”

## 3. 关键算法/数据结构（链接到 Topic）
- [[T__AStar]]
- [[T__BFS]]
- [[T__DFS]]
- [[T__优先队列_堆]]
- [[T__栅格地图_Grid2D]]

## 4. 性能与瓶颈
- 当前：
- `widget.cpp:L632-L663` “比较模式记录 visitedCount/pathLength/executionTime(ms)，能量化算法差异。”
- 瓶颈：
- `widget.cpp:L632-L663` “compareAlgorithms 同步执行可能阻塞 UI；大地图时更明显。”
- 优化手段（缓存/并行/降采样/鲁棒性）：
- `core/algorithms.cpp:L46-L46` “可视化 runner 用定时器分步执行，避免一次性爆发式计算。”
- `core/algorithms.cpp:L103-L103` “A* 使用 8 邻域，路径更短但扩展分支更多；可做邻域/启发式可配置。”

## 5. 证据（实验/日志/参数/截图）
- `core/algorithms.cpp:L20-L20` “A* open set 使用 priority_queue（小根堆语义由 CompareAStar 保证）。”
- `core/algorithms.cpp:L331-L370` “BFS 使用 queue 做层序扩展，天然保证最短步数（4 邻域）。”
- `core/algorithms.cpp:L392-L419` “DFS 用递归 dfsHelper 回溯路径，易陷入深分支（非最短）。”
"@

$moduleControlBody = @"
## 1. 模块职责（只做什么，不做什么）
- `core/algorithms.h:L69-L69` “控制层在本项目中指：控制算法步进节拍（m_stepDelay），而非车辆纵横向控制。”
- `core/algorithms.cpp:L46-L46` “用 QTimer 驱动每一步执行，形成可控的实时节奏。”

## 2. 输入输出与接口（话题/数据结构/文件）
- 输入：
- `widget.cpp:L530-L534` “输入：速度滑块把 value 转成 delay，并调用 AlgorithmRunner::setSpeed。”
- 输出：
- `widget.cpp:L507-L517` “输出：每个 step 更新 Mainmap 状态 + visitedCount + refresh（可视化闭环）。”
- 关键接口：
- `core/algorithms.cpp:L43-L52` “scheduleNextStep 统一调度下一步（timer.start）。”

## 3. 关键算法/数据结构（链接到 Topic）
- [[T__QTimer_事件驱动]]
- [[T__Qt_Signals_Slots]]

## 4. 性能与瓶颈
- 当前：
- `core/algorithms.h:L81-L83` “定时器 singleShot + m_stepDelay 控制可视化速度，便于演示与调参。”
- 瓶颈：
- `widget.cpp:L507-L517` “每 step 都 refresh，会导致高频 paintEvent；大图下容易抖动/卡顿。”
- 优化手段（缓存/并行/降采样/鲁棒性）：
- `constants.h:L33-L33` “节流阈值已用于拖拽；同理可用于 step 刷新（例如每 N 步刷新一次）。”

## 5. 证据（实验/日志/参数/截图）
- `core/algorithms.cpp:L8-L8` “QTimer timeout 连接到 onTimerTimeout，驱动 A*/BFS/DFS 分步执行。”
"@

$moduleEngineeringBody = @"
## 1. 模块职责（只做什么，不做什么）
- `CMakeLists.txt:L9-L13` “工程化：CMake + Qt 自动 MOC/UIC/RCC，形成可构建的桌面应用。”
- `widget.cpp:L98-L122` “工程集成：connect 把按钮/滑块/画布事件统一接入到业务逻辑。”

## 2. 输入输出与接口（话题/数据结构/文件）
- 输入：
- `main.cpp:L6-L9` “输入来自 Qt 事件循环：键盘/鼠标/UI 控件事件。”
- 输出：
- `widget.cpp:L712-L712` “输出：状态栏展示结果文本，CompareDialog 展示表格指标。”
- 关键接口：
- `services/fileservice.cpp:L11-L23` “文件接口：saveMap/loadMap 支持把地图写入/读出文本。”

## 3. 关键算法/数据结构（链接到 Topic）
- [[T__Qt_Signals_Slots]]
- [[T__性能测量_chrono]]
- [[T__文件持久化_QFile_QTextStream]]

## 4. 性能与瓶颈
- 当前：
- `widget.cpp:L632-L635` “用 chrono 计时得到 ms 级执行时间，适合作为复试‘性能优化’素材。”
- 瓶颈：
- `widget.cpp:L632-L663` “比较功能同步执行；若地图变大，应改成异步任务 + UI 更新。”
- 优化手段（缓存/并行/降采样/鲁棒性）：
- `core/algorithms.h:L56-L60` “算法与 UI 通过信号解耦，天然适合迁移到工作线程（保持接口不变）。”

## 5. 证据（实验/日志/参数/截图）
- `widget.h:L93-L101` “Widget 定义 updateTextEdit 信号与多个 slots，符合 Qt 事件驱动范式。”
"@

$modules = @(
  @{ Name = "感知"; Layer = "感知"; Topics = @("T__栅格地图_Grid2D","T__文件持久化_QFile_QTextStream","T__Qt_Signals_Slots"); Body = $modulePerceptionBody },
  @{ Name = "定位"; Layer = "定位"; Topics = @("T__栅格地图_Grid2D","T__Qt_Signals_Slots"); Body = $moduleLocalizationBody },
  @{ Name = "规划"; Layer = "规划"; Topics = @("T__AStar","T__BFS","T__DFS","T__优先队列_堆","T__栅格地图_Grid2D"); Body = $modulePlanningBody },
  @{ Name = "控制"; Layer = "控制"; Topics = @("T__QTimer_事件驱动","T__Qt_Signals_Slots"); Body = $moduleControlBody },
  @{ Name = "工程"; Layer = "工程"; Topics = @("T__Qt_Signals_Slots","T__性能测量_chrono","T__文件持久化_QFile_QTextStream"); Body = $moduleEngineeringBody }
)

$moduleNameEn = @{
  "感知" = "Perception"
  "定位" = "Localization"
  "规划" = "Planning"
  "控制" = "Control"
  "工程" = "Engineering"
}

foreach ($m in $modules) {
  $path = Join-Path $modulesDir ("M__{0}.md" -f $m.Name)
  $displayName = "{0}（{1}）" -f $m.Name, $moduleNameEn[$m.Name]
  $content = Module-Card $displayName $m.Layer $m.Topics $m.Body
  Write-FileWithBackup $path $content
}

$qaPath = Join-Path $qaDir ("I__{0}__QA.md" -f $Short)
$qaContent = @"
---
type: interview
status: draft
scope: project
project: $projectLink
module:
topics:
  - [[T__AStar]]
  - [[T__BFS]]
  - [[T__DFS]]
  - [[T__优先队列_堆]]
  - [[T__Qt_Signals_Slots]]
  - [[T__QTimer_事件驱动]]
  - [[T__性能测量_chrono]]
  - [[T__栅格地图_Grid2D]]
  - [[T__文件持久化_QFile_QTextStream]]
---

# $ProjectName 复试问答（20问20答）

> 目标：每题回答 30~60 秒；每题带 1 个“证据点”；至少 6 题显式 408 联动（追问陷阱点名考点）。

## Q1
- **问：** 这个项目的整体架构/分层是怎样的？你怎么保证模块边界清晰？
- **答（口语化，30~60秒）：** 我把系统拆成 UI/交互层（Widget+MazeWidget）、算法层（core/algorithms）、数据层（MazeGrid/Grid2D）和服务层（FileService）。UI 只负责收集输入、展示状态和把事件分发；算法层通过 signals 把“每一步”反馈出来；数据层提供统一的栅格点结构，避免 UI 和算法各自维护一套状态。这样复试讲起来也清晰：我既能讲算法，也能讲工程集成与可视化闭环。
- **证据：** `CMakeLists.txt:L15-L35` “PROJECT_SOURCES 显式包含 core/models/services + UI 文件，结构分层清晰。”
- **追问陷阱：** 你如何防止 UI 直接改算法内部状态导致耦合？你会怎么做接口抽象/单元测试？

## Q2
- **问：** 项目的入口和事件驱动模型是什么？为什么这样设计？
- **答（口语化，30~60秒）：** 入口是 Qt Widgets 应用：main 里启动 QApplication，再 show 主窗口，最后进入 a.exec() 的事件循环。这个模型天然适合交互式系统：鼠标拖拽画墙、按钮触发算法、定时器逐步执行都靠事件循环调度。对复试来说我会强调：这种事件驱动和 ROS 回调/实时系统调度的思路是类似的，工程上要关注“谁在主线程跑、会不会卡 UI”。
- **证据：** `main.cpp:L6-L9` “QApplication + a.exec() 事件循环。”
- **追问陷阱：** 如果算法很慢导致 UI 卡住，你怎么定位并改成异步？（408：OS 线程/调度/同步）

## Q3
- **问：** 你怎么把算法过程可视化？为什么不用一次性跑完？
- **答（口语化，30~60秒）：** 我用 AlgorithmRunner 把搜索拆成“可执行的离散步”，每步 emit stepExecuted，把某个格子标成 Visited 或 Path，然后 UI 收到信号就刷新。好处是可视化清楚，面试时也好讲实时性与可观测性；另外还能用滑块调节 stepDelay，方便在不同地图大小下权衡流畅度和速度。
- **证据：** `core/algorithms.cpp:L46-L46` “m_timer.start(m_stepDelay) 调度下一步，支持分步执行。”
- **追问陷阱：** 如果 stepExecuted 触发太频繁导致 repaint 变慢，你怎么做节流/批量更新？

## Q4
- **问：** 你实现的 A* 核心机制是什么？open set 为什么用优先队列？
- **答（口语化，30~60秒）：** A* 的核心是用 f=g+h 做最优优先扩展：g 是从起点累计代价，h 是到终点的启发式估计。我用 priority_queue 来维护 open set，每次取 f 最小的节点扩展；同时用 visitedSet 去重，避免重复处理。这样相比 BFS 能把搜索集中在更可能接近终点的区域，通常访问节点更少。
- **证据：** `core/algorithms.h:L88-L90` “A* open set 使用 std::priority_queue 并由 CompareAStar 比较 f_cost。”
- **追问陷阱：** h 必须满足什么性质才能保证最优？如果启发式不可采纳会怎样？（408：数据结构 堆/优先队列）

## Q5
- **问：** BFS/DFS 的区别你怎么讲？在这个项目里各自适合什么场景？
- **答（口语化，30~60秒）：** BFS 用队列做层序扩展，在无权图里能保证最短步数，所以它是“最短路基线”；DFS 用栈/递归深入，很快能找到一条路但不保证最短，更多用于探索或演示。这个项目里 BFS 我用来对齐最短路径正确性，而 DFS 更多用于展示搜索策略差异。
- **证据：** `core/algorithms.cpp:L331-L342` “BFS 使用 queue，while 循环逐层出队扩展。”
- **追问陷阱：** DFS 为什么不保证最短？你怎么构造反例？（408：图搜索 BFS/DFS）

## Q6
- **问：** 你这里的代价模型是什么？为什么有 8 邻域和对角代价？
- **答（口语化，30~60秒）：** 我在 A* 里允许 8 邻域移动，对角用 1.414 近似 sqrt(2)，直走用 1.0。这样路径更接近“欧氏最短”。但工程上要注意：如果地图障碍很密，对角可能出现穿角问题；以及启发式和代价的一致性也需要验证，必要时可以切到 4 邻域或加约束。
- **证据：** `core/algorithms.cpp:L103-L116` “directions[8] + isDiagonal + moveCost=1.414/1.0。”
- **追问陷阱：** 你如何禁止穿角？代价可配置怎么设计？（408：复杂度/正确性证明）

## Q7
- **问：** 你怎么统计“访问节点数”和“路径长度”？这些指标面试怎么讲？
- **答（口语化，30~60秒）：** visitedCount 我在可视化步进里按 stepExecuted 逐步累计，同时在 compare 模式里从算法结果里直接取 visitedCount；路径长度就是最终 path 的长度。面试表达上，我会把它当作“可量化证据”：同一地图下 A* 通常访问节点更少、耗时更短；BFS 最短但可能扩展更广；DFS 访问少不代表最优。
- **证据：** `widget.cpp:L507-L516` “onAlgorithmStep 每步 m_visitedCount++ 并显示 ‘已访问’。”
- **追问陷阱：** visitedCount 与实际时间关系是什么？如何避免 UI 刷新把时间污染？

## Q8
- **问：** 你在 compareAlgorithms 里为什么还写了一个 MazeSolver？和 AlgorithmRunner 的区别是什么？
- **答（口语化，30~60秒）：** AlgorithmRunner 是为了可视化和交互，强调“分步执行”和 UI 信号；MazeSolver 更像离线评测工具，一次性算完并返回 path、visited、time，便于做纯性能对比，不被 UI 刷新影响。两套的边界不同：一个面向实时演示，一个面向 benchmark。
- **证据：** `widget.cpp:L625-L663` “compareAlgorithms 使用 MazeSolver::findPathBFS/findPathDFS/compareAllAlgorithms 并计时。”
- **追问陷阱：** 两套实现如何保证一致性？你会如何写测试对齐输出？

## Q9
- **问：** 当前项目最可能的性能瓶颈在哪里？你会怎么定位？
- **答（口语化，30~60秒）：** 两类瓶颈：算法计算和 UI 渲染。算法侧可以看 visitedCount、open set 操作次数；UI 侧主要是 paintEvent 双重循环，每一步 refresh 都会触发重绘。定位上我会先把 compare 模式放大地图测 ms，再把可视化关掉对比；然后用采样工具看 CPU 热点，确认是算法还是绘制。
- **证据：** `mazewidget.cpp:L26-L66` “paintEvent 遍历整个 Grid2D 并逐格绘制。”
- **追问陷阱：** 你如何做‘脏矩形’或每 N 步刷新一次？会影响观感吗？

## Q10
- **问：** 你是怎么做计时的？ms 级指标如何避免测量误差？
- **答（口语化，30~60秒）：** 我用 std::chrono::high_resolution_clock::now() 获取起止时间，然后 duration<double, milli> 转成 ms。为了减少误差，应该在同一地图上多次运行取均值/方差，并把 UI 刷新从计时区间剥离；另外还要固定随机种子，保证可复现。
- **证据：** `widget.cpp:L632-L635` “chrono::high_resolution_clock + duration(milli) 计算 bfsTime。”
- **追问陷阱：** high_resolution_clock 一定单调吗？在 Windows 上你怎么选更稳的时钟？（408：计组/OS 计时与中断）

## Q11
- **问：** 如果地图变大到 200x200，你会优先做哪三件优化？
- **答（口语化，30~60秒）：** 第一，渲染优化：减少每步全图重绘，改成增量更新或降刷新频率。第二，线程化：把 compare/一次性求解放到工作线程，避免阻塞 UI。第三，算法/数据结构优化：A* 的 open set 用堆没问题，但 visited/parent 的结构要减少频繁分配，必要时用数组替代 map/set。
- **证据：** `core/algorithms.cpp:L517-L517` “compareAllAlgorithms 的 openSet 是 priority_queue（堆），visited/parent 用 set/map。”
- **追问陷阱：** 为什么 set/map 可能慢？你会换成什么结构？复杂度怎么变？（408：哈希/树/复杂度）

## Q12
- **问：** 你为什么用 Qt signals/slots，而不是直接函数回调或全局变量？
- **答（口语化，30~60秒）：** signals/slots 能把模块解耦：算法只负责发事件，不关心 UI 怎么画；UI 也不需要侵入算法内部。工程上这让后续迁移到线程更自然——把 AlgorithmRunner 放到工作线程时，信号跨线程排队投递，UI 线程仍然安全更新界面。
- **证据：** `widget.cpp:L117-L118` “connect 算法 runner 的 stepExecuted/searchFinished 到 Widget slots。”
- **追问陷阱：** 跨线程信号默认是什么连接方式？会不会数据竞争？（408：OS 同步/竞态）

## Q13
- **问：** 你怎么处理用户的非法输入（没设起终点/起终点重叠/设在墙上）？
- **答（口语化，30~60秒）：** 我在按钮触发时先判断 startPoint/endPoint 是否有效，否则直接 return 并在 statusBar 提示。类似的约束也可以放到点击事件里，比如禁止把起终点设成墙或重叠。面试里我会强调这是鲁棒性：系统要对异常输入有明确提示和可恢复路径。
- **证据：** `widget.cpp:L215-L218` “未设置起终点时提示错误并 return。”
- **追问陷阱：** 你会怎么做更系统的输入校验？（状态机/断言/错误码）

## Q14
- **问：** 你如何把地图持久化？为什么选择文本格式？
- **答（口语化，30~60秒）：** 我用 QFile 打开文件，QTextStream 逐行把 0/1 矩阵写出去，加载时再按空格拆分解析成 int。文本格式最大的优势是可读、可调试、便于做对比实验；缺点是体积大、解析慢，但对这个项目规模足够。
- **证据：** `services/fileservice.cpp:L11-L23` “saveMap：QFile + QTextStream 按行写入整数。”
- **追问陷阱：** 如果要更快/更稳，你会换成什么格式？怎么做版本兼容？

## Q15（408 联动：数据结构）
- **问：** A* 为什么离不开“堆/优先队列”？如果用 vector 每次找最小会怎样？
- **答（口语化，30~60秒）：** A* 每一步都需要取 f 最小的节点扩展，用堆能把“取最小+插入”控制在 logN；如果用 vector 每次线性扫描最小，就是 O(N) 级别，会让整体复杂度上升，地图大时非常明显。面试我会把它讲成：算法正确性靠 f=g+h，性能靠数据结构。
- **证据：** `core/algorithms.h:L88-L90` “m_openSetAStar 是 priority_queue（堆）。”
- **追问陷阱：** 408 考点：堆的插入/删除最小复杂度；priority_queue 的底层结构是什么？

## Q16（408 联动：图搜索）
- **问：** 为什么 BFS 能保证最短？你的实现里最短性依赖哪些条件？
- **答（口语化，30~60秒）：** BFS 按层扩展，第一次到达终点一定是最少步数，这个结论依赖图是无权或每条边代价相同。在我的实现里 directions 是 4 邻域，扩展顺序由队列保证层序；如果改成不同 moveCost，就不能直接用 BFS 了，需要 Dijkstra 或 A*。
- **证据：** `core/algorithms.cpp:L331-L342` “BFS while + queue，按出队顺序层序扩展。”
- **追问陷阱：** 408 考点：BFS/DFS/A* 的适用条件与最短性证明；如果有权边怎么做？

## Q17（408 联动：OS 实时性/调度）
- **问：** 你用 QTimer 做分步执行，本质是在做什么“调度”？有什么抖动/延迟问题？
- **答（口语化，30~60秒）：** 本质是在事件循环里做软实时调度：每次 timeout 触发下一步，把计算切成小片段，避免一次性占满主线程。问题是：timer 精度有限，UI 绘制也占时间，导致延迟抖动；地图大时每步耗时超过 stepDelay，会出现“跟不上”的现象。工程上可以用后台线程计算+主线程渲染，或用帧率上限控制。
- **证据：** `core/algorithms.cpp:L43-L52` “scheduleNextStep 用 m_timer.start(m_stepDelay) 调度下一步。”
- **追问陷阱：** 408 考点：实时调度/优先级/中断延迟；什么情况下会发生抖动？怎么测？

## Q18（408 联动：OS 线程/同步）
- **问：** 如果把 MazeSolver 放到工作线程，你怎么保证线程安全？哪些数据要保护？
- **答（口语化，30~60秒）：** 原则是：工作线程只读输入 maze，不直接写 UI 结构；UI 更新通过信号把结果回传，避免跨线程直接操作 QWidget。共享数据比如 Mainmap 或 results 容器要么做拷贝，要么用互斥锁保护，但更推荐消息传递。这样既符合 Qt 线程模型，也能避免竞态和死锁。
- **证据：** `core/algorithms.h:L56-L60` “AlgorithmRunner 已用 signals 做消息传递，天然适合跨线程。”
- **追问陷阱：** 408 考点：线程/进程区别、互斥锁/条件变量、死锁四条件；Qt 的 queued connection 是什么？

## Q19（408 联动：计组 缓存/局部性）
- **问：** 你的栅格数据结构是二维 vector+shared_ptr，这对缓存友好吗？你会怎么优化？
- **答（口语化，30~60秒）：** 现在 Grid2D 是 vector<vector<shared_ptr<APoint>>>，指针跳转多，缓存局部性比较差；大地图时会影响遍历/渲染/搜索性能。优化方向是把 APoint 做成连续数组（例如 vector<APoint> 或扁平化 1D 数组），用索引代替 shared_ptr，减少 cache miss。面试我会把它讲成：数据结构决定性能上限。
- **证据：** `models/mazegrid.h:L37-L37` “Grid2D 定义为 vector<vector<APointPtr>>（指针型二维结构）。”
- **追问陷阱：** 408 考点：缓存局部性、cache miss 代价；为什么连续内存更快？

## Q20（408 联动：OS 内存管理）
- **问：** 你大量使用 shared_ptr，会有什么代价？怎么避免内存泄漏或循环引用？
- **答（口语化，30~60秒）：** shared_ptr 带来引用计数开销和可能的循环引用风险；这里 parent 也是 shared_ptr，理论上如果形成环会泄漏，不过路径树是单向父指针，一般不会回指。更稳的做法可以把 parent 改成弱引用或用坐标索引存 parent，既省内存又更快。复试追问时我会强调：工程上要看对象生命周期和所有权。
- **证据：** `models/mazegrid.h:L17-L25` “APoint::parent 使用 std::shared_ptr（父指针结构）。”
- **追问陷阱：** 408 考点：虚拟内存/分配/泄漏；shared_ptr/weak_ptr 的区别与适用场景？
"@

Write-FileWithBackup $qaPath $qaContent

function Topic-Card([string]$topicName, [string[]]$courseRefs, [string[]]$projects, [string[]]$keywords, [string]$body) {
  $courseYaml = if ($courseRefs.Count -gt 0) { ("course_refs:`n" + ($courseRefs | ForEach-Object { "  - " + $_ } | Out-String).TrimEnd()) } else { "course_refs: []" }
  $projectsYaml = if ($projects.Count -gt 0) { ("projects:`n" + ($projects | ForEach-Object { "  - " + $_ } | Out-String).TrimEnd()) } else { "projects: []" }
  $keywordsYaml = if ($keywords.Count -gt 0) { ("keywords:`n" + ($keywords | ForEach-Object { "  - " + $_ } | Out-String).TrimEnd()) } else { "keywords: []" }
  return @"
---
type: topic
status: draft
$courseYaml
$projectsYaml
$keywordsYaml
---

# $topicName（Topic）

$body
"@
}

$topicCards = @(
  @{
    File = "T__AStar.md"
    Title = "A* 搜索（AStar）"
    Course = @("408-DS: 堆/优先队列", "408-DS: 图搜索/A*")
    Keywords = @("A*", "heuristic", "priority_queue", "f=g+h")
    Body = @"
## 1. 一句话定义（面试口径）
- A* 是一种带启发式的最短路搜索：用 f=g+h 优先扩展更“接近终点”的节点。

## 2. 核心机制（用自己的话讲清楚）
- open set 维护候选节点（按 f 最小弹出）；visited 负责去重；parent 用于回溯路径。

## 3. 复杂度/代价（面试常问）
- 时间：常见实现为 O(E log V) 量级（堆操作主导，取决于启发式质量）。
- 空间：open set + visited + parent 占用与搜索范围相关。
- 工程代价：启发式、邻域、代价模型要与约束一致，否则会出现“看似更短但不合法”的路径。

## 4. 常见追问与答法（3~5条）
- Q: h 需要满足什么性质？
  A: 为了保证最优，h 通常要求可采纳（不高估）并尽量一致；否则可能退化或不最优。
- Q: 为什么用堆？
  A: 每步都要取最小 f，堆把取最小/插入降到 logN。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__规划]]
- `core/algorithms.h:L88-L90` “A* open set 用 priority_queue 按 f_cost 排序。”
- `core/algorithms.cpp:L103-L116` “A* 8 邻域 + 对角代价 1.414/1.0 的代价模型。”
- `core/algorithms.cpp:L322-L322` “启发式使用欧式距离（sqrt(pow(dx,2)+pow(dy,2))）。”
"@
  },
  @{
    File = "T__BFS.md"
    Title = "BFS（广度优先搜索）"
    Course = @("408-DS: 图搜索 BFS")
    Keywords = @("BFS", "queue", "shortest path (unweighted)")
    Body = @"
## 1. 一句话定义（面试口径）
- BFS 按层扩展，能在无权图里保证最短步数路径。

## 2. 核心机制（用自己的话讲清楚）
- 用队列维护 frontier；出队一个点，扩展其邻居并入队；首次到达终点即最短。

## 3. 复杂度/代价（面试常问）
- 时间：O(V+E)（实现细节会受 visited 结构影响）。
- 空间：队列 + visited/parent。
- 工程代价：若边有权重或代价不等，BFS 不再保证最短。

## 4. 常见追问与答法（3~5条）
- Q: 为什么 BFS 最短？
  A: 因为按层推进，层数就是步数；首次到达终点即最小层数。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__规划]]
- `core/algorithms.cpp:L331-L342` “BFS 用 queue 出队扩展（层序搜索骨架）。”
- `widget.cpp:L259-L275` “把 Grid2D 转成 0/1 maze 后调用 startBFS（端到端链路）。”
"@
  },
  @{
    File = "T__DFS.md"
    Title = "DFS（深度优先搜索）"
    Course = @("408-DS: 图搜索 DFS")
    Keywords = @("DFS", "stack", "recursion")
    Body = @"
## 1. 一句话定义（面试口径）
- DFS 优先沿着一条分支深入探索，适合做可达性/遍历，不保证最短路径。

## 2. 核心机制（用自己的话讲清楚）
- 用栈或递归推进；走不通再回溯；可配合 parent/path 记录路径。

## 3. 复杂度/代价（面试常问）
- 时间：O(V+E)（遍历意义上），但找到解的速度取决于分支顺序。
- 空间：递归栈/显式栈。
- 工程代价：容易走入深分支；对最短路不友好。

## 4. 常见追问与答法（3~5条）
- Q: DFS 为什么不最短？
  A: 因为它不按层扩展，可能先找到一条很长的路径。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__规划]]
- `core/algorithms.cpp:L392-L419` “MazeSolver::dfsHelper 递归推进并回溯 path（DFS 典型实现）。”
- `widget.cpp:L296-L312` “UI 触发 DFS：构造 maze 并调用 startDFS。”
"@
  },
  @{
    File = "T__优先队列_堆.md"
    Title = "优先队列/堆（priority_queue / heap）"
    Course = @("408-DS: 堆/优先队列", "408-DS: 复杂度分析")
    Keywords = @("heap", "priority_queue", "logN")
    Body = @"
## 1. 一句话定义（面试口径）
- 堆是一种支持快速取最值的数据结构，优先队列常用堆实现。

## 2. 核心机制（用自己的话讲清楚）
- 通过完全二叉树（数组）维护堆序性质：插入/删除最值通过上滤/下滤保持性质。

## 3. 复杂度/代价（面试常问）
- 时间：push/pop 通常是 O(logN)，top 是 O(1)。
- 空间：O(N)。
- 工程代价：比较函数要正确；频繁分配会影响常数项。

## 4. 常见追问与答法（3~5条）
- Q: 为什么不是平衡树？
  A: 堆更轻量、常数更小，且只需要最值操作。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__规划]]
- `core/algorithms.h:L88-L90` “A* open set 是 priority_queue。”
- `core/algorithms.cpp:L517-L517` “compareAllAlgorithms 的 openSet 也是 priority_queue（配合 greater）。”
"@
  },
  @{
    File = "T__Qt_Signals_Slots.md"
    Title = "Qt Signals/Slots（事件驱动解耦）"
    Course = @("408-OS: IPC/同步（类比）")
    Keywords = @("signals", "slots", "event-driven", "decoupling")
    Body = @"
## 1. 一句话定义（面试口径）
- Qt 的 signals/slots 是一种事件驱动通信机制，用于解耦“事件产生者”和“事件处理者”。

## 2. 核心机制（用自己的话讲清楚）
- 对象发出 signal，接收方的 slot 被调用；跨线程时可用队列投递，避免直接并发访问 UI。

## 3. 复杂度/代价（面试常问）
- 时间：主要是一次分发开销（相对较小，取决于连接类型）。
- 空间：连接表与参数拷贝（跨线程时）。
- 工程代价：需要明确线程边界；UI 更新必须在主线程。

## 4. 常见追问与答法（3~5条）
- Q: 这和回调有什么区别？
  A: 更强的解耦与对象生命周期管理；跨线程投递更自然。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__工程]]
- `core/algorithms.h:L56-L60` “AlgorithmRunner 定义 stepExecuted/pathFound/searchFinished 信号。”
- `widget.cpp:L117-L118` “Widget 连接 runner 信号到 onAlgorithmStep/onAlgorithmFinished。”
- `mazewidget.h:L36-L37` “MazeWidget 发出 clicked/dragged 信号，Widget 接收并更新地图。”
"@
  },
  @{
    File = "T__QTimer_事件驱动.md"
    Title = "QTimer（事件循环中的软实时调度）"
    Course = @("408-OS: 实时调度/中断/延迟抖动（类比）")
    Keywords = @("QTimer", "event loop", "scheduling", "jitter")
    Body = @"
## 1. 一句话定义（面试口径）
- QTimer 是基于事件循环的定时触发器，可用于把长任务切片成可调度的小步执行。

## 2. 核心机制（用自己的话讲清楚）
- timer.start(ms) 后，timeout 事件在事件循环中到期触发；singleShot 模式每次只触发一次。

## 3. 复杂度/代价（面试常问）
- 代价：精度受系统调度与 UI 工作负载影响；无法保证硬实时。
- 工程代价：若每步耗时 > 周期，会出现积压与抖动。

## 4. 常见追问与答法（3~5条）
- Q: 如何保证不阻塞 UI？
  A: 把计算放到工作线程，主线程只负责渲染；或降低刷新频率。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__控制]]
- `core/algorithms.h:L82-L83` “AlgorithmRunner 内置 QTimer + m_stepDelay。”
- `core/algorithms.cpp:L8-L8` “timeout 连接到 onTimerTimeout 触发下一步。”
- `core/algorithms.cpp:L46-L46` “scheduleNextStep 用 m_timer.start(m_stepDelay) 控制节拍。”
"@
  },
  @{
    File = "T__性能测量_chrono.md"
    Title = "性能测量（std::chrono 计时）"
    Course = @("408-OS/计组: 计时与测量误差（类比）")
    Keywords = @("chrono", "latency", "benchmark", "variance")
    Body = @"
## 1. 一句话定义（面试口径）
- 用 std::chrono 获取起止时间并计算 ms 级耗时，是做算法/工程性能对比的基础工具。

## 2. 核心机制（用自己的话讲清楚）
- now() 取时间点，duration 计算差值；多次运行取均值/方差更可靠。

## 3. 复杂度/代价（面试常问）
- 代价：计时本身有开销；UI 刷新会污染算法纯耗时；系统时钟精度影响结果。

## 4. 常见追问与答法（3~5条）
- Q: 如何做严谨 benchmark？
  A: 固定输入、重复多次、隔离 IO/渲染、记录统计量，并落盘保存原始数据。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__工程]]
- `widget.cpp:L632-L635` “对 BFS 运行区间计时并得到 bfsTime(ms)。”
- `core/algorithms.cpp:L512-L582` “compareAllAlgorithms 内部也记录 executionTime(ms)。"
"@
  },
  @{
    File = "T__栅格地图_Grid2D.md"
    Title = "栅格地图（Grid2D / APoint）"
    Course = @("408-DS: 数组/向量与复杂度")
    Keywords = @("grid", "occupancy", "vector", "shared_ptr")
    Body = @"
## 1. 一句话定义（面试口径）
- 栅格地图把环境离散成二维格子，用格子状态（墙/空/起终点等）表达可通行性。

## 2. 核心机制（用自己的话讲清楚）
- Grid2D 保存每个格子的 APoint：坐标、类型、代价、父指针；算法在这个结构上读写状态。

## 3. 复杂度/代价（面试常问）
- 访问：O(1)（按索引），遍历：O(HW)。
- 代价：二维 vector + 指针结构对缓存不友好；大规模下常数项偏大。

## 4. 常见追问与答法（3~5条）
- Q: 为什么不用连续数组？
  A: 连续数组更 cache-friendly；指针结构更灵活但性能常数更差。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__感知]] / [[M__规划]]
- `models/mazegrid.h:L8-L15` “AType 定义了 Wall/Start/End/Visited 等格子语义。”
- `models/mazegrid.h:L37-L37` “Grid2D = vector<vector<APointPtr>>。”
- `widget.cpp:L259-L267` “把 Grid2D 的 Wall/空转换为 0/1 maze 矩阵供 BFS/DFS/评测使用。”
"@
  },
  @{
    File = "T__文件持久化_QFile_QTextStream.md"
    Title = "文件持久化（QFile / QTextStream）"
    Course = @("408-OS: 文件系统/IO（类比）")
    Keywords = @("QFile", "QTextStream", "serialization")
    Body = @"
## 1. 一句话定义（面试口径）
- 用 QFile/QTextStream 把程序内部状态序列化为可读文本，实现复现实验与数据回放。

## 2. 核心机制（用自己的话讲清楚）
- 保存时逐行写出整数矩阵；加载时逐行读取并 split，再转 int 构建二维结构。

## 3. 复杂度/代价（面试常问）
- 时间：O(HW) 的读写 + 文本解析开销。
- 空间：文件体积随地图大小线性增长。
- 工程代价：要处理格式/版本兼容与错误输入。

## 4. 常见追问与答法（3~5条）
- Q: 为什么不用二进制？
  A: 文本更易调试；规模大时可换二进制并加版本号。

## 5. 在我项目里怎么用（证据链接）
- $projectLink / [[M__工程]]
- `services/fileservice.cpp:L11-L23` “saveMap：写出 0/1 矩阵文本。”
- `services/fileservice.cpp:L28-L46` “loadMap：读入并解析为二维 QVector<int>。”
"@
  }
)

foreach ($t in $topicCards) {
  $path = Join-Path $topicsDir $t.File
  $content = Topic-Card $t.Title $t.Course @($projectLink) $t.Keywords $t.Body
  Write-FileWithBackup $path $content
}

Write-Output ("Wrote project: {0}" -f $projectCardPath)
Write-Output ("Wrote modules: {0}" -f $modulesDir)
Write-Output ("Wrote QA: {0}" -f $qaPath)
Write-Output ("Wrote topics: {0}" -f $topicsDir)
