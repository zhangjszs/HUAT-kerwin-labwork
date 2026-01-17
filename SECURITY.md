# 安全最佳实践

本文档概述了 HUAT-kerwin-labwork 项目的安全政策和最佳实践。

## 🔒 个人信息保护

### 已完成的安全措施

- ✅ 移除真实姓名和学号
- ✅ 移除 QQ 号和手机号
- ✅ 保留邮箱地址（按用户要求）
- ✅ 使用 Git LFS 管理大文件
- ✅ 配置 .gitignore 忽略敏感文件

### 个人信息保护原则

1. **最小化原则**: 只收集和存储必要的信息
2. **匿名化**: 使用匿名或假名代替真实姓名
3. **脱敏**: 对敏感信息进行脱敏处理
4. **加密**: 对存储的敏感信息进行加密

## 🛡️ 代码安全

### 敏感数据处理

#### ❌ 不安全的做法

```cpp
// 硬编码密码
const char* DB_PASSWORD = "my_password_123";

// 硬编码 API 密钥
const char* API_KEY = "sk-1234567890abcdef";
```

#### ✅ 安全的做法

```cpp
// 使用环境变量
const char* DB_PASSWORD = getenv("DB_PASSWORD");
const char* API_KEY = getenv("API_KEY");

// 或使用配置文件
Config config = loadConfig("config.json");
const char* password = config.getPassword();
```

```python
# 使用环境变量
import os
db_password = os.getenv('DB_PASSWORD')
api_key = os.getenv('API_KEY')

# 或使用配置文件
import configparser
config = configparser.ConfigParser()
config.read('config.ini')
password = config['database']['password']
```

### 日志安全

#### ❌ 不安全的做法

```cpp
// 在日志中输出敏感信息
printf("User login: username=%s, password=%s\n", username, password);
```

```python
# 在日志中输出敏感信息
logging.info(f"User login: username={username}, password={password}")
```

#### ✅ 安全的做法

```cpp
// 脱敏处理
printf("User login: username=%s, password=***\n", username);
```

```python
# 脱敏处理
logging.info(f"User login: username={username}, password=***")
```

### 文件操作安全

#### ❌ 不安全的做法

```cpp
// 直接使用用户输入的路径
void readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    // ...
}
```

```python
# 直接使用用户输入的路径
def read_file(filename):
    with open(filename, 'r') as f:
        return f.read()
```

#### ✅ 安全的做法

```cpp
// 验证路径有效性
bool isValidPath(const char* path) {
    // 检查路径是否在允许的目录内
    // 防止路径遍历攻击
    return true;
}

void readFileSafe(const char* filename) {
    if (!isValidPath(filename)) {
        printf("Invalid path!\n");
        return;
    }
    FILE* file = fopen(filename, "r");
    // ...
}
```

```python
import os

def is_valid_path(path):
    """验证路径是否在允许的目录内"""
    allowed_dir = "/safe/directory/"
    real_path = os.path.realpath(path)
    return real_path.startswith(allowed_dir)

def read_file_safe(filename):
    if not is_valid_path(filename):
        raise ValueError("Invalid path!")
    with open(filename, 'r') as f:
        return f.read()
```

### 字符串操作安全

#### ❌ 不安全的做法

```cpp
// 可能缓冲区溢出
char buffer[100];
strcpy(buffer, userInput);
```

```cpp
// 可能格式化字符串漏洞
char buffer[100];
sprintf(buffer, userInput);
```

#### ✅ 安全的做法

```cpp
// 使用安全的字符串函数
char buffer[100];
strncpy(buffer, userInput, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\0';
```

```cpp
// 使用安全的格式化函数
char buffer[100];
snprintf(buffer, sizeof(buffer), "%s", userInput);
```

### 输入验证

#### ❌ 不安全的做法

```cpp
// 不验证输入
void processInput(int value) {
    int array[100];
    array[value] = 0;  // 可能数组越界
}
```

```python
# 不验证输入
def process_input(value):
    array = [0] * 100
    array[value] = 0  # 可能索引越界
```

#### ✅ 安全的做法

```cpp
// 验证输入范围
void processInput(int value) {
    if (value < 0 || value >= 100) {
        printf("Invalid input!\n");
        return;
    }
    int array[100];
    array[value] = 0;
}
```

