import java.util.Arrays;

class Part1Method<E extends Comparable <E>>{
    /**
     * Wrapper method to create the tree.
     * @param binTree binary tree
     * @param arr the array
     * @return A BinarySearchTree which contains the elements in the array and is in the form of binTree. 
     */
    public BinarySearchTree<E> part1func(BinaryTree<E> binTree, E[] arr) {
        BinarySearchTree<E> bst = new BinarySearchTree<E>();        //create BST
        Arrays.sort(arr);           //sort the array
        part1recursive(binTree, arr, bst, 0);   //call recursive method
        return bst;
      }
    
      /**
       * The recursive method to create the tree.
       * @param binTree current BinaryTree tree/subtree
       * @param arr the array
       * @param bst current BinarySearchTree tree/subtree
       * @param leftInd index to start while choosing element from the array. It's first value must be 0.
       */
    private void part1recursive(BinaryTree<E> binTree, E[] arr, BinarySearchTree<E> bst, int leftInd){
        //If tree is null, return
        if(binTree == null)
            return;

        //The index of the array element to insert is chosen.
        int index;
        if(binTree.root.left == null)
            index = 0;
        else
            index = countNode(binTree.getLeftSubtree().root);

        //The index is summed with leftInd and the element in the resulting index is assigned to BST's current subtree root.
        bst.root.data = arr[index + leftInd];

        //The two conditions below are kind of base cases as well.

        //If there is a left subtree, create a left node in BST and call the function with left subtrees and leftInd
        if(binTree.getLeftSubtree() != null){
            bst.root.left = new BinaryTree.Node<E>(null);
            part1recursive(binTree.getLeftSubtree(), arr, bst.getLeftSubtree(), leftInd);
        }

        //If there is a right subtree, create a right node in BST and call the function, incrementing leftInd by index + 1
        //By this way, leftInd in the next function will be the index of starting of right half of the array.
        if(binTree.getRightSubtree() != null){
            bst.root.right = new BinaryTree.Node<E>(null);
            part1recursive(binTree.getRightSubtree(), arr, bst.getRightSubtree(), index + leftInd + 1);
        }
    }

    
    public int countNode(BinaryTree.Node<E> root){

        //base case
        if(root==null)
            return 0;

        //recursive call to left child and right child and
        // add the result of these with 1 ( 1 for counting the root)
        return 1 + countNode(root.left) + countNode(root.right);
    }
}