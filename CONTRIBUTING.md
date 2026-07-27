# 贡献指南

感谢你对本项目的关注！欢迎通过 Issue 或 Pull Request 参与维护。

## 📋 提交前检查

在提交任何更改前，请确认：

### 1. 无临时文件

```bash
# 检查是否混入了临时文件
git status
```

确保没有以下类型的文件：
- `*~`、`*.bak`、`*.swp`（编辑器备份）
- `.DS_Store`、`Thumbs.db`（系统文件）
- `__pycache__/`、`*.pyc`（Python 缓存）
- `tmp/` 目录内容

### 2. 大文件使用 Git LFS

```bash
# 检查文件是否被 LFS 正确管理
git lfs ls-files
```

超过 1MB 的二进制文件（docx、pdf、pptx、zip 等）应通过 LFS 提交。

### 3. README 同步更新

- 新增目录 → 添加 `README.md`
- 修改目录结构 → 更新对应 `README.md`
- 新增课程 → 在根目录 `README.md` 索引表中添加条目

## 📝 提交规范

### Commit Message 格式

```
<type>: <简短描述>

<详细说明（可选）>
```

**type 类型**：
| type | 用途 |
|------|------|
| `feat` | 新增课程/项目/功能 |
| `fix` | 修复错误 |
| `docs` | 文档更新 |
| `chore` | 维护性改动（清理、配置等） |
| `refactor` | 代码重构 |

### 示例

```
feat: 新增机器学习课程实验代码

- 添加 ml-practice/ 目录
- 包含 3 个实验：分类、回归、聚类
- 使用 scikit-learn 实现
```

## 🔄 Pull Request 流程

1. Fork 本仓库
2. 创建功能分支：`git checkout -b feature/your-feature`
3. 提交更改：`git commit -m "feat: 描述"`
4. 推送到 Fork：`git push origin feature/your-feature`
5. 创建 Pull Request，描述清楚改动内容

## 📁 新增课程目录模板

```
course-name/
├── README.md          # 目录说明（必须）
├── experiment/        # 实验代码（如有）
├── homework/          # 作业（如有）
└── docs/              # 文档资料（如有）
```

**README.md 模板**：

```markdown
# 课程名称

本目录包含《课程名称》课程的相关资料与作业。

## 📁 目录结构

| 目录/文件 | 说明 |
|-----------|------|
| [experiment/](./experiment/) | 实验代码 |
| [homework/](./homework/) | 课后作业 |

## 📝 内容说明

- 简要描述课程内容和本目录资料范围

## ⚠️ 注意事项

- 文档使用 Git LFS 管理，克隆后如需查看请确保 LFS 文件已拉取

---

*最后更新：YYYY年MM月DD日*
```

## 📜 行为准则

- 本仓库仅供学习交流，请勿直接抄袭用于课程作业
- 尊重他人劳动成果，引用请注明来源
- 保持友善，共同维护良好的学习氛围

---

*最后更新：2026年7月27日*
