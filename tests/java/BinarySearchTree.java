import java.util.ArrayList;
import java.util.List;

/**
 * 二叉搜索树类
 */
public class BinarySearchTree {
    private TreeNode root;

    public BinarySearchTree() {
        this.root = null;
    }

    /**
     * 插入节点
     *
     * @param value 要插入的值
     */
    public void insert(int value) {
        root = insertHelper(root, value);
    }

    private TreeNode insertHelper(TreeNode node, int value) {
        if (node == null) {
            return new TreeNode(value);
        }

        if (value < node.value) {
            node.left = insertHelper(node.left, value);
        } else if (value > node.value) {
            node.right = insertHelper(node.right, value);
        }

        return node;
    }

    /**
     * 查找节点
     *
     * @param value 要查找的值
     * @return 如果找到返回节点，否则返回 null
     */
    public TreeNode search(int value) {
        return searchHelper(root, value);
    }

    private TreeNode searchHelper(TreeNode node, int value) {
        if (node == null || node.value == value) {
            return node;
        }

        if (value < node.value) {
            return searchHelper(node.left, value);
        } else {
            return searchHelper(node.right, value);
        }
    }

    /**
     * 中序遍历
     *
     * @return 遍历结果列表
     */
    public List<Integer> inorderTraversal() {
        List<Integer> result = new ArrayList<>();
        inorderHelper(root, result);
        return result;
    }

    private void inorderHelper(TreeNode node, List<Integer> result) {
        if (node != null) {
            inorderHelper(node.left, result);
            result.add(node.value);
            inorderHelper(node.right, result);
        }
    }
}
