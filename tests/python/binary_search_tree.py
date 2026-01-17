"""
二叉搜索树实现

这是一个简单的二叉搜索树实现，用于演示测试用例。
"""

class TreeNode:
    """二叉搜索树节点"""

    def __init__(self, value):
        """
        初始化节点

        Args:
            value: 节点的值
        """
        self.value = value
        self.left = None
        self.right = None


class BinarySearchTree:
    """二叉搜索树"""

    def __init__(self):
        """初始化二叉搜索树"""
        self.root = None

    def insert(self, value):
        """
        插入节点

        Args:
            value: 要插入的值
        """
        self.root = self._insert_helper(self.root, value)

    def _insert_helper(self, node, value):
        """
        递归插入节点的辅助方法

        Args:
            node: 当前节点
            value: 要插入的值

        Returns:
            插入后的根节点
        """
        if node is None:
            return TreeNode(value)

        if value < node.value:
            node.left = self._insert_helper(node.left, value)
        elif value > node.value:
            node.right = self._insert_helper(node.right, value)

        return node

    def search(self, value):
        """
        查找节点

        Args:
            value: 要查找的值

        Returns:
            如果找到返回节点，否则返回 None
        """
        return self._search_helper(self.root, value)

    def _search_helper(self, node, value):
        """
        递归查找节点的辅助方法

        Args:
            node: 当前节点
            value: 要查找的值

        Returns:
            如果找到返回节点，否则返回 None
        """
        if node is None or node.value == value:
            return node

        if value < node.value:
            return self._search_helper(node.left, value)
        else:
            return self._search_helper(node.right, value)

    def inorder_traversal(self):
        """
        中序遍历

        Returns:
            遍历结果列表
        """
        result = []
        self._inorder_helper(self.root, result)
        return result

    def _inorder_helper(self, node, result):
        """
        中序遍历的辅助方法

        Args:
            node: 当前节点
            result: 结果列表
        """
        if node is not None:
            self._inorder_helper(node.left, result)
            result.append(node.value)
            self._inorder_helper(node.right, result)
