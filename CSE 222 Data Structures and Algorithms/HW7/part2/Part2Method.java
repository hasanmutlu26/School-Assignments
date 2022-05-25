class Part2Method<E extends Comparable<E>>{

    /**
     * This is the main method. It is wrapper for the recursive method.
     * @param bst BinarySearchTree to be reorganized
     * @return AVLTree equavelent of the organized BST
     */
    public AVLTree<E> part2func(BinarySearchTree<E> bst){
        bst.root = part2recursive(bst);
        AVLTree<E> avl = new AVLTree<E>();
        copyTree(bst, avl);
        return avl;
    }

    /**
     * Recursive method to rebalance the BST. It rotates the tree around current root until it gets balanced.
     * After that, it continues to balance its children nodes.
     * @param bst BST to be reorganized
     * @return current root of the tree/subtree
     */
    private BinaryTree.Node<E> part2recursive(BinarySearchTree<E> bst){
        if(bst == null || bst.root == null)
            return null;
        int isBalanced;
        int prev = 0;
        while(true){
            isBalanced = isBalanced(bst.root);
            if(isBalanced > 1){
                bst.root = rotateLeft(bst.root);
            }
            else if(isBalanced < -1){
                bst.root = rotateRight(bst.root);
            }
            else{
                break;
            }
            if(isBalanced(bst.root) == prev){
                if(bst.root.left != null)
                    bst.root.left = part2func(bst.getLeftSubtree()).root;
                if(bst.root.right != null)
                    bst.root.right = part2func(bst.getRightSubtree()).root;
            }
            prev = isBalanced;
        }
        if(bst.root.left != null)
            bst.root.left = part2func(bst.getLeftSubtree()).root;
        if(bst.root.right != null)
            bst.root.right = part2func(bst.getRightSubtree()).root;

        return bst.root;
    }
    


    /**
    * Checks if the tree is balanced or unbalanced
    * @param node Current root to be checked
    * @return The rate of balance. It is balanced if value is between -1 and 1. If it is lower than -1, it is unbalanced
    * toward left and vice versa.
    */
    private int isBalanced(BinaryTree.Node<E> root) {

        if (root == null)
            return 0;

        int heightLeft = height(root.left);
        int heightRight = height(root.right);

        return heightRight - heightLeft;
    }

    /**
    * Finds a node's height
    * @param node Node to be checked
    * @return height of the node.
    */
    private int height(BinaryTree.Node<E> node) {
        if (node == null)
            return 0;

        return 1 + Math.max(height(node.left), height(node.right));

    }

    /** Method to perform a right rotation.
    pre:  root is the root of a binary search tree.
    post: root.right is the root of a binary search tree,
        root.right.right is raised one level,
        root.right.left does not change levels,
        root.left is lowered one level,
        the new root is returned.
    @param root The root of the binary tree to be rotated
    @return The new root of the rotated tree
    */
    private BinaryTree.Node < E > rotateRight(BinaryTree.Node < E > root) {
        BinaryTree.Node < E > temp = root.left;
        root.left = temp.right;
        temp.right = root;
        return temp;
    }

   /** rotateLeft
        pre:  localRoot is the root of a binary search tree
        post: localRoot.right is the root of a binary search tree
                localRoot.right.right is raised one level
                localRoot.right.left does not change levels
                localRoot.left is lowered one level
                the new localRoot is returned.
        @param localRoot The root of the binary tree to be rotated
        @return the new root of the rotated tree
    */
    private BinaryTree.Node < E > rotateLeft(BinaryTree.Node < E > localRoot) {
        BinaryTree.Node < E > temp = localRoot.right;
        localRoot.right = temp.left;
        temp.left = localRoot;
        return temp;
   }

    /**
     * Copies a BinarySearchTree into an AVLTree
     * @param bst Source BinarySearchTree
     * @param avl Destination AVLTree
     */
    private void copyTree(BinarySearchTree<E> bst, AVLTree<E> avl){
        if(bst == null || bst.root == null)
            return;

        if(avl.root == null)
            avl.root = new BinaryTree.Node<E>(bst.root.data);
        else
            avl.root.data = bst.root.data;

        if(bst.getLeftSubtree() != null){
            avl.root.left = new BinaryTree.Node<E>(null);
            copyTree(bst.getLeftSubtree(), avl.getLeftSubtree());
        }
        
        if(bst.getRightSubtree() != null){
            avl.root.right = new BinaryTree.Node<E>(null);
            copyTree(bst.getRightSubtree(), avl.getRightSubtree());
        }
    }
}