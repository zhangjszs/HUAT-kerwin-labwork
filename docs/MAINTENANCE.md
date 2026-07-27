# 项目维护指南

本文档定义 HUAT-kerwin-labwork 仓库的临时文件管理、文档检查与质量保障流程。

---

## 一、临时文件管理机制

### 1.1 禁止提交的文件类型

以下模式已在根目录 `.gitignore` 中配置，**不应手动移除或覆盖**：

| 类别 | 模式 | 说明 |
|------|------|------|
| 系统文件 | `.DS_Store`, `Thumbs.db`, `Desktop.ini` | 操作系统自动生成 |
| 编辑器备份 | `*~`, `*.swp`, `*.swo`, `*.bak`, `*.orig`, `~$*` | Vim / WPS / Office 临时文件 |
| 构建产物 | `__pycache__/`, `*.pyc`, `build/`, `dist/`, `.gradle/` | 编译/打包生成 |
| IDE 配置 | `.idea/`, `.vscode/*`, `*.iml` | 个人 IDE 设置（共享配置除外） |
| 环境文件 | `.env`, `*.local`, `venv/` | 本地环境/密钥 |
| 临时目录 | `tmp/`, `tmp_archive/` | 临时扫描/备份文件 |

### 1.2 防止 tmp 目录重复出现

```bash
# 定期扫描（建议每月执行一次）
find . -path ./.git -prune -o -type d -name "tmp" -print
find . -path ./.git -prune -o -type d -name "__pycache__" -print

# 扫描编辑器备份文件
find . -path ./.git -prune -o -name "*~" -type f -print
```

### 1.3 发现误提交临时文件时的处理

```bash
# 1. 从 git 追踪中移除（保留本地文件）
git rm --cached <文件路径>

# 2. 确认 .gitignore 已覆盖该模式
git check-ignore -v <文件路径>

# 3. 提交修复
git add .gitignore
git commit -m "chore: 移除误提交的临时文件"
```

---

## 二、文档完整性检查清单

### 2.1 目录 README 标准

每个课程目录应包含 `README.md`，至少包括：

- [ ] 目录用途说明（1-2 句话）
- [ ] 目录结构表格（列出主要文件/子目录及说明）
- [ ] 内容说明或使用指南
- [ ] 注意事项（如 LFS 文件提示）
- [ ] 最后更新日期

### 2.2 当前各目录 README 覆盖情况

| 目录 | README | 质量 | 备注 |
|------|--------|------|------|
| spss/ | ✅ | 良好 | 2026-07-27 更新 |
| software-engineering/ | ✅ | 良好 | 2026-07-27 重写 |
| android-mobile-development/final_course_project/ | ✅ | 良好 | 2026-07-27 新增 |
| data-analysis-visualization/ | ✅ | - | - |
| 其他课程目录 | 部分 | 待完善 | 中期逐步补充 |

### 2.3 定期检查脚本

```bash
#!/bin/bash
# 检查所有一级目录的 README 覆盖情况
for dir in */; do
    if [ -f "$dir/README.md" ]; then
        echo "✅ $dir"
    else
        echo "❌ $dir (缺少 README)"
    fi
done
```

---

## 三、目录结构规范

### 3.1 命名约定

| 规则 | 正确 | 错误 |
|------|------|------|
| 目录名使用英文 + 连字符 | `data-structures` | `数据结构` |
| 文件名避免特殊字符 | `experiment1.ipynb` | `实验一.ipynb`（尽量） |
| 大文件使用 Git LFS | `.docx`, `.pdf`, `.pptx`, `.jpg` | 直接提交二进制 |

### 3.2 LFS 管理的大文件类型

```
*.doc, *.docx    # Word 文档
*.pdf            # PDF 文档
*.ppt, *.pptx    # PPT 课件
*.xls, *.xlsx    # Excel 表格
*.jpg, *.png     # 图片（>100KB 时）
*.zip, *.rar     # 压缩包
```

### 3.3 目录层级建议

```
课程目录/
├── README.md          # 必须：目录说明
├── experiment/        # 实验代码
├── homework/          # 课后作业
├── ppt/               # 课件资料
├── docs/              # 项目文档
└── final_project/     # 期末项目
```

---

## 四、质量检查流程

### 4.1 提交前自检（每次 commit）

```bash
# 1. 检查是否有临时文件混入
git diff --cached --name-only | grep -E '(\.tmp|~|\.bak|__pycache__)'

# 2. 检查大文件是否走 LFS
git diff --cached --name-only | while read f; do
    if [ -f "$f" ] && [ $(stat -c%s "$f") -gt 1048576 ]; then
        echo "⚠️ 大文件: $f"
    fi
done

# 3. 检查 README 是否同步更新
git diff --cached --name-only | grep -v "README.md" | head -5
```

### 4.2 定期维护（每月/每学期）

| 检查项 | 频率 | 工具/方法 |
|--------|------|-----------|
| 临时文件扫描 | 每月 | `find` 命令 |
| README 完整性 | 每学期 | 检查清单 2.1 |
| LFS 文件健康检查 | 每学期 | `git lfs fsck` |
| 死链接检查 | 每学期 | 手动检查 README 内链接 |
| 空目录清理 | 每学期 | `find . -type d -empty` |

### 4.3 文档更新触发条件

以下情况应立即更新对应 README：

- 新增/删除课程目录
- 目录内文件结构发生重大变化
- LFS 文件增减
- 课程作业或项目内容更新

---

## 五、维护责任

| 角色 | 职责 |
|------|------|
| 仓库所有者 | 最终审核、LFS 管理、重大结构调整 |
| 贡献者 | 提交时自检、更新自己添加内容的 README |
| 自动化（未来） | pre-commit hook 拦截临时文件、CI 检查文档完整性 |

---

*最后更新：2026年7月27日*