```python
# 验证输入范围
def process_input(value):
    if value < 0 or value >= 100:
        raise ValueError("Invalid input!")
    array = [0] * 100
    array[value] = 0
```

## 🔐 依赖管理

### 第三方依赖安全

1. **定期更新依赖**: 及时更新到最新稳定版本
2. **使用已知安全的版本**: 避免使用有已知漏洞的版本
3. **检查依赖漏洞**: 使用工具扫描依赖漏洞
4. **最小化依赖**: 只使用必要的依赖库

#### Python 依赖安全

```bash
# 检查依赖漏洞
pip install safety
safety check

# 更新依赖
pip install --upgrade package_name
```

#### Java 依赖安全

```bash
# 使用 Maven 检查依赖漏洞
mvn org.owasp:dependency-check-maven:check

# 更新依赖
mvn versions:display-dependency-updates
```

#### Node.js 依赖安全

```bash
# 检查依赖漏洞
npm audit

# 修复依赖漏洞
npm audit fix
```

## 🌐 Git 安全

### .gitignore 配置

确保以下文件不被提交到 Git：

```
# 敏感配置文件
config.json
.env
*.key
*.pem

# 个人信息
*.docx
*.pdf
*.xlsx

# 编译产物
*.class
*.o
*.exe
*.dll
*.so

# IDE 配置
.vscode/
.idea/
*.swp
*.swo

# 操作系统文件
.DS_Store
Thumbs.db
```

### Git LFS 安全

- 使用 Git LFS 管理大文件（ZIP、PDF、PPT、Excel等）
- 配置 Git LFS 权限
- 定期检查 LFS 文件

```bash
# 初始化 Git LFS
git lfs install

# 跟踪大文件
git lfs track "*.pdf"
git lfs track "*.zip"
git lfs track "*.pptx"

# 提交 .gitattributes
git add .gitattributes
git commit -m "chore: add git lfs tracking"
```

### 提交历史安全

- 不要在提交历史中包含敏感信息
- 使用 `git filter-repo` 清理历史中的敏感信息
- 定期检查提交历史

```bash
# 检查提交历史中的敏感信息
git log -p | grep -i "password\|secret\|key"

# 清理历史中的敏感信息（谨慎使用）
git filter-repo --invert-paths --path sensitive_file.txt
```

## 🚨 安全漏洞报告

### 报告安全漏洞

如果你发现了安全漏洞，请通过以下方式报告：

1. **不要公开披露**: 不要在 GitHub Issues 或公开论坛中披露
2. **发送邮件**: zhangjszs@foxmail.com
3. **提供详细信息**:
   - 漏洞描述
   - 复现步骤
   - 影响范围
   - 建议的修复方案

### 漏洞响应流程

1. **确认漏洞**: 维护者确认漏洞的存在
2. **评估影响**: 评估漏洞的严重程度和影响范围
3. **修复漏洞**: 开发修复补丁
4. **测试验证**: 验证修复的有效性
5. **发布更新**: 发布安全更新
6. **公开披露**: 在修复后公开披露漏洞信息

## 📋 安全检查清单

在提交代码前，请检查：

- [ ] 代码中不包含硬编码的密码、密钥、Token
- [ ] 敏感信息已脱敏处理
- [ ] 日志中不输出敏感信息
- [ ] 文件操作已验证路径有效性
- [ ] 字符串操作使用安全函数
- [ ] 输入已进行验证
- [ ] 依赖已更新到最新稳定版本
- [ ] .gitignore 已配置正确
- [ ] 不提交敏感文件到 Git

## 📚 安全资源

- [OWASP Top 10](https://owasp.org/www-project-top-ten/)
- [CWE Top 25](https://cwe.mitre.org/top25/)
- [Python 安全最佳实践](https://docs.python.org/3/library/security_warnings.html)
- [C++ 安全编码](https://isocpp.org/blog/2018/01/secure-coding)

## 📞 联系方式

如有安全问题或建议，请通过以下方式联系：

- Email: zhangjszs@foxmail.com
- GitHub Issues: [提交 Issue](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues)

---

感谢你对项目安全的关注！🔒
