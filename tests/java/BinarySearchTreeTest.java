import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.util.List;

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

    /**
     * 测试查找存在的节点
     */
    @Test
    public void testSearchExistingNode() {
        BinarySearchTree bst = new BinarySearchTree();
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);

        TreeNode node = bst.search(3);
        assertNotNull(node);
        assertEquals(3, node.value);
    }

    /**
     * 测试查找不存在的节点
     */
    @Test
    public void testSearchNonExistingNode() {
        BinarySearchTree bst = new BinarySearchTree();
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);

        TreeNode node = bst.search(10);
        assertNull(node);
    }

    /**
     * 测试中序遍历
     */
    @Test
    public void testInorderTraversal() {
        BinarySearchTree bst = new BinarySearchTree();
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);
        bst.insert(1);
        bst.insert(9);

        List<Integer> result = bst.inorderTraversal();
        assertEquals(5, result.size());
        assertEquals(1, result.get(0));
        assertEquals(3, result.get(1));
        assertEquals(5, result.get(2));
        assertEquals(7, result.get(3));
        assertEquals(9, result.get(4));
    }

    /**
     * 测试空树
     */
    @Test
    public void testEmptyTree() {
        BinarySearchTree bst = new BinarySearchTree();

        assertNull(bst.search(5));
        List<Integer> result = bst.inorderTraversal();
        assertEquals(0, result.size());
    }

    /**
     * 测试插入重复值
     */
    @Test
    public void testDuplicateValues() {
        BinarySearchTree bst = new BinarySearchTree();
        bst.insert(5);
        bst.insert(5);
        bst.insert(5);

        TreeNode root = bst.search(5);
        assertNotNull(root);
        assertNull(root.left);
        assertNull(root.right);
    }
}
