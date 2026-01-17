# 项目目录整理建议

本文档提供了 HUAT-kerwin-labwork 项目的目录结构整理建议。

## 📊 当前目录结构分析

### 🔴 高优先级问题

#### 1. 临时文件和未命名文件

**问题位置**：
- `C语言课设/未命名文件.*` - 多个未命名文件
- `数据结构课设/coding/CMakeLists.txt.user` - IDE配置文件

**建议**：
- 将未命名文件重命名为有意义的名称
- 将IDE配置文件添加到 `.gitignore`

#### 2. 编译产物未清理

**问题位置**：
- Android项目的 `app/build/` 目录
- Java项目的 `target/` 目录

**建议**：
- 这些目录已在 `.gitignore` 中，建议定期清理本地构建产物

### 🟡 中优先级问题

#### 3. 目录命名不一致

**问题**：
- 混用中英文：`C语言课设` vs `data-structure`
- 混用空格和下划线：`graph theory` vs `Stackandqueue`
- 大小写不一致：`BinaryTree` vs `linked list`

**建议重命名**：

| 当前名称 | 建议名称 | 原因 |
|---------|-----------|--------|
| C语言课设 | c-course-design | 统一使用英文，避免空格 |
| android移动开发 | android-mobile-development | 统一使用英文，避免空格 |
| javaweb课设 | javaweb-course-design | 统一使用英文，避免空格 |
| linux系统应用 | linux-system-application | 统一使用英文，避免空格 |
| 嵌入式系统原理与应用 | embedded-systems | 统一使用英文，避免空格 |
| 微机原理和接口 | microcomputer-principles | 统一使用英文，避免空格 |
| 微机原理和接口课设 | microcomputer-course-design | 统一使用英文，避免空格 |
| 数据分析与可视化 | data-analysis-visualization | 统一使用英文，避免空格 |
| 数据分析与可视化课程设计 | data-analysis-course-design | 统一使用英文，避免空格 |
| 数据采集与预处理 | data-collection-preprocessing | 统一使用英文，避免空格 |
| graph theory | graph-theory | 使用下划线代替空格 |
| Stackandqueue | stack-and-queue | 使用连字符分隔单词 |
| linked list | linked-list | 使用连字符代替空格 |
| twoDimensionalArray | two-dimensional-array | 使用连字符分隔单词 |

#### 4. 重复文档

**问题位置**：
- `嵌入式系统原理与应用/大作业/` 有多个优化文档

**建议**：
- 保留最新的优化文档
- 将旧文档移动到 `docs/archive/` 目录

### 🟢 低优先级问题

#### 5. 文档分散

**问题**：
- 各课程目录下的文档分散

**建议**：
- 创建统一的文档管理结构

## 📁 建议的目录结构

```
HUAT-kerwin-labwork/
├── .github/                    # GitHub配置
│   ├── ISSUE_TEMPLATE/
│   ├── workflows/
│   └── PULL_REQUEST_TEMPLATE.md
├── .vscode/                   # VS Code配置
├── docs/                      # 项目文档
│   ├── testing-guide.md
│   ├── archive/                # 归档文档
│   └── course-guides/          # 课程指导书
├── tests/                     # 测试代码
│   ├── python/
│   ├── cpp/
│   └── java/
├── c-course-design/            # C语言课设
│   ├── src/                   # 源代码
│   ├── docs/                  # 文档
│   ├── images/                # 图片
│   └── README.md
├── android-mobile-development/  # Android移动开发
│   ├── experiments/            # 实验代码
│   ├── final-project/          # 期末课设
│   └── README.md
├── javaweb-labs/              # JavaWeb实验
│   ├── WebLab1/
│   ├── WebLab2/
│   ├── WebLab3/
│   ├── WebLab4/
│   ├── WebLab5/
│   └── README.md
├── javaweb-course-design/       # JavaWeb课设
│   └── README.md
├── java-course-design/         # Java课程设计
│   └── README.md
├── linux-system-application/   # Linux系统应用
│   ├── homework/
│   ├── exam-review/
│   └── README.md
├── embedded-systems/          # 嵌入式系统
│   ├── experiments/
│   ├── final-project/
│   └── README.md
├── microcomputer-principles/   # 微机原理
│   ├── experiments/
│   ├── class-examples/
│   └── README.md
├── microcomputer-course-design/ # 微机原理课设
│   └── README.md
├── operating-system/           # 操作系统
│   ├── labwork1/
│   ├── labwork2/
│   ├── labwork3/
│   ├── labwork4/
│   ├── labwork5/
│   ├── labwork6/
│   ├── practice/
│   └── README.md
├── data-structure/            # 数据结构
│   ├── linked-list/
│   ├── stack-and-queue/
│   ├── string-operations/
│   ├── two-dimensional-array/
│   ├── binary-tree/
│   ├── graph-theory/
│   ├── search-algorithms/
│   ├── sorting-technology/
│   └── README.md
├── data-structure-course-design/ # 数据结构课设
│   ├── src/
│   ├── python-version/
│   └── README.md
├── data-analysis-visualization/ # 数据分析与可视化
│   ├── experiments/
│   ├── practice/
│   └── README.md
├── data-analysis-course-design/  # 数据分析课设
│   ├── analysis/
│   ├── report-templates/
│   └── README.md
├── data-collection-preprocessing/ # 数据采集与预处理
│   ├── experiments/
│   ├── practice/
│   └── README.md
├── database/                  # 数据库
│   ├── labwork1/
│   ├── labwork4/
│   ├── labwork5/
│   └── README.md
├── school-java-homework/      # Java作业
│   └── README.md
├── .clang-format              # C/C++代码格式化
├── .editorconfig              # 编辑器配置
├── .eslintrc                 # JavaScript代码检查
├── .gitattributes             # Git属性
├── .gitignore                # Git忽略文件
├── .prettierrc               # 代码格式化
├── .pylintrc                 # Python代码检查
├── CODE_OF_CONDUCT.md         # 行为准则
├── CONTRIBUTING.md            # 贡献指南
├── ISSUE_TEMPLATE.md          # Issue模板
├── LICENSE                   # 许可证
├── PULL_REQUEST_TEMPLATE.md  # PR模板
├── README.md                 # 项目说明
├── SECURITY.md               # 安全文档
└── pyproject.toml           # Python配置
```

