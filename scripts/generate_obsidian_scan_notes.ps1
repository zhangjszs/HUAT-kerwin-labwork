$ErrorActionPreference = 'Stop'

$vaultRoot = 'D:\laboratory\obsidianLibrary'
$today = '2026-02-19'

function Join-YamlInline {
    param([array]$Items)
    if ($null -eq $Items -or $Items.Count -eq 0) {
        return '[]'
    }
    $escaped = $Items | ForEach-Object { "'" + (($_ -as [string]) -replace "'", "''") + "'" }
    return '[' + ($escaped -join ', ') + ']'
}

function Bullets {
    param([array]$Items)
    if ($null -eq $Items -or $Items.Count -eq 0) {
        return '- 待补证据'
    }
    return (($Items | ForEach-Object { '- ' + $_ }) -join "`n")
}

function Merge-UserBlock {
    param(
        [string]$Existing,
        [string]$NewContent
    )
    if ($Existing -match '(?s)<!-- USER-START -->.*?<!-- USER-END -->') {
        $block = $matches[0]
        if ($NewContent -notmatch '(?s)<!-- USER-START -->.*?<!-- USER-END -->') {
            return $NewContent.TrimEnd() + "`r`n`r`n" + $block + "`r`n"
        }
    }
    return $NewContent
}

function Write-Note {
    param(
        [string]$Path,
        [string]$Content
    )
    $dir = Split-Path -Path $Path -Parent
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }

    if (Test-Path $Path) {
        $old = Get-Content -Path $Path -Raw
        Set-Content -Path ($Path + '.backup') -Value $old -Encoding UTF8
        $Content = Merge-UserBlock -Existing $old -NewContent $Content
    }

    Set-Content -Path $Path -Value $Content -Encoding UTF8
}

function New-ProjectCard {
    param([hashtable]$P)

    $topicLinks = $P.Topics | ForEach-Object { "[[T__$($_)]]" }
    $moduleLinks = $P.Modules | ForEach-Object { "[[M__$($_.Name)]]" }

    $content = @"
---
type: project
status: active
time: $today
direction: $($P.Direction)
scene: $($P.Scene)
repo: $($P.Repo)
stack: $(Join-YamlInline $P.Stack)
skills: $(Join-YamlInline $P.Skills)
topics: $(Join-YamlInline $topicLinks)
metrics:
  latency_ms: $($P.Metrics.latency_ms)
  hz: $($P.Metrics.hz)
  accuracy: $($P.Metrics.accuracy)
  error: $($P.Metrics.error)
highlights: $(Join-YamlInline $P.YamlHighlights)
evidence: $(Join-YamlInline $P.YamlEvidence)
risks: $(Join-YamlInline $P.YamlRisks)
tradeoffs: $(Join-YamlInline $P.YamlTradeoffs)
next_improve: $(Join-YamlInline $P.YamlNextImprove)
---

> 项目类型：$($P.Type)（置信度：$($P.Confidence)）
> 识别信号摘要：$($P.SignalSummary)

# $($P.Name)（Project Card）

## 1. 一句话简介
$(Bullets $P.OneLiner)

## 2. 我负责什么（边界要清晰）
$(Bullets $P.Responsibility)

## 3. 系统/模块总览（链接到模块卡）
$(Bullets $moduleLinks)

## 4. 核心技术亮点（可量化）
$(Bullets $P.CoreHighlights)

## 5. 关键证据链（我如何证明我做过、做对了）
$(Bullets $P.EvidenceChain)

## 6. 复试追问高危点（提前准备）
$(Bullets $P.HighRiskQuestions)

## 7. 下一步改进（面试可以讲"未来工作"）
$(Bullets $P.NextWork)
"@
    return $content.Trim() + "`r`n"
}

function New-ModuleCard {
    param(
        [hashtable]$P,
        [hashtable]$M
    )

    $topicLinks = $M.Topics | ForEach-Object { "[[T__$($_)]]" }

    $content = @"
---
type: module
status: draft
time: $today
project: [[P__$($P.Short)]]
layer: $($M.Layer)
inputs: $(Join-YamlInline $M.Inputs)
outputs: $(Join-YamlInline $M.Outputs)
interfaces: $(Join-YamlInline $M.Interfaces)
stack: $(Join-YamlInline $M.Stack)
topics: $(Join-YamlInline $topicLinks)
metrics:
  latency_ms: $($M.Metrics.latency_ms)
  hz: $($M.Metrics.hz)
evidence: $(Join-YamlInline $M.YamlEvidence)
risks: $(Join-YamlInline $M.YamlRisks)
tradeoffs: $(Join-YamlInline $M.YamlTradeoffs)
---

# $($M.Name)（Module）

## 1. 模块职责（只做什么，不做什么）
$(Bullets $M.Duty)

## 2. 输入输出与接口（话题/数据结构/文件）
$(Bullets $M.IO)

## 3. 关键算法/数据结构（链接到 Topic）
$(Bullets $topicLinks)
$(Bullets $M.Algorithms)

## 4. 性能与瓶颈
$(Bullets $M.Performance)

## 5. 证据（实验/日志/参数/截图）
$(Bullets $M.EvidenceLines)
"@
    return $content.Trim() + "`r`n"
}

function New-TopicCard {
    param([hashtable]$T)

    $projectLinks = $T.Projects | ForEach-Object { "[[P__$($_)]]" }
    $qaLines = @()
    foreach ($qa in $T.CommonQA) {
        $qaLines += "- Q: $($qa.Q)"
        $qaLines += "  A: $($qa.A)"
    }

    $content = @"
---
type: topic
status: draft
course_refs: $(Join-YamlInline $T.CourseRefs)
projects: $(Join-YamlInline $projectLinks)
keywords: $(Join-YamlInline $T.Keywords)
---

# $($T.Name)（Topic）

## 1. 一句话定义（面试口径）
$(Bullets $T.Definition)

## 2. 核心机制（用自己的话讲清楚）
$(Bullets $T.Mechanism)

## 3. 复杂度/代价（面试常问）
- 时间：$($T.CostTime)
- 空间：$($T.CostSpace)
- 工程代价：$($T.CostEngineering)

## 4. 常见追问与答法（3~5条）
$($qaLines -join "`n")

## 5. 在我项目里怎么用（证据链接）
$(Bullets $T.UsageLinks)
"@
    return $content.Trim() + "`r`n"
}

