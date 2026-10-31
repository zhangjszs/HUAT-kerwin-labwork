# 脚本工具

本目录包含项目维护和个人学习过程中使用的脚本工具。

## 📁 文件说明

| 文件 | 说明 |
|------|------|
| [generate_obsidian_scan_notes.ps1](./generate_obsidian_scan_notes.ps1) | Obsidian 扫描笔记生成脚本（PowerShell） |
| [tools/clean_notebooks.mjs](./tools/clean_notebooks.mjs) | Jupyter Notebook 输出清理与附件导出（Node） |
| [tools/setup_notebook_filter.ps1](./tools/setup_notebook_filter.ps1) | 为当前 clone 启用 notebook 自动清理 filter（PowerShell） |
| [tools/](./tools/) | 其他辅助工具 |

## Jupyter Notebook 清理

`tools/clean_notebooks.mjs` 会清空 `.ipynb` 的运行输出（`outputs`、`execution_count`、执行时间戳），并把 markdown 内嵌附件（截图）导出为 `<notebook>.assets/*.png`，同时改写引用路径。附件目录按 `.gitattributes` 走 Git LFS。

```bash
# 检查（存在输出/附件时退出码为 1）
node scripts/tools/clean_notebooks.mjs --check

# 一次性清理全部已跟踪 notebook
node scripts/tools/clean_notebooks.mjs --all

# 只清输出 / 只导出附件
node scripts/tools/clean_notebooks.mjs --strip
node scripts/tools/clean_notebooks.mjs --extract-attachments
```

### 提交时自动清理（git clean filter）

等价于 nbstripout，但只依赖 Node，不需要 Python：

```powershell
powershell -ExecutionPolicy Bypass -File scripts/tools/setup_notebook_filter.ps1
```

`.gitattributes` 已声明 `*.ipynb filter=nbclean`；未执行上述脚本的 clone 会原样提交，不会报错。撤销配置：`git config --local --remove-section filter.nbclean`。

注意：filter 只清输出，不导出附件；新 notebook 里的截图请先运行 `--extract-attachments`。

## 📝 说明

- 脚本主要用于本地笔记整理和项目扫描
- 运行前请确保已安装对应运行环境