## 🛠️ 整理步骤

### 步骤1：备份当前项目

```bash
# 创建备份
cd ..
cp -r HUAT-kerwin-labwork HUAT-kerwin-labwork-backup
```

### 步骤2：重命名目录

```bash
# Windows PowerShell
cd HUAT-kerwin-labwork
Rename-Item "C语言课设" "c-course-design"
Rename-Item "android移动开发" "android-mobile-development"
Rename-Item "javaweb课设" "javaweb-course-design"
Rename-Item "linux系统应用" "linux-system-application"
Rename-Item "嵌入式系统原理与应用" "embedded-systems"
Rename-Item "微机原理和接口" "microcomputer-principles"
Rename-Item "微机原理和接口课设" "microcomputer-course-design"
Rename-Item "数据分析与可视化" "data-analysis-visualization"
Rename-Item "数据分析与可视化课程设计" "data-analysis-course-design"
Rename-Item "数据采集与预处理" "data-collection-preprocessing"
Rename-Item "schoolJavaHomework" "school-java-homework"

# 重命名子目录
cd data-structure
Rename-Item "graph theory" "graph-theory"
Rename-Item "Stackandqueue" "stack-and-queue"
Rename-Item "linked list" "linked-list"
Rename-Item "twoDimensionalArray" "two-dimensional-array"
Rename-Item "BinaryTree" "binary-tree"
```

### 步骤3：更新README中的链接

重命名目录后，需要更新以下文件中的路径引用：
- 主 `README.md`
- 各子目录的 `README.md`
- 文档中的路径引用

### 步骤4：清理临时文件

```bash
# 删除IDE配置文件（已添加到.gitignore）
rm -f data-structure-course-design/coding/CMakeLists.txt.user

# 清理构建产物
find . -type d -name "build" -exec rm -rf {} +
find . -type d -name "target" -exec rm -rf {} +
```

### 步骤5：整理文档

```bash
# 创建归档目录
mkdir -p docs/archive

# 移动旧文档
mv embedded-systems/大作业/OPTIMIZATION_*.md docs/archive/
```

## ⚠️ 注意事项

1. **Git历史**：重命名目录会改变Git历史，建议在新的分支上进行
2. **相对路径**：代码中的相对路径引用需要更新
3. **CI/CD**：GitHub Actions中的路径需要更新
4. **文档链接**：所有文档中的超链接需要更新

## 📋 整理检查清单

- [ ] 备份当前项目
- [ ] 重命名所有中文名称目录为英文
- [ ] 统一目录命名风格（使用连字符）
- [ ] 更新README中的所有路径引用
- [ ] 更新代码中的相对路径
- [ ] 清理临时文件和IDE配置
- [ ] 整理重复文档
- [ ] 更新CI/CD配置（如有）
- [ ] 测试所有路径引用
- [ ] 提交更改

## 📚 参考资料

- [Google Shell Style Guide](https://google.github.io/styleguide/shellguide.html)
- [File Naming Conventions](https://en.wikipedia.org/wiki/File_naming_convention)
- [Git Best Practices](https://sethrobertson.github.io/git_best_practices/)

---

**注意**：本文档仅提供整理建议，请根据实际需求选择性地实施。
