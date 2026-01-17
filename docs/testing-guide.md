# 测试指南

本文档提供了 HUAT-kerwin-labwork 项目的测试指南，包括不同语言的测试框架和最佳实践。

## 📋 目录

- [Python 测试](#python-测试)
- [C/C++ 测试](#cc-测试)
- [Java 测试](#java-测试)
- [测试最佳实践](#测试最佳实践)
- [CI/CD 集成](#cicd-集成)

## Python 测试

### 测试框架

本项目使用 **pytest** 作为 Python 测试框架。

### 安装依赖

```bash
pip install pytest pytest-cov
```

### 运行测试

```bash
# 运行所有测试
pytest

# 运行特定测试文件
pytest tests/python/test_binary_search_tree.py

# 运行特定测试函数
pytest tests/python/test_binary_search_tree.py::TestBinarySearchTree::test_insert_single_node

# 显示详细输出
pytest -v

# 显示测试覆盖率
pytest --cov=tests/python --cov-report=html
```

### 测试示例

```python
import pytest
from binary_search_tree import BinarySearchTree, TreeNode


class TestBinarySearchTree:
    """二叉搜索树测试类"""

    def test_insert_single_node(self):
        """测试插入单个节点"""
        bst = BinarySearchTree()
        bst.insert(5)

        assert bst.root.value == 5
        assert bst.root.left is None
        assert bst.root.right is None

    def test_insert_multiple_nodes(self):
        """测试插入多个节点"""
        bst = BinarySearchTree()
        bst.insert(5)
        bst.insert(3)
        bst.insert(7)

        assert bst.root.value == 5
        assert bst.root.left.value == 3
        assert bst.root.right.value == 7
```

### 测试最佳实践

1. **使用描述性的测试名称**: 测试名称应该清楚地描述测试的内容
2. **遵循 AAA 模式**: Arrange（准备）、Act（执行）、Assert（断言）
3. **使用 fixture**: 对于重复的测试设置，使用 pytest fixture
4. **测试边界条件**: 测试正常情况、边界情况和异常情况

## C/C++ 测试

### 测试框架

本项目使用 **assert** 宏进行简单的单元测试。

### 编译和运行

```bash
# 编译测试
g++ -std=c++11 tests/cpp/test_binary_search_tree.cpp -o test_binary_search_tree

# 运行测试
./test_binary_search_tree
```

### 测试示例

```cpp
#include <iostream>
#include <cassert>

/**
 * @brief 测试插入单个节点
 */
void testInsertSingleNode() {
    BinarySearchTree bst;
    bst.insert(5);

    assert(bst.search(5) != nullptr);
    assert(bst.search(5)->value == 5);
    assert(bst.search(3) == nullptr);

    std::cout << "testInsertSingleNode: PASSED\n";
}

/**
 * @brief 主函数
 */
int main() {
    std::cout << "Running tests...\n\n";

    testInsertSingleNode();
    testInsertMultipleNodes();
    testSearchExistingNode();

    std::cout << "\nAll tests passed!\n";
    return 0;
}
```

### 测试最佳实践

1. **使用 assert 宏**: 使用 assert 进行断言
2. **提供清晰的输出**: 使用 std::cout 输出测试进度
3. **测试边界条件**: 测试空树、单节点、多节点等情况
4. **使用命名空间**: 将测试函数放在合适的命名空间中

## Java 测试

### 测试框架

本项目使用 **JUnit 5** 作为 Java 测试框架。

### 依赖配置

在 `pom.xml` 中添加 JUnit 5 依赖：

```xml
<dependency>
    <groupId>org.junit.jupiter</groupId>
    <artifactId>junit-jupiter-api</artifactId>
    <version>5.9.2</version>
    <scope>test</scope>
</dependency>
<dependency>
    <groupId>org.junit.jupiter</groupId>
    <artifactId>junit-jupiter-engine</artifactId>
    <version>5.9.2</version>
    <scope>test</scope>
</dependency>
```

### 运行测试

```bash
# 使用 Maven
mvn test

# 使用 Gradle
./gradlew test

# 运行特定测试类
mvn test -Dtest=BinarySearchTreeTest

# 运行特定测试方法
mvn test -Dtest=BinarySearchTreeTest#testInsertSingleNode
```

### 测试示例

```java
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * 二叉搜索树测试类
 */
public class BinarySearchTreeTest {

    /**
     * 测试插入单个节点
     */
    @Test
    public void testInsertSingleNode() {
        BinarySearchTree bst = new BinarySearchTree();
        bst.insert(5);

        TreeNode node = bst.search(5);
        assertNotNull(node);
        assertEquals(5, node.value);
        assertNull(bst.search(3));
    }

    /**
     * 测试插入多个节点
     */
    @Test
    public void testInsertMultipleNodes() {
        BinarySearchTree bst = new BinarySearchTree();
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);

        TreeNode root = bst.search(5);
        assertNotNull(root);
        assertNotNull(bst.search(3));
        assertNotNull(bst.search(7));
        assertEquals(3, root.left.value);
        assertEquals(7, root.right.value);
    }
}
```

### 测试最佳实践

1. **使用 @Test 注解**: 标记测试方法
2. **使用断言方法**: 使用 assertNotNull、assertEquals 等断言方法
3. **提供清晰的测试名称**: 测试方法名称应该清楚地描述测试的内容
4. **测试边界条件**: 测试正常情况、边界情况和异常情况

## 测试最佳实践

### 通用原则

1. **独立性**: 每个测试应该独立运行，不依赖其他测试
2. **可重复性**: 测试应该能够重复运行，并且每次结果一致
3. **快速性**: 测试应该快速运行，避免不必要的延迟
4. **清晰性**: 测试应该清楚地描述测试的目的和预期结果

### 测试覆盖率

- **单元测试**: 测试单个函数或方法
- **集成测试**: 测试多个组件之间的交互
- **端到端测试**: 测试整个系统的功能

### 测试命名规范

- **Python**: `test_<function_name>` 或 `test_<class_name>_<method_name>`
- **C/C++**: `test<FunctionName>` 或 `test<ClassName><MethodName>`
- **Java**: `test<MethodName>` 或 `test<ClassName><MethodName>`

### 断言原则

1. **一个测试一个断言**: 每个测试应该只测试一个功能点
2. **使用有意义的断言消息**: 断言失败时提供清晰的错误信息
3. **测试边界条件**: 测试正常情况、边界情况和异常情况

## CI/CD 集成

### GitHub Actions

项目使用 GitHub Actions 进行自动化测试。

### 工作流示例

```yaml
name: CI

on:
  push:
    branches: [main, develop]
  pull_request:
    branches: [main, develop]

jobs:
  test-python:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Set up Python
        uses: actions/setup-python@v4
        with:
          python-version: '3.11'
      - name: Install dependencies
        run: |
          python -m pip install --upgrade pip
          pip install pytest pytest-cov
      - name: Run tests
        run: pytest tests/python/ -v

  test-cpp:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install dependencies
        run: sudo apt-get install -y g++
      - name: Compile tests
        run: g++ -std=c++11 tests/cpp/test_binary_search_tree.cpp -o test_binary_search_tree
      - name: Run tests
        run: ./test_binary_search_tree

  test-java:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Set up JDK
        uses: actions/setup-java@v4
        with:
          java-version: '11'
          distribution: 'temurin'
      - name: Run tests
        run: mvn test
```

## 📚 学习资源

- [pytest 官方文档](https://docs.pytest.org/)
- [JUnit 5 用户指南](https://junit.org/junit5/docs/current/user-guide/)
- [C++ 测试最佳实践](https://google.github.io/googletest/)
- [测试驱动开发 (TDD)](https://en.wikipedia.org/wiki/Test-driven_development)

## 📞 联系方式

如有测试相关问题，请通过以下方式联系：

- GitHub Issues: [提交 Issue](https://github.com/zhangjszs/HUAT-kerwin-labwork/issues)
- Email: zhangjszs@foxmail.com

---

感谢你对项目质量的关注！🧪
