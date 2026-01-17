"""
二叉搜索树测试用例

使用 pytest 框架进行单元测试
"""

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

    def test_search_existing_node(self):
        """测试查找存在的节点"""
        bst = BinarySearchTree()
        bst.insert(5)
        bst.insert(3)
        bst.insert(7)

        node = bst.search(3)
        assert node is not None
        assert node.value == 3

    def test_search_non_existing_node(self):
        """测试查找不存在的节点"""
        bst = BinarySearchTree()
        bst.insert(5)
        bst.insert(3)
        bst.insert(7)

        node = bst.search(10)
        assert node is None

    def test_inorder_traversal(self):
        """测试中序遍历"""
        bst = BinarySearchTree()
        bst.insert(5)
        bst.insert(3)
        bst.insert(7)
        bst.insert(1)
        bst.insert(9)

        result = bst.inorder_traversal()
        assert result == [1, 3, 5, 7, 9]

    def test_empty_tree(self):
        """测试空树"""
        bst = BinarySearchTree()

        assert bst.root is None
        assert bst.search(5) is None
        assert bst.inorder_traversal() == []

    def test_duplicate_values(self):
        """测试插入重复值"""
        bst = BinarySearchTree()
        bst.insert(5)
        bst.insert(5)
        bst.insert(5)

        assert bst.root.value == 5
        assert bst.root.left is None
        assert bst.root.right is None


class TestTreeNode:
    """二叉搜索树节点测试类"""

    def test_node_initialization(self):
        """测试节点初始化"""
        node = TreeNode(10)
        assert node.value == 10
        assert node.left is None
        assert node.right is None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
