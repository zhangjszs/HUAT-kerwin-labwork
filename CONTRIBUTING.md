# 贡献指南

感谢你对 HUAT-kerwin-labwork 项目的关注！我们欢迎任何形式的贡献。

## 🤝 如何贡献

### 📋 报告问题

如果你发现了 Bug 或有改进建议，请通过以下方式报告：

1. **Bug 报告**
   - 使用 GitHub Issues 报告 Bug
   - 描述复现步骤
   - 附上截图或日志
   - 标注严重程度

2. **功能建议**
   - 提出新功能建议
   - 改进现有功能
   - 优化用户体验

### 📝 提交代码

#### 1. Fork 项目

```bash
# Fork 本仓库到你的 GitHub 账号
# 然后克隆你的 fork
git clone https://github.com/YOUR_USERNAME/HUAT-kerwin-labwork.git
cd HUAT-kerwin-labwork
```

#### 2. 创建功能分支

```bash
# 同步最新代码
git fetch upstream
git checkout main
git merge upstream/main

# 创建新分支
git checkout -b feature/your-feature-name
```

#### 3. 提交更改

```bash
# 添加更改的文件
git add .

# 提交更改（遵循提交信息规范）
git commit -m "feat: add your feature description"
```

#### 4. 推送到你的 fork

```bash
# 推送到你的远程仓库
git push origin feature/your-feature-name
```

#### 5. 创建 Pull Request

- 在 GitHub 上创建 Pull Request
- 详细描述你的改动
- 关联相关 Issue
- 等待代码审查

### 📝 代码规范

#### 命名规范

**C/C++**
- 类名：大驼峰 (PascalCase)
- 函数名：小驼峰 (camelCase)
- 变量名：小驼峰 (camelCase)
- 常量：全大写下划线 (UPPER_CASE)
- 成员变量：前缀下划线 (m_memberName)

**Java**
- 类名：大驼峰 (PascalCase)
- 方法名：小驼峰 (camelCase)
- 常量：全大写下划线 (UPPER_CASE)
- 包名：全小写

**Python**
- 类名：大驼峰 (PascalCase)
- 函数名：小写下划线 (snake_case)
- 变量名：小写下划线 (snake_case)
- 常量：全大写下划线 (UPPER_CASE)

**Assembly**
- 标签：全大写下划线 (UPPER_CASE)

#### 代码风格

- **缩进**：4 空格
- **行宽**：120 字符
- **注释**：使用中文注释，说明功能逻辑
- **函数**：单一职责原则

#### 提交信息规范

使用语义化提交信息：

```
type(scope): brief description

feat: 新功能
fix: 修复 Bug
docs: 文档更新
style: 代码格式调整
refactor: 代码重构
test: 测试相关
chore: 构建/工具相关
```

示例：
```
feat(data-structure): add binary search tree implementation
fix(android): resolve memory leak in MainActivity
docs(readme): update installation instructions
```

### 🧪 测试要求

提交代码前请确保：

1. **代码通过编译**
   - C/C++: `g++ -std=c++11 file.cpp -o output`
   - Java: `javac file.java`
   - Python: 无编译步骤，确保语法正确

2. **测试通过**
   - Python: `pytest tests/`
   - Java: `mvn test`
   - C/C++: 运行单元测试

3. **代码格式化**
   - C/C++: `clang-format -i file.cpp`
   - Python: `black file.py`
   - JavaScript: `prettier --write file.js`

### 🚫 注意事项

- 请确保代码通过编译和测试
- 请遵循上述命名规范
- 提交前请先同步最新代码
- 大改动请先创建 Issue 讨论
- 不要提交敏感信息（密码、密钥等）
- 不要提交编译产物（.class, .o, .exe等）

## 📋 开发环境设置

### 安装 Git LFS

```bash
# Windows (推荐)
winget install Git.GitLFS

# 或下载安装
# 访问: https://git-lfs.github.com/

# 初始化 Git LFS
git lfs install
```

### 克隆项目

```bash
# 克隆项目到本地
git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
cd HUAT-kerwin-labwork

# 初始化 Git LFS
git lfs install
git lfs pull
```

### 安装依赖

**Python**
```bash
pip install -r requirements.txt
pip install pytest pylint black
```

**Java**
```bash
# 使用 Maven
mvn clean install

# 或使用 Gradle
./gradlew build
```

**C/C++**
```bash
# 安装编译器
# Windows: MinGW 或 Visual Studio
# Linux: sudo apt-get install build-essential
# macOS: xcode-select --install
```

## 📚 学习资源

- [Git 官方文档](https://git-scm.com/doc)
- [GitHub 指南](https://guides.github.com/)
- [语义化提交](https://www.conventionalcommits.org/)
- [Python 代码风格指南](https://www.python.org/dev/peps/pep-0008/)
- [Google C++ 风格指南](https://google.github.io/styleguide/cppguide.html)

## 🤝 贡献者指南

### 代码审查流程

1. 提交 Pull Request 后，维护者会进行代码审查
2. 根据反馈意见修改代码
3. 所有检查通过后，代码会被合并到主分支

### 行为准则

- 尊重所有贡献者
- 保持友好和专业的沟通
- 接受建设性的批评
- 专注于对项目最有利的事情

## 📞 联系方式

如有疑问，请通过以下方式联系：

- GitHub Issues: [提交 Issue](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues)
- Email: zhangjszs@foxmail.com

---

感谢你的贡献！🎉