function Build-QAItems {
    param([hashtable]$P)

    $moduleNames = $P.Modules | ForEach-Object { $_.Name }
    $m1 = if ($moduleNames.Count -gt 0) { $moduleNames[0] } else { '核心模块' }
    $m2 = if ($moduleNames.Count -gt 1) { $moduleNames[1] } else { $m1 }
    $t1 = if ($P.Topics.Count -gt 0) { $P.Topics[0] } else { '关键主题' }
    $t2 = if ($P.Topics.Count -gt 1) { $P.Topics[1] } else { $t1 }

    $eArch = $P.QAEvidence.arch
    $eAlgo = $P.QAEvidence.algo
    $ePerf = $P.QAEvidence.perf
    $eTrade = $P.QAEvidence.trade
    $e408 = $P.QAEvidence.link408

    return @(
        @{ Q = "你如何分层讲解 $($P.Name)？"; A = ("我按「{0}」讲主链路，先说入口，再说核心处理，最后落到数据与工程层。" -f ($moduleNames -join ' -> ')); E = $eArch; T = "如果加新需求先改哪一层？[架构扩展]" },
        @{ Q = "为什么把 $m1 和 $m2 分开？"; A = "分层后职责更清晰，复试时可以把输入、处理、输出边界讲清楚。"; E = $eArch; T = "边界打破会带来什么耦合问题？[系统设计]" },
        @{ Q = "主流程从启动到结束的关键路径是什么？"; A = "先初始化配置或数据，再进入核心逻辑循环，最后做结果输出/持久化。"; E = $eArch; T = "异常中断时如何保证状态一致？[工程可靠性]" },

        @{ Q = "项目里最核心的算法点是哪个？"; A = "我重点讲 $t1 的实现路径：输入条件、计算规则、输出结果三段式。"; E = $eAlgo; T = "最坏复杂度和边界条件怎么回答？[算法细节]" },
        @{ Q = "为什么选择当前算法而不是替代方案？"; A = "当前方案实现成本低、可解释性强，适合课程项目与答辩演示。"; E = $eAlgo; T = "如果数据规模扩大到10倍怎么办？[算法选型]" },
        @{ Q = "你如何验证算法正确性？"; A = "我通过可复现样例和关键分支检查来验证，先看中间状态再看最终结果。"; E = $eAlgo; T = "有没有失败样例和修复记录？[调试能力]" },
        @{ Q = "第二个关键点 $t2 怎么讲？"; A = "会先定义问题，再讲机制和代价，最后结合代码中的函数/流程落地。"; E = $eAlgo; T = "能否手推一个最小示例？[白板推导]" },
        @{ Q = "算法中的防错设计有哪些？"; A = "我会强调输入合法性、边界条件和异常路径处理，避免错误扩散。"; E = $eAlgo; T = "如果防错失败，故障会在哪一层暴露？[故障定位]" },

        @{ Q = "当前性能瓶颈在哪？"; A = "瓶颈在高频循环和全量处理路径，数据量上来后响应时间会线性或平方上升。"; E = $ePerf; T = "有没有做基准测试数据？[性能测量]" },
        @{ Q = "你做过哪些性能优化尝试？"; A = "优先做低成本优化：减少重复计算、控制I/O频率、必要时降采样。"; E = $ePerf; T = "优化后怎么证明收益而非偶然？[实验设计]" },
        @{ Q = "如果要继续优化，你先做哪一步？"; A = "先补可量化指标和性能日志，再决定是改数据结构还是改算法。"; E = $ePerf; T = "指标阈值怎么定？[性能工程]" },

        @{ Q = "实现中最大工程权衡是什么？"; A = "在「易实现/易讲解」和「高性能/高扩展」之间做了偏教学场景的选择。"; E = $eTrade; T = "如果用于生产，你会怎么重构？[工程化]" },
        @{ Q = "为什么保留当前代码组织方式？"; A = "它能保证功能完整和复试可讲性，但我也明确了后续解耦方向。"; E = $eTrade; T = "重构优先级如何排序？[技术债治理]" },
        @{ Q = "你会如何向面试官解释「暂未实现」的部分？"; A = "直接说当前边界、风险和补齐计划，给出可执行落地路径。"; E = $eTrade; T = "补齐计划如何验收？[项目管理]" },

        @{ Q = "这个项目和 408 数据结构最强关联点是什么？"; A = "我会把核心结构与复杂度直接映射到 408 的查找/插入/删除与空间代价。"; E = $e408; T = "请给出复杂度并说明退化场景。[408-数据结构]" },
        @{ Q = "和 408 操作系统能怎么联动？"; A = "从文件I/O、资源管理、异常退出一致性来讲，能把理论落到实现细节。"; E = $e408; T = "如果并发访问发生会怎样处理？[408-操作系统]" },
        @{ Q = "和 408 计网/组成原理如何联动？"; A = "根据项目类型讲协议控制、队列机制或执行周期，并对应到代码证据。"; E = $e408; T = "只讲理论不讲实现会扣分在哪里？[408-综合追问]" },
        @{ Q = "你如何避免「背书式回答」？"; A = "每个概念都绑定项目证据路径，先讲我做了什么，再讲理论依据。"; E = $e408; T = "现场追问一条证据你能立刻定位吗？[408-证据链]" },
        @{ Q = "若面试官追问复杂度证明，你怎么回答？"; A = "按代码循环层次和数据结构操作拆解，给出最好/平均/最坏三档。"; E = $e408; T = "能否口算并写出推导步骤？[408-算法分析]" },
        @{ Q = "如果让你把项目改成一道408综合题，你怎么出题？"; A = "我会把项目流程拆成建模、复杂度、权衡三问，要求证据驱动回答。"; E = $e408; T = "出题后如何给标准答案评分点？[408-应试策略]" }
    )
}

function New-QACard {
    param([hashtable]$P)
    $topicLinks = $P.Topics | ForEach-Object { "[[T__$($_)]]" }
    $qaItems = Build-QAItems -P $P
    $sections = @()
    $idx = 1
    foreach ($item in $qaItems) {
        $sections += @"
## Q$idx
- **问：** $($item.Q)
- **答（口语化，30~60秒）：** $($item.A)
- **证据：** $($item.E)
- **追问陷阱：** $($item.T)
"@
        $idx++
    }

    $content = @"
---
type: interview
status: draft
scope: project
project: [[P__$($P.Short)]]
module: 
topics: $(Join-YamlInline $topicLinks)
---

# $($P.Name) 复试问答（20问20答）

> 目标：每题回答 30~60 秒；每题带 1 个"证据点"。

$($sections -join "`n")
"@
    return $content.Trim() + "`r`n"
}

