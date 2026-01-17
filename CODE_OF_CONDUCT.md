# 编码规范与行为准则

本项目致力于为所有贡献者提供一个友好、开放和包容的环境。请阅读并遵守以下规范。

## 📋 编码规范

### C/C++ 命名规范

#### 类命名
```cpp
class BinarySearchTree {
private:
    TreeNode* m_rootNode;
public:
    BinarySearchTree();
    void insert(int value);
};
```

#### 函数命名
```cpp
void insertNode(TreeNode* node, int value) {
    // 函数实现
}
```

#### 变量命名
```cpp
int studentCount = 0;
double averageScore = 0.0;
bool isValid = true;
```

#### 常量命名
```cpp
const int MAX_SIZE = 100;
const double PI = 3.1415926;
const char* DEFAULT_NAME = "Unknown";
```

#### 成员变量命名
```cpp
class Student {
private:
    int m_id;
    std::string m_name;
    double m_score;
};
```

### Java 命名规范

#### 类命名
```java
public class BinarySearchTree {
    private TreeNode rootNode;

    public BinarySearchTree() {
        // 构造函数
    }

    public void insert(int value) {
        // 方法实现
    }
}
```

#### 方法命名
```java
public void insertNode(TreeNode node, int value) {
    // 方法实现
}

public boolean isValid() {
    return true;
}
```

#### 常量命名
```java
public static final int MAX_SIZE = 100;
public static final double PI = 3.1415926;
public static final String DEFAULT_NAME = "Unknown";
```

#### 包命名
```java
package com.example.datastructure;
package org.huat.labwork;
```

### Python 命名规范

#### 类命名
```python
class BinarySearchTree:
    def __init__(self):
        self.root_node = None

    def insert(self, value):
        # 方法实现
        pass
```

#### 函数命名
```python
def insert_node(node, value):
    # 函数实现
    pass

def calculate_average(scores):
    # 函数实现
    pass
```

#### 变量命名
```python
student_count = 0
average_score = 0.0
is_valid = True
```

#### 常量命名
```python
MAX_SIZE = 100
PI = 3.1415926
DEFAULT_NAME = "Unknown"
```

### Assembly 命名规范

```assembly
; 标签使用全大写
START:
    MOV AX, @DATA
    MOV DS, AX

MAIN_LOOP:
    CMP CX, 0
    JZ END_PROGRAM

END_PROGRAM:
    MOV AH, 4CH
    INT 21H
```

## 📋 代码风格

### 缩进和格式

- **缩进**: 4 空格
- **行宽**: 120 字符
- **空行**: 函数之间空 1 行，逻辑块之间空 1 行

#### C/C++ 示例
```cpp
#include <iostream>

class BinarySearchTree {
private:
    struct TreeNode {
        int value;
        TreeNode* left;
        TreeNode* right;
    };

    TreeNode* m_root;

public:
    BinarySearchTree() : m_root(nullptr) {}

    void insert(int value) {
        m_root = insertHelper(m_root, value);
    }

private:
    TreeNode* insertHelper(TreeNode* node, int value) {
        if (node == nullptr) {
            return new TreeNode{value, nullptr, nullptr};
        }

        if (value < node->value) {
            node->left = insertHelper(node->left, value);
        } else if (value > node->value) {
            node->right = insertHelper(node->right, value);
        }

        return node;
    }
};
```

#### Python 示例
```python
class BinarySearchTree:
    """二叉搜索树实现"""

    def __init__(self):
        """初始化二叉搜索树"""
        self.root = None

    def insert(self, value):
        """插入节点"""
        self.root = self._insert_helper(self.root, value)

    def _insert_helper(self, node, value):
        """递归插入节点的辅助方法"""
        if node is None:
            return TreeNode(value)

        if value < node.value:
            node.left = self._insert_helper(node.left, value)
        elif value > node.value:
            node.right = self._insert_helper(node.right, value)

        return node
```

### 注释规范

- 使用中文注释
- 函数必须有文档字符串
- 复杂逻辑需要详细注释
- 注释要说明"为什么"，而不是"是什么"

#### C/C++ 注释示例
```cpp
/**
 * @brief 向二叉搜索树中插入一个值
 * @param value 要插入的值
 * @note 如果值已存在，则不插入
 */
void insert(int value) {
    // 使用递归方式插入节点
    m_root = insertHelper(m_root, value);
}
```

#### Python 注释示例
```python
def insert(self, value):
    """
    向二叉搜索树中插入一个值

    Args:
        value: 要插入的值

    Note:
        如果值已存在，则不插入
    """
    self.root = self._insert_helper(self.root, value)
```

## 📋 提交信息规范

使用语义化提交信息：

```
type(scope): brief description

详细描述（可选）

关联的 Issue（可选）
```

### 提交类型

- **feat**: 新功能
- **fix**: 修复 Bug
- **docs**: 文档更新
- **style**: 代码格式调整（不影响代码运行）
- **refactor**: 代码重构（既不是新功能也不是修复）
- **perf**: 性能优化
- **test**: 测试相关
- **chore**: 构建/工具相关

### 提交示例

```
feat(data-structure): add binary search tree implementation

实现了二叉搜索树的基本操作，包括插入、查找和遍历。

Closes #123
```

```
fix(android): resolve memory leak in MainActivity

修复了在 MainActivity 中未释放资源导致的内存泄漏问题。

Fixes #456
```

```
docs(readme): update installation instructions

更新了 README 中的安装步骤，添加了 Git LFS 的安装说明。
```

## 📋 安全规范

### 敏感信息处理

- ❌ 不要在代码中硬编码密码、密钥、Token
- ❌ 不要在日志中输出个人信息
- ❌ 不要提交包含敏感信息的配置文件
- ✅ 使用环境变量存储敏感配置
- ✅ 使用配置文件（并在 .gitignore 中忽略）
- ✅ 使用密钥管理服务

### 文件操作安全

```cpp
// 不安全：直接使用用户输入的路径
void readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    // ...
}

// 安全：验证路径有效性
void readFileSafe(const char* filename) {
    if (!isValidPath(filename)) {
        return;
    }
    FILE* file = fopen(filename, "r");
    // ...
}
```

### 字符串操作安全

```cpp
// 不安全：可能缓冲区溢出
char buffer[100];
strcpy(buffer, userInput);

// 安全：使用安全的字符串函数
char buffer[100];
strncpy(buffer, userInput, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\0';
```

## 📋 行为准则

### 我们的承诺

为了营造开放和友好的环境，我们承诺：

- 尊重不同的观点和经验
- 优雅地接受建设性批评
- 关注对项目最有利的事情
- 对其他社区成员表示同理心

### 不可接受的行为

- 使用性化的语言或图像
- 人身攻击或侮辱性评论
- 骚扰
- 未经许可发布他人的私人信息
- 其他不道德或不专业的行为

### 项目维护者的责任

项目维护者有责任：

- 阐明可接受行为的标准
- 采取适当和公平的纠正措施
- 对不可接受的行为做出回应

## 📋 联系方式

如有任何问题或建议，请通过以下方式联系：

- GitHub Issues: [提交 Issue](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues)
- Email: zhangjszs@foxmail.com

---

感谢你的贡献和对规范的遵守！🎉