# Topics and project data are appended below.
$topics = @(
    @{
        Name = '单链表记录管理'
        CourseRefs = @('408-数据结构')
        Projects = @('CCD')
        Keywords = @('链表', 'CRUD', '复杂度')
        Definition = @('用节点指针维护动态记录集合，适合课设增删改查。`c-course-design/vehicle_claim.h:27-30`')
        Mechanism = @('头插/尾插、按ID与VIN遍历查找、按前驱删除。`c-course-design/vehicle_claim.c:167-223`')
        CostTime = '头插O(1)，查询/删除定位O(n)。`c-course-design/vehicle_claim.c:167-213`'
        CostSpace = 'O(n)节点空间。`c-course-design/vehicle_claim.h:27-30`'
        CostEngineering = '无索引，数据量增长后查询退化。`c-course-design/vehicle_claim.c:252-260`'
        CommonQA = @(
            @{ Q = '为什么不用数组？'; A = '记录数不固定时链表增删更直接。`c-course-design/vehicle_claim.c:167-187`' },
            @{ Q = '删除如何防断链？'; A = '保存前驱节点后再重连并释放目标。`c-course-design/vehicle_claim.c:195-212`' },
            @{ Q = '性能瓶颈在哪？'; A = '多次线性遍历会在大数据下变慢。`c-course-design/vehicle_claim.c:215-260`' }
        )
        UsageLinks = @('[[P__CCD]] / [[M__索赔记录]]：核心数据结构。`c-course-design/vehicle_claim.c:141-273`')
    },
    @{
        Name = '冒泡排序复杂度'
        CourseRefs = @('408-排序')
        Projects = @('CCD')
        Keywords = @('冒泡排序', 'O(n^2)')
        Definition = @('通过两层循环比较相邻记录金额实现升序排序。`c-course-design/vehicle_claim.c:275-285`')
        Mechanism = @('若前者金额更大则交换节点数据。`c-course-design/vehicle_claim.c:278-282`')
        CostTime = 'O(n^2)。`c-course-design/vehicle_claim.c:275-285`'
        CostSpace = 'O(1)额外空间。`c-course-design/vehicle_claim.c:279-281`'
        CostEngineering = '可读性高但扩展性一般。`c-course-design/vehicle_claim.c:275-285`'
        CommonQA = @(
            @{ Q = '为什么选冒泡？'; A = '课设强调可解释性与快速实现。`c-course-design/vehicle_claim.c:275-285`' },
            @{ Q = '是否稳定？'; A = '只在大于时交换，相等值相对顺序不变。`c-course-design/vehicle_claim.c:278-282`' },
            @{ Q = '如何优化？'; A = '加早停标记或改更优排序（待补证据）。' }
        )
        UsageLinks = @('[[P__CCD]] / [[M__索赔记录]]：按金额排序展示。`c-course-design/vehicle_claim.c:274-285`')
    },
    @{
        Name = '文本文件持久化'
        CourseRefs = @('408-操作系统')
        Projects = @('CCD')
        Keywords = @('fopen', 'fscanf', '持久化')
        Definition = @('把链表记录写入文本并在启动时回读。`c-course-design/vehicle_claim.c:419-480`')
        Mechanism = @('写文件先表头后记录，读文件跳过表头逐行插入。`c-course-design/vehicle_claim.c:432-477`')
        CostTime = '读写均O(n)。`c-course-design/vehicle_claim.c:434-439`'
        CostSpace = 'O(1)额外缓冲。`c-course-design/vehicle_claim.c:463-474`'
        CostEngineering = '可读但缺事务与并发保护。`c-course-design/vehicle_claim.c:425-460`'
        CommonQA = @(
            @{ Q = '为什么文本格式？'; A = '便于课程演示与人工校验。`c-course-design/vehicle_claim.c:432-437`' },
            @{ Q = '文件不存在怎么办？'; A = '自动创建并写入表头。`c-course-design/vehicle_claim.c:452-460`' },
            @{ Q = '并发写入如何处理？'; A = '当前未实现文件锁（待补证据）。' }
        )
        UsageLinks = @('[[P__CCD]] / [[M__文件持久化]]：启动读档、退出落盘。`c-course-design/vehicle_claim.c:698-699`')
    },
    @{
        Name = '词法分析器'
        CourseRefs = @('编译原理', '408-自动机')
        Projects = @('CPR')
        Keywords = @('Getsym', 'token', '词法单元')
        Definition = @('按字符流识别标识符、数字、符号等token。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`')
        Mechanism = @('空白过滤+分支识别+必要回退，最终给出sym/token/num。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:93-133`')
        CostTime = '线性O(n)。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`'
        CostSpace = 'O(1)状态量。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:32-37`'
        CostEngineering = '错误恢复能力有限。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:86-88`'
        CommonQA = @(
            @{ Q = '如何识别保留字？'; A = 'token转大写后查WORD表。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:69-84`' },
            @{ Q = '为什么要Retract？'; A = '超前读取后需回退一个字符。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:57-60`' },
            @{ Q = '错误如何定位？'; A = 'row/col会随读取动态更新。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:36-55`' }
        )
        UsageLinks = @('[[P__CPR]] / [[M__词法分析]]：主循环持续调用Getsym。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:55-69`')
    },
    @{
        Name = '保留字识别'
        CourseRefs = @('编译原理')
        Projects = @('CPR')
        Keywords = @('WORD', 'WSYM', 'Reserve')
        Definition = @('WORD字符串表映射到WSYM种别码表。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:10-20`')
        Mechanism = @('Reserve遍历WORD，命中返回索引，否则-1。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:69-84`')
        CostTime = 'O(W)查表，W=14。`compiler-principles/Code/Grammar_analysis/include/symbol.h:7`'
        CostSpace = 'O(W)常量表。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:10-20`'
        CostEngineering = '线性查找可替换哈希（待补证据）。'
        CommonQA = @(
            @{ Q = '为什么先统一大小写？'; A = '避免大小写差异影响保留字识别。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:74-76`' },
            @{ Q = '未命中怎么处理？'; A = '按一般标识符IDENT处理。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:107-111`' },
            @{ Q = '表规模在哪里定义？'; A = 'WORDLEN=14。`compiler-principles/Code/Grammar_analysis/include/symbol.h:7`' }
        )
        UsageLinks = @('[[P__CPR]] / [[M__词法分析]]：决定token分类。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:106-110`')
    },
    @{
        Name = '递归下降表达式求值'
        CourseRefs = @('编译原理', '408-算法')
        Projects = @('CPR')
        Keywords = @('parseExpression', '优先级', '递归')
        Definition = @('Expression->Term->Factor 分层递归实现四则运算。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:61-106`')
        Mechanism = @('Factor处理数字和括号，Term处理乘除，Expression处理加减。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:42-105`')
        CostTime = '线性O(n)。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:109-121`'
        CostSpace = '递归深度O(d)。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:47-55`'
        CostEngineering = '深嵌套存在栈深风险（待补证据）。'
        CommonQA = @(
            @{ Q = '如何保证优先级？'; A = '分层函数调用天然编码优先级。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:61-105`' },
            @{ Q = '除零如何处理？'; A = '直接抛运行时异常。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:76-79`' },
            @{ Q = '尾部脏字符怎么查？'; A = 'calculate结尾检查pos。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:115-119`' }
        )
        UsageLinks = @('[[P__CPR]] / [[M__表达式求值]]：读取b.txt后调用求值接口。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:70-82`')
    },
    @{
        Name = 'RED算法'
        CourseRefs = @('计算机网络', '408-传输层')
        Projects = @('CNET')
        Keywords = @('RED', '阈值', '丢包概率')
        Definition = @('RED按队列阈值分段计算丢包概率。`computer-network/Improve_RED/src/red_simulator.py:13-20`')
        Mechanism = @('低于min_th不丢包，高于max_th满概率，中间线性增长。`computer-network/Improve_RED/src/red_simulator.py:13-20`')
        CostTime = '单次O(1)。`computer-network/Improve_RED/src/red_simulator.py:12-20`'
        CostSpace = 'O(1)状态。`computer-network/Improve_RED/src/red_simulator.py:6-10`'
        CostEngineering = '参数敏感，需要实验校准。`computer-network/Improve_RED/src/red_simulator.py:48-50`'
        CommonQA = @(
            @{ Q = '关键参数有哪些？'; A = 'min_th、max_th、max_p。`computer-network/Improve_RED/src/red_simulator.py:6-10`' },
            @{ Q = '为什么要早期随机丢包？'; A = '提前反馈拥塞，降低队列爆满风险。`computer-network/Improve_RED/README.md:20-23`' },
            @{ Q = '阈值反转怎么防？'; A = '回调里强制max_th>min_th。`computer-network/Improve_RED/src/red_simulator.py:59-63`' }
        )
        UsageLinks = @('[[P__CNET]] / [[M__RED_ARED基线]]：基线概率模型。`computer-network/Improve_RED/src/red_simulator.py:31-75`')
    },
    @{
        Name = 'ARED算法'
        CourseRefs = @('计算机网络', '408-拥塞控制')
        Projects = @('CNET')
        Keywords = @('ARED', 'alpha', 'beta')
        Definition = @('ARED会动态调整max_p以适应队列波动。`computer-network/Improve_RED/src/ared_simulator.py:24-33`')
        Mechanism = @('队列过低衰减max_p，过高增大max_p，再做区间夹紧。`computer-network/Improve_RED/src/ared_simulator.py:25-33`')
        CostTime = '单步O(1)。`computer-network/Improve_RED/src/ared_simulator.py:24-33`'
        CostSpace = 'O(1)。`computer-network/Improve_RED/src/ared_simulator.py:6-12`'
        CostEngineering = 'alpha/beta选取影响稳定性。`computer-network/Improve_RED/src/ared_simulator.py:7-8`'
        CommonQA = @(
            @{ Q = 'alpha和beta含义？'; A = '分别控制减小和增大max_p的强度。`computer-network/Improve_RED/src/ared_simulator.py:7-8`' },
            @{ Q = '为什么夹紧max_p？'; A = '避免概率越界。`computer-network/Improve_RED/src/ared_simulator.py:32-33`' },
            @{ Q = 'ARED对比RED一句话？'; A = 'ARED多了自适应调参环。`computer-network/Improve_RED/src/ared_simulator.py:24-33`' }
        )
        UsageLinks = @('[[P__CNET]] / [[M__RED_ARED基线]]：自适应概率基线。`computer-network/Improve_RED/src/ared_simulator.py:24-33`')
    },
    @{
        Name = 'S型丢包概率'
        CourseRefs = @('计算机网络', '概率统计')
        Projects = @('CNET')
        Keywords = @('Sigmoid', 'S型函数', '改进RED')
        Definition = @('改进RED通过S型函数构造非线性丢包概率。`computer-network/Improve_RED/src/improved_red_simulator.py:25-33`')
        Mechanism = @('在阈值区间内按beta控制曲线斜率。`computer-network/Improve_RED/src/improved_red_simulator.py:16-33`')
        CostTime = '单次O(1)。`computer-network/Improve_RED/src/improved_red_simulator.py:18-34`'
        CostSpace = 'O(1)。`computer-network/Improve_RED/src/improved_red_simulator.py:6-16`'
        CostEngineering = '参数解释成本高于线性模型。`computer-network/Improve_RED/README.md:8-11`'
        CommonQA = @(
            @{ Q = '为什么用S型？'; A = '中段更平滑，减少突变。`computer-network/Improve_RED/README.md:8-11`' },
            @{ Q = 'beta调大会怎样？'; A = '曲线更陡，反馈更激进。`computer-network/Improve_RED/src/improved_red_simulator.py:16`' },
            @{ Q = '上界条件是什么？'; A = '队列达到2*max_th后概率置1。`computer-network/Improve_RED/src/improved_red_simulator.py:22-23`' }
        )
        UsageLinks = @('[[P__CNET]] / [[M__改进RED]]：改进概率函数核心。`computer-network/Improve_RED/src/improved_red_simulator.py:18-34`')
    },
    @{
        Name = '离散队列仿真'
        CourseRefs = @('408-排队', '计算机网络')
        Projects = @('CNET')
        Keywords = @('泊松到达', '离散时间', '队列长度')
        Definition = @('按时间步模拟到达、服务和丢包，记录队列轨迹。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-145`')
        Mechanism = @('每步采样新包与服务完成包，再用算法概率决定丢弃。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113-140`')
        CostTime = 'O(T)。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:101-112`'
        CostSpace = 'O(T)存储轨迹。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:104-106`'
        CostEngineering = '随机性高，需多轮统计（待补证据）。'
        CommonQA = @(
            @{ Q = '为什么用泊松？'; A = '常用于随机到达过程建模。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113`' },
            @{ Q = '比较了哪几种算法？'; A = 'RED、ARED、改进RED。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:104-106`' },
            @{ Q = '输出图怎么看？'; A = '看三条队列曲线和阈值线相对位置。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:148-154`' }
        )
        UsageLinks = @('[[P__CNET]] / [[M__仿真执行]]：统一对比框架。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-161`')
    },
    @{
        Name = '指令执行周期'
        CourseRefs = @('408-组成原理')
        Projects = @('CORG')
        Keywords = @('取指', '译码', '执行')
        Definition = @('待补证据：需从实验文档转写后再固化定义。')
        Mechanism = @('待补证据：建议先建立 `computer-organization/Evidence/实验文本转写.md`。')
        CostTime = '待补证据'
        CostSpace = '待补证据'
        CostEngineering = '待补证据'
        CommonQA = @(
            @{ Q = '单周期与多周期怎么讲？'; A = '待补证据。' },
            @{ Q = '关键路径怎么识别？'; A = '待补证据。' },
            @{ Q = '如何落到实验现象？'; A = '待补证据。' }
        )
        UsageLinks = @('[[P__CORG]] / [[M__实验复盘]]：待补证据。')
    },
    @{
        Name = 'Cache映射策略'
        CourseRefs = @('408-组成原理')
        Projects = @('CORG')
        Keywords = @('直接映射', '组相联', '命中率')
        Definition = @('待补证据：需补实验参数与结果。')
        Mechanism = @('待补证据：需补地址分解与替换策略说明。')
        CostTime = '待补证据'
        CostSpace = '待补证据'
        CostEngineering = '待补证据'
        CommonQA = @(
            @{ Q = '映射策略如何权衡？'; A = '待补证据。' },
            @{ Q = '命中率如何测量？'; A = '待补证据。' },
            @{ Q = '如何给出工程结论？'; A = '待补证据。' }
        )
        UsageLinks = @('[[P__CORG]] / [[M__证据补全]]：待补证据。')
    },
    @{
        Name = '流水线冒险'
        CourseRefs = @('408-组成原理')
        Projects = @('CORG')
        Keywords = @('数据冒险', '控制冒险', '旁路')
        Definition = @('待补证据：需补实验流程与时序图。')
        Mechanism = @('待补证据：需补暂停与旁路策略记录。')
        CostTime = '待补证据'
        CostSpace = '待补证据'
        CostEngineering = '待补证据'
        CommonQA = @(
            @{ Q = 'RAW/WAR/WAW区别？'; A = '待补证据。' },
            @{ Q = '旁路如何降低停顿？'; A = '待补证据。' },
            @{ Q = '分支失败代价怎么算？'; A = '待补证据。' }
        )
        UsageLinks = @('[[P__CORG]] / [[M__实验复盘]]：待补证据。')
    }
)
$projects = @(
    @{
        Short = 'CCD'
        Name = 'C语言课程设计：车辆索赔管理系统'
        Type = '通用/其他'
        Confidence = '低'
        SignalSummary = 'CMakeLists.txt×1；未发现 package.json/requirements.txt/ipynb/ROS 配置。'
        Direction = '计算机基础课程'
        Scene = '课程设计-车辆索赔管理'
        Repo = 'c-course-design'
        Stack = @('C99', 'CMake', '单链表', '文件I/O')
        Skills = @('链表CRUD', '日期校验', '菜单交互')
        Topics = @('单链表记录管理', '冒泡排序复杂度', '文本文件持久化')
        Metrics = @{ latency_ms = '待补证据'; hz = '待补证据'; accuracy = '待补证据'; error = '待补证据' }
        YamlHighlights = @('菜单8个功能入口。`c-course-design/vehicle_claim.c:125-133`', '链表CRUD完整。`c-course-design/vehicle_claim.c:141-273`', '文件读写闭环。`c-course-design/vehicle_claim.c:419-480`')
        YamlEvidence = @('c-course-design/README.md:3-9', 'c-course-design/CMakeLists.txt:1-18', 'c-course-design/vehicle_claim.c:684-705')
        YamlRisks = @('全局状态耦合。`c-course-design/vehicle_claim.c:30-33`', 'system调用跨平台弱。`c-course-design/vehicle_claim.c:703-705`')
        YamlTradeoffs = @('头插法快但顺序不直观。`c-course-design/vehicle_claim.c:636-637`', '文本可读但容错弱。`c-course-design/vehicle_claim.c:425-460`')
        YamlNextImprove = @('补性能基准。', '补自动化测试。', '补异常恢复策略。')
        OneLiner = @('C99控制台系统，单链表管理索赔记录并落盘到文本文件。`c-course-design/README.md:3-3` `c-course-design/vehicle_claim.c:419-480`')
        Responsibility = @('负责菜单、链表、统计、文件读写全链路。`c-course-design/vehicle_claim.c:505-681`', '不涉及网络服务和数据库事务。`c-course-design/vehicle_claim.c:684-705`')
        CoreHighlights = @('ID重复检查+日期合法性校验。`c-course-design/vehicle_claim.c:618-627`', '统计支持3维度。`c-course-design/vehicle_claim.c:640-667`', '退出前自动保存。`c-course-design/vehicle_claim.c:670-672`')
        EvidenceChain = @('启动读档。`c-course-design/vehicle_claim.c:698-699`', '删除后立即写档。`c-course-design/vehicle_claim.c:605-610`', '释放链表内存。`c-course-design/vehicle_claim.c:714-725`')
        HighRiskQuestions = @('链表复杂度如何证明？（408-数据结构）`c-course-design/vehicle_claim.c:215-260`', '排序为何选冒泡？（408-排序）`c-course-design/vehicle_claim.c:275-285`', '文件一致性怎么保证？（408-操作系统）`c-course-design/vehicle_claim.c:425-460`')
        NextWork = @('记录1k/10k数据下查询耗时。待补证据', '补写失败回滚。待补证据', '拆分keyDown分支。待补证据')
        QAEvidence = @{
            arch = 'c-course-design/vehicle_claim.c:684-705'
            algo = 'c-course-design/vehicle_claim.c:141-417'
            perf = 'c-course-design/vehicle_claim.c:215-285'
            trade = 'c-course-design/vehicle_claim.c:425-460'
            link408 = 'c-course-design/vehicle_claim.c:141-417'
        }
        Modules = @(
            @{
                Name = '交互菜单'
                Layer = '接口层'
                Inputs = @('菜单编号', '字段输入')
                Outputs = @('业务分支调用')
                Interfaces = @('makeMenu()', 'keyDown()')
                Stack = @('C99', 'stdio')
                Topics = @('单链表记录管理')
                Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }
                YamlEvidence = @('c-course-design/vehicle_claim.c:121-135', 'c-course-design/vehicle_claim.c:505-681')
                YamlRisks = @('分支过长。`c-course-design/vehicle_claim.c:513-680`')
                YamlTradeoffs = @('集中式路由易讲解但耦合高。`c-course-design/vehicle_claim.c:513-680`')
                Duty = @('负责菜单展示与输入分发。`c-course-design/vehicle_claim.c:121-133`')
                IO = @('输入：键盘命令。`c-course-design/vehicle_claim.c:512-523`', '输出：提示与结果。`c-course-design/vehicle_claim.c:528-535`', '关键接口：`makeMenu`/`keyDown`。`c-course-design/vehicle_claim.c:121-135`')
                Algorithms = @('switch分发表驱动业务流程。`c-course-design/vehicle_claim.c:513-680`')
                Performance = @('当前：交互性能受业务分支影响。`c-course-design/vehicle_claim.c:520-668`', '瓶颈：超长switch。`c-course-design/vehicle_claim.c:513-680`', '优化：命令表重构。待补证据')
                EvidenceLines = @('菜单定义：`c-course-design/vehicle_claim.c:125-133`', '分发逻辑：`c-course-design/vehicle_claim.c:513-680`')
            },
            @{
                Name = '索赔记录'
                Layer = '业务层'
                Inputs = @('ID', 'VIN', '金额', '日期')
                Outputs = @('查询结果', '统计结果')
                Interfaces = @('insertNodeByHead()', 'searchByCarID()', 'bubbleSortList()')
                Stack = @('单链表', '字符串比较')
                Topics = @('单链表记录管理', '冒泡排序复杂度')
                Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }
                YamlEvidence = @('c-course-design/vehicle_claim.c:141-417')
                YamlRisks = @('线性查找退化。`c-course-design/vehicle_claim.c:215-260`')
                YamlTradeoffs = @('实现简单但复杂度高。`c-course-design/vehicle_claim.c:275-285`')
                Duty = @('负责增删改查、统计、排序。`c-course-design/vehicle_claim.c:141-417`')
                IO = @('输入：菜单层条件。`c-course-design/vehicle_claim.c:520-667`', '输出：记录指针/金额。`c-course-design/vehicle_claim.c:223-224` `c-course-design/vehicle_claim.c:384-417`', '关键接口：CRUD与统计函数。`c-course-design/vehicle_claim.h:39-64`')
                Algorithms = @('遍历查找+冒泡排序。`c-course-design/vehicle_claim.c:215-285`')
                Performance = @('当前：查找O(n)、排序O(n^2)。`c-course-design/vehicle_claim.c:215-285`', '瓶颈：重复遍历。`c-course-design/vehicle_claim.c:372-417`', '优化：索引化。待补证据')
                EvidenceLines = @('CRUD：`c-course-design/vehicle_claim.c:141-273`', '统计：`c-course-design/vehicle_claim.c:372-417`')
            },
            @{
                Name = '文件持久化'
                Layer = '数据层'
                Inputs = @('链表', 'ClimeData.txt')
                Outputs = @('文本记录', '回读链表')
                Interfaces = @('saveInfoFile()', 'readInfoFile()')
                Stack = @('stdio')
                Topics = @('文本文件持久化')
                Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }
                YamlEvidence = @('c-course-design/vehicle_claim.c:419-481')
                YamlRisks = @('缺文件锁。`c-course-design/vehicle_claim.c:425-443`')
                YamlTradeoffs = @('文本可读但脆弱。`c-course-design/vehicle_claim.c:432-437`')
                Duty = @('负责程序状态落盘与回档。`c-course-design/vehicle_claim.c:419-481`')
                IO = @('输入：链表记录。`c-course-design/vehicle_claim.c:431-439`', '输出：制表符文本。`c-course-design/vehicle_claim.c:432-437`', '关键接口：save/read。`c-course-design/vehicle_claim.h:66-67`')
                Algorithms = @('顺序遍历写入+格式化读取。`c-course-design/vehicle_claim.c:434-477`')
                Performance = @('当前：全量I/O O(n)。`c-course-design/vehicle_claim.c:434-439`', '瓶颈：每次修改后全量写。`c-course-design/vehicle_claim.c:609-610`', '优化：增量日志。待补证据')
                EvidenceLines = @('写文件：`c-course-design/vehicle_claim.c:425-443`', '读文件：`c-course-design/vehicle_claim.c:451-480`')
            },
            @{
                Name = '构建运行'
                Layer = '工程层'
                Inputs = @('CMake配置', '源码')
                Outputs = @('VehicleClaimManagement')
                Interfaces = @('CMakeLists.txt', 'main()')
                Stack = @('CMake', 'GCC/MinGW')
                Topics = @('文本文件持久化')
                Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }
                YamlEvidence = @('c-course-design/CMakeLists.txt:1-18', 'c-course-design/vehicle_claim.c:684-705')
                YamlRisks = @('缺少测试目标。`c-course-design/CMakeLists.txt:17-18`')
                YamlTradeoffs = @('配置简洁但扩展性一般。`c-course-design/CMakeLists.txt:10-18`')
                Duty = @('负责构建入口与主循环。`c-course-design/CMakeLists.txt:13-18` `c-course-design/vehicle_claim.c:700-705`')
                IO = @('输入：CMake配置。`c-course-design/CMakeLists.txt:10-12`', '输出：可执行程序。`c-course-design/CMakeLists.txt:18-18`', '关键接口：`main`。`c-course-design/vehicle_claim.c:684-711`')
                Algorithms = @('流程编排为主。`c-course-design/vehicle_claim.c:684-705`')
                Performance = @('当前：交互循环稳定。`c-course-design/vehicle_claim.c:700-705`', '瓶颈：无自动化验证。待补证据', '优化：CI+CTest。待补证据')
                EvidenceLines = @('构建目标：`c-course-design/CMakeLists.txt:13-18`', '主流程：`c-course-design/vehicle_claim.c:684-705`')
            }
        )
    },
    @{
        Short = 'CPR'
        Name = '编译原理课程实验：词法分析与表达式求值'
        Type = '通用/其他'
        Confidence = '低'
        SignalSummary = 'CMakeLists.txt×1；核心是编译原理实验代码，未命中Web/AI/ROS特征。'
        Direction = '编译原理课程'
        Scene = '课程实验-前端核心机制'
        Repo = 'compiler-principles'
        Stack = @('C++20', 'CMake', '词法分析', '递归下降')
        Skills = @('token识别', '保留字映射', '异常处理')
        Topics = @('词法分析器', '保留字识别', '递归下降表达式求值')
        Metrics = @{ latency_ms = '待补证据'; hz = '待补证据'; accuracy = '待补证据'; error = '待补证据' }
        YamlHighlights = @('35种词法种别码。`compiler-principles/Code/Grammar_analysis/include/symbol.h:9-19`', 'Getsym覆盖主扫描路径。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:96-191`', '表达式支持括号和四则。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:42-106`')
        YamlEvidence = @('compiler-principles/README.md:3-4', 'compiler-principles/Code/Grammar_analysis/CMakeLists.txt:13-17', 'compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:55-84')
        YamlRisks = @('错误恢复弱。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:86-88`', '文件名硬编码。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:26-34`')
        YamlTradeoffs = @('表驱动简单但线性查找。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:77-83`', '递归下降清晰但深度风险。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:47-55`')
        YamlNextImprove = @('补词法回归测试。', '参数化输入文件。', '补性能指标。')
        OneLiner = @('实验把词法扫描与表达式求值串联成可运行前端。`compiler-principles/README.md:3-4` `compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:55-84`')
        Responsibility = @('负责核心代码链路梳理与复试讲解口径。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`', '不包含完整语义分析和后端生成。`compiler-principles/README.md:3-4`')
        CoreHighlights = @('词法结果双通道输出。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:53-67`', '表达式异常可见。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:76-79`', 'CMake统一构建。`compiler-principles/Code/Grammar_analysis/CMakeLists.txt:13-17`')
        EvidenceChain = @('主循环调用Getsym。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:55-69`', '保留字双表映射。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:10-20`', '对外求值接口明确。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:124-132`')
        HighRiskQuestions = @('Getsym的状态转换如何证明？（408-自动机）`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:96-191`', '优先级由什么保证？（408-编译原理）`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:61-106`', '错误恢复怎么增强？（工程）`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:86-88`')
        NextWork = @('补黄金样本测试。待补证据', '统一I/O风格。待补证据', '补批处理模式。待补证据')
        QAEvidence = @{ arch = 'compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:48-95'; algo = 'compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195'; perf = 'compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:53-67'; trade = 'compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:69-84'; link408 = 'compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:61-106' }
        Modules = @(
            @{ Name = '输入读取'; Layer = '接口层'; Inputs = @('b.txt', '输出文件mydata.dat'); Outputs = @('字符流', '表达式字符串'); Interfaces = @('Init()', 'Quit()'); Stack = @('FILE*', 'ifstream'); Topics = @('词法分析器'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:21-45', 'compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:70-78'); YamlRisks = @('输入硬编码。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:26-27`'); YamlTradeoffs = @('简单但可配置性弱。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:23-33`'); Duty = @('负责文件打开与生命周期管理。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:21-45`'); IO = @('输入：b.txt。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:26-27`', '输出：fd/fout与expression。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:33-34` `compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:76-78`', '关键接口：Init/Quit。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:21-45`'); Algorithms = @('I/O编排为主。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:21-45`'); Performance = @('当前：串行I/O。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:53-69`', '瓶颈：同步打印。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:60-61`', '优化：缓冲输出。待补证据'); EvidenceLines = @('文件打开：`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:26-39`') },
            @{ Name = '词法分析'; Layer = '词法层'; Inputs = @('字符ch', 'WORD/WSYM'); Outputs = @('sym/token/num'); Interfaces = @('Getsym()', 'Reserve()', 'Errorsym()'); Stack = @('字符串处理', '枚举'); Topics = @('词法分析器', '保留字识别'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195'); YamlRisks = @('错误恢复不足。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:86-88`'); YamlTradeoffs = @('线性查找便于讲解。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:77-83`'); Duty = @('负责token识别和种别码输出。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`'); IO = @('输入：字符流。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:40-55`', '输出：sym/token。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:32-34`', '关键接口：Getsym。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`'); Algorithms = @('分支扫描+回退。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:96-133`'); Performance = @('当前：O(n)扫描。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`', '瓶颈：字符串拼接对比。`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:62-83`', '优化：哈希保留字。待补证据'); EvidenceLines = @('词法主流程：`compiler-principles/Code/Grammar_analysis/src/SYMBOL.CPP:90-195`') },
            @{ Name = '表达式求值'; Layer = '语法层'; Inputs = @('表达式字符串'); Outputs = @('double结果/异常'); Interfaces = @('evaluateExpression()', 'calculate()'); Stack = @('递归下降'); Topics = @('递归下降表达式求值'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:42-133'); YamlRisks = @('深递归风险（待补证据）'); YamlTradeoffs = @('结构清晰但扩展语法需加函数。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:87-105`'); Duty = @('负责四则表达式解析和计算。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:109-122`'); IO = @('输入：一行表达式。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:76-78`', '输出：计算结果。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:81-84`', '关键接口：evaluateExpression。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:124-132`'); Algorithms = @('Expression-Term-Factor递归层次。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:61-106`'); Performance = @('当前：O(n)。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:109-121`', '瓶颈：异常路径成本。`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:129-132`', '优化：预token化。待补证据'); EvidenceLines = @('括号处理：`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:47-55`', '除零处理：`compiler-principles/Code/Grammar_analysis/src/ExpressionCalculator.cpp:76-79`') },
            @{ Name = '构建输出'; Layer = '工程层'; Inputs = @('CMake配置', 'src源文件'); Outputs = @('Grammar_analysis可执行文件'); Interfaces = @('CMakeLists.txt', 'main()'); Stack = @('CMake'); Topics = @('词法分析器'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('compiler-principles/Code/Grammar_analysis/CMakeLists.txt:1-17'); YamlRisks = @('未区分构建模式。待补证据'); YamlTradeoffs = @('配置简单，上手快。`compiler-principles/Code/Grammar_analysis/CMakeLists.txt:13-17`'); Duty = @('负责工程编译入口。`compiler-principles/Code/Grammar_analysis/CMakeLists.txt:13-17`'); IO = @('输入：3个源文件。`compiler-principles/Code/Grammar_analysis/CMakeLists.txt:14-16`', '输出：可执行目标。`compiler-principles/Code/Grammar_analysis/CMakeLists.txt:13-17`', '关键接口：main。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:48-95`'); Algorithms = @('流程组织。'); Performance = @('当前：输出串行。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:53-67`', '瓶颈：打印开销。`compiler-principles/Code/Grammar_analysis/src/TESTSYM.CPP:60-61`', '优化：批量写。待补证据'); EvidenceLines = @('构建配置：`compiler-principles/Code/Grammar_analysis/CMakeLists.txt:13-17`') }
        )
    },
    @{
        Short = 'CNET'
        Name = '计算机网络课程实验：RED/ARED拥塞控制仿真'
        Type = '数据分析/可视化'
        Confidence = '高'
        SignalSummary = 'matplotlib/numpy仿真脚本命中≥6；README明确「模拟和可视化不同RED算法」。'
        Direction = '计算机网络课程'
        Scene = '拥塞控制算法仿真'
        Repo = 'computer-network'
        Stack = @('Python', 'NumPy', 'Matplotlib', '队列仿真')
        Skills = @('RED建模', 'ARED自适应', '可视化调参')
        Topics = @('RED算法', 'ARED算法', 'S型丢包概率', '离散队列仿真')
        Metrics = @{ latency_ms = '待补证据'; hz = '待补证据'; accuracy = '待补证据'; error = '待补证据' }
        YamlHighlights = @('三算法同框对比。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:125-133`', '支持滑条调参。`computer-network/Improve_RED/src/red_simulator.py:42-75`', '改进RED含S型+自适应。`computer-network/Improve_RED/src/improved_red_simulator.py:18-47`')
        YamlEvidence = @('computer-network/README.md:3-9', 'computer-network/Improve_RED/README.md:3-40', 'computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-161')
        YamlRisks = @('随机波动大。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113-118`', '缺统一配置。待补证据')
        YamlTradeoffs = @('演示友好但工程化不足。`computer-network/Improve_RED/README.md:3-4`', '多脚本复用性一般。`computer-network/Improve_RED/README.md:7-40`')
        YamlNextImprove = @('固定随机种子。', '补批量实验脚本。', '补量化指标落盘。')
        OneLiner = @('以RED家族算法为核心，做拥塞控制仿真与可视化对比。`computer-network/Improve_RED/README.md:3-4`')
        Responsibility = @('负责算法脚本结构拆解、参数口径统一和复试问答映射。`computer-network/Improve_RED/README.md:7-40`', '不涉及真实路由器线上压测。待补证据')
        CoreHighlights = @('仿真循环含到达/服务/丢包完整链路。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113-140`', 'max_p动态调整可视化。`computer-network/Improve_RED/src/improved_red_simulator.py:81-97`', '交互参数回调实时重绘。`computer-network/Improve_RED/src/improved_red_simulator.py:110-145`')
        EvidenceChain = @('RED分段概率实现清晰。`computer-network/Improve_RED/src/red_simulator.py:13-20`', 'ARED更新规则明确。`computer-network/Improve_RED/src/ared_simulator.py:24-33`', 'S型函数公式落地。`computer-network/Improve_RED/src/improved_red_simulator.py:25-33`')
        HighRiskQuestions = @('RED与ARED差异怎么量化？（408-传输层）`computer-network/Improve_RED/src/ared_simulator.py:24-33`', 'S型函数优势是什么？（概率建模）`computer-network/Improve_RED/src/improved_red_simulator.py:25-33`', '如何保障可复现？（实验设计）`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113-118`')
        NextWork = @('补seed与多轮平均。待补证据', '补吞吐/丢包/队列长度指标。待补证据', '统一配置文件。待补证据')
        QAEvidence = @{ arch = 'computer-network/Improve_RED/README.md:7-40'; algo = 'computer-network/Improve_RED/src/improved_red_simulator.py:18-47'; perf = 'computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-145'; trade = 'computer-network/Improve_RED/src/red_simulator.py:42-75'; link408 = 'computer-network/Improve_RED/src/red_queue_simulation_visualization.py:125-133' }
        Modules = @(
            @{ Name = '参数生成'; Layer = '数据输入层'; Inputs = @('阈值参数', '滑条事件'); Outputs = @('算法参数样本'); Interfaces = @('Slider.on_changed', 'np.linspace'); Stack = @('NumPy', 'Matplotlib widgets'); Topics = @('RED算法', 'ARED算法'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('computer-network/Improve_RED/src/red_simulator.py:31-75'); YamlRisks = @('参数口径分散。待补证据'); YamlTradeoffs = @('交互快但批处理弱。`computer-network/Improve_RED/src/red_simulator.py:42-75`'); Duty = @('负责参数初始化与更新，不定义核心公式。`computer-network/Improve_RED/src/red_simulator.py:53-57`'); IO = @('输入：滑条值。`computer-network/Improve_RED/src/red_simulator.py:48-50`', '输出：更新后的参数。`computer-network/Improve_RED/src/red_simulator.py:55-57`', '关键接口：on_changed。`computer-network/Improve_RED/src/red_simulator.py:72-74`'); Algorithms = @('参数合法性修正。`computer-network/Improve_RED/src/red_simulator.py:59-63`'); Performance = @('当前：更新即重算。`computer-network/Improve_RED/src/red_simulator.py:64-67`', '瓶颈：高采样重绘。待补证据', '优化：局部刷新。待补证据'); EvidenceLines = @('参数与回调：`computer-network/Improve_RED/src/red_simulator.py:42-75`') },
            @{ Name = 'RED_ARED基线'; Layer = '算法策略层'; Inputs = @('avg_queue_length', '阈值'); Outputs = @('基线丢包概率'); Interfaces = @('red_algorithm()', 'ared_algorithm()'); Stack = @('Python函数建模'); Topics = @('RED算法', 'ARED算法'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('computer-network/Improve_RED/src/red_queue_simulation_visualization.py:4-37', 'computer-network/Improve_RED/src/ared_simulator.py:24-33'); YamlRisks = @('参数敏感。`computer-network/Improve_RED/src/ared_simulator.py:7-8`'); YamlTradeoffs = @('可解释性高。`computer-network/Improve_RED/src/ared_simulator.py:24-33`'); Duty = @('负责传统RED与ARED概率路径。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:4-37`'); IO = @('输入：队列长度。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:4-20`', '输出：概率值。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:13-19`', '关键接口：update_max_p。`computer-network/Improve_RED/src/ared_simulator.py:24-33`'); Algorithms = @('分段线性+自适应max_p。`computer-network/Improve_RED/src/ared_simulator.py:24-33`'); Performance = @('当前：单步O(1)。`computer-network/Improve_RED/src/ared_simulator.py:24-33`', '瓶颈：调参轮次。待补证据', '优化：自动调参。待补证据'); EvidenceLines = @('算法函数：`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:4-37`') },
            @{ Name = '改进RED'; Layer = '算法策略层'; Inputs = @('queue_length', 'beta', '阈值'); Outputs = @('S型概率', 'max_p轨迹'); Interfaces = @('s_shaped_probability()', 'update_max_p()'); Stack = @('非线性概率建模'); Topics = @('S型丢包概率', 'RED算法'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('computer-network/Improve_RED/src/improved_red_simulator.py:18-47'); YamlRisks = @('beta迁移成本高。`computer-network/Improve_RED/src/improved_red_simulator.py:16`'); YamlTradeoffs = @('更灵活但更复杂。`computer-network/Improve_RED/src/improved_red_simulator.py:25-33`'); Duty = @('负责改进RED公式和自适应控制。`computer-network/Improve_RED/src/improved_red_simulator.py:18-47`'); IO = @('输入：队列长度+参数。`computer-network/Improve_RED/src/improved_red_simulator.py:20-33`', '输出：概率和max_p序列。`computer-network/Improve_RED/src/improved_red_simulator.py:89-95`', '关键接口：s_shaped_probability。`computer-network/Improve_RED/src/improved_red_simulator.py:18-34`'); Algorithms = @('S型函数+目标区间更新。`computer-network/Improve_RED/src/improved_red_simulator.py:37-47`'); Performance = @('当前：单步O(1)。`computer-network/Improve_RED/src/improved_red_simulator.py:18-34`', '瓶颈：多参数联动重绘。`computer-network/Improve_RED/src/improved_red_simulator.py:121-145`', '优化：缓存中间结果。待补证据'); EvidenceLines = @('S型概率：`computer-network/Improve_RED/src/improved_red_simulator.py:25-33`') },
            @{ Name = '仿真执行'; Layer = '仿真执行层'; Inputs = @('arrival_rate', 'service_rate', 'simulation_time'); Outputs = @('三种算法队列长度'); Interfaces = @('for t in range(simulation_time)'); Stack = @('NumPy随机采样'); Topics = @('离散队列仿真'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('computer-network/Improve_RED/src/red_queue_simulation_visualization.py:99-145'); YamlRisks = @('随机波动。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113`'); YamlTradeoffs = @('仿真快但与真实网络有差距。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:99-102`'); Duty = @('负责时间步推进与队列更新。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-145`'); IO = @('输入：到达/服务率。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:99-102`', '输出：队列轨迹。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:143-145`', '关键接口：仿真主循环。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-141`'); Algorithms = @('泊松到达+二项丢包。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:113-137`'); Performance = @('当前：O(T)。`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:101-112`', '瓶颈：长时仿真。待补证据', '优化：并行多轮。待补证据'); EvidenceLines = @('仿真循环：`computer-network/Improve_RED/src/red_queue_simulation_visualization.py:111-141`') },
            @{ Name = '可视化交互'; Layer = '可视化交互层'; Inputs = @('算法输出序列', '滑条事件'); Outputs = @('对比曲线', '调参界面'); Interfaces = @('plot()', 'Slider.on_changed()'); Stack = @('Matplotlib'); Topics = @('RED算法', 'S型丢包概率'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('computer-network/Improve_RED/src/red_simulator.py:27-87', 'computer-network/Improve_RED/src/improved_red_simulator.py:72-163'); YamlRisks = @('依赖图形环境。待补证据'); YamlTradeoffs = @('演示强但自动评测弱。`computer-network/Improve_RED/src/red_simulator.py:86-89`'); Duty = @('负责可视化展示与交互，不改底层公式。`computer-network/Improve_RED/src/red_simulator.py:35-75`'); IO = @('输入：参数变化。`computer-network/Improve_RED/src/red_simulator.py:53-67`', '输出：曲线图。`computer-network/Improve_RED/src/red_simulator.py:35-40`', '关键接口：Slider。`computer-network/Improve_RED/src/red_simulator.py:72-74`'); Algorithms = @('回调触发重绘。`computer-network/Improve_RED/src/improved_red_simulator.py:121-145`'); Performance = @('当前：全量重绘。`computer-network/Improve_RED/src/improved_red_simulator.py:132-145`', '瓶颈：高分辨率卡顿。待补证据', '优化：降采样。待补证据'); EvidenceLines = @('可视化入口：`computer-network/Improve_RED/src/red_simulator.py:27-40`') }
        )
    },
    @{
        Short = 'CORG'
        Name = '计算机组成原理课程实验资料'
        Type = '通用/其他'
        Confidence = '低'
        SignalSummary = '仅检测到doc/docx文档，缺少可解析源码；多数结论需待补证据。'
        Direction = '408复试准备'
        Scene = '实验资料整理'
        Repo = 'computer-organization'
        Stack = @('Word实验文档')
        Skills = @('实验归档', '证据补全')
        Topics = @('指令执行周期', 'Cache映射策略', '流水线冒险')
        Metrics = @{ latency_ms = '待补证据'; hz = '待补证据'; accuracy = '待补证据'; error = '待补证据' }
        YamlHighlights = @('待补证据：先转写文档。', '待补证据：先补实验结果表。', '待补证据：先补408映射。')
        YamlEvidence = @('待补证据')
        YamlRisks = @('当前缺可检索证据链。待补证据')
        YamlTradeoffs = @('原始文档保真但不可查询。待补证据')
        YamlNextImprove = @('建立Evidence转写文件。', '补指标表。', '补模块化复盘卡。')
        OneLiner = @('当前以原始实验文档归档为主，技术细节尚需转写补证据。待补证据')
        Responsibility = @('先搭复试结构和卡片框架，再补证据与量化指标。待补证据')
        CoreHighlights = @('待补证据')
        EvidenceChain = @('待补证据')
        HighRiskQuestions = @('追问实验参数时会卡壳（待补证据）', '追问性能数据时缺表格（待补证据）')
        NextWork = @('docx转写md', '补实验截图参数', '补408关联问答')
        QAEvidence = @{ arch = '待补证据'; algo = '待补证据'; perf = '待补证据'; trade = '待补证据'; link408 = '待补证据' }
        Modules = @(
            @{ Name = '资料归档'; Layer = '文档层'; Inputs = @('实验doc/docx'); Outputs = @('转写文本（待补）'); Interfaces = @('待补证据'); Stack = @('文档整理'); Topics = @('指令执行周期'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('待补证据'); YamlRisks = @('文档不可检索。待补证据'); YamlTradeoffs = @('保真高但分析难。待补证据'); Duty = @('负责文档分类与命名规范。待补证据'); IO = @('输入：doc/docx；输出：可检索文本。待补证据'); Algorithms = @('待补证据'); Performance = @('当前：检索效率低。待补证据', '瓶颈：无索引。待补证据', '优化：建立Evidence目录。待补证据'); EvidenceLines = @('待补证据') },
            @{ Name = '实验复盘'; Layer = '复习层'; Inputs = @('转写内容（待补）'); Outputs = @('复试口径'); Interfaces = @('待补证据'); Stack = @('Q&A复盘'); Topics = @('流水线冒险'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('待补证据'); YamlRisks = @('缺原始图表。待补证据'); YamlTradeoffs = @('先框架后证据。待补证据'); Duty = @('负责把实验内容映射成问答。待补证据'); IO = @('输入：实验过程；输出：答辩话术。待补证据'); Algorithms = @('待补证据'); Performance = @('当前：准确率无法量化。待补证据', '瓶颈：证据缺失。待补证据', '优化：统一模板。待补证据'); EvidenceLines = @('待补证据') },
            @{ Name = '证据补全'; Layer = '工程层'; Inputs = @('实验文档/截图/参数'); Outputs = @('Evidence索引'); Interfaces = @('待补证据'); Stack = @('证据管理'); Topics = @('Cache映射策略'); Metrics = @{ latency_ms = '待补证据'; hz = '待补证据' }; YamlEvidence = @('待补证据'); YamlRisks = @('无证据即无可辩护结论。待补证据'); YamlTradeoffs = @('补证据成本高但收益大。待补证据'); Duty = @('负责定义补证据路径与模板。待补证据'); IO = @('输入：原材料；输出：证据化文本。待补证据'); Algorithms = @('待补证据'); Performance = @('当前：无量化指标。待补证据', '瓶颈：材料未结构化。待补证据', '优化：模板化采集。待补证据'); EvidenceLines = @('待补证据') }
        )
    }
)

foreach ($p in $projects) {
    $projectDir = Join-Path $vaultRoot ("01_Projects\P__{0}" -f $p.Short)
    Write-Note -Path (Join-Path $projectDir ("P__{0}.md" -f $p.Short)) -Content (New-ProjectCard -P $p)

    foreach ($m in $p.Modules) {
        Write-Note -Path (Join-Path $projectDir ("Modules\M__{0}.md" -f $m.Name)) -Content (New-ModuleCard -P $p -M $m)
    }

    Write-Note -Path (Join-Path $projectDir ("QA\I__{0}__QA.md" -f $p.Short)) -Content (New-QACard -P $p)
}

$topicSeen = @{}
foreach ($t in $topics) {
    if ($topicSeen.ContainsKey($t.Name)) { continue }
    $topicSeen[$t.Name] = $true
    Write-Note -Path (Join-Path $vaultRoot ("03_Topics\T__{0}.md" -f $t.Name)) -Content (New-TopicCard -T $t)
}

Write-Output 'Generated all project/module/qa/topic notes.'

