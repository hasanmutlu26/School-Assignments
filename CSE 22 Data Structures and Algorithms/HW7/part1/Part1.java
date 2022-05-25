import java.util.Arrays;
import java.util.Random;

class Part1{
    public static void main(String[] args) {
        Part1Method<Integer> func = new Part1Method<Integer>();
        BinaryTree<Integer> t1 = new BinaryTree<Integer>(0, null, null);
        BinaryTree<Integer> t2 = new BinaryTree<Integer>(0, null, null);
        BinaryTree<Integer> t3 = new BinaryTree<Integer>(0, t1, t2);
        BinaryTree<Integer> t4 = new BinaryTree<Integer>(0, t3, null);
        BinaryTree<Integer> t5 = new BinaryTree<Integer>(0, null, null);
        BinaryTree<Integer> t6 = new BinaryTree<Integer>(0, t5, null);
        BinaryTree<Integer> binTree = new BinaryTree<Integer>(0, t6, t4);

        System.out.println("TEST 1: Random unbalanced binary tree");
        System.out.println("The binary tree:");
        System.out.println(binTree.toString());
        //System.out.println(func.countNode(binTree.root) + " - " + binTree.getLeftSubtree().countNode() + " " + binTree.getRightSubtree().countNode());
        BinarySearchTree<Integer> bst = new BinarySearchTree<Integer>();
        
        Integer[] arr = new Integer[func.countNode(binTree.root)];
        Random rand = new Random();
        for(int i = 0; i < func.countNode(binTree.root); i++){
            arr[i] = rand.nextInt(100);
        }
        Arrays.sort(arr);
        System.out.println("The random numbers in the array: ");
        for(int i = 0; i < func.countNode(binTree.root); i++){
            System.out.print(arr[i] + " ");
        }
        System.out.println();System.out.println();

        bst = func.part1func(binTree, arr);
        System.out.println("The binary search tree:");
        System.out.println(bst.toString());
        System.out.println();System.out.println("****************************************");
        /////////////////////////////////////////

        t1 = new BinaryTree<Integer>(0, null, null);
        t2 = new BinaryTree<Integer>(0, null, t1);
        t3 = new BinaryTree<Integer>(0, null, t2);
        t4 = new BinaryTree<Integer>(0, null, t3);
        t5 = new BinaryTree<Integer>(0, null, t4);
        t6 = new BinaryTree<Integer>(0, null, t5);
        binTree = new BinaryTree<Integer>(0, null, t6);

        System.out.println("TEST 2: Linearly unbalanced binary tree");
        System.out.println("The binary tree:");
        System.out.println(binTree.toString());
        //System.out.println(func.countNode(binTree.root) + " - " + binTree.getLeftSubtree().countNode() + " " + binTree.getRightSubtree().countNode());
        
        arr = new Integer[func.countNode(binTree.root)];
        for(int i = 0; i < func.countNode(binTree.root); i++){
            arr[i] = rand.nextInt(100);
        }
        Arrays.sort(arr);
        System.out.println("The random numbers in the array: ");
        for(int i = 0; i < func.countNode(binTree.root); i++){
            System.out.print(arr[i] + " ");
        }
        System.out.println();System.out.println();

        bst = func.part1func(binTree, arr);
        System.out.println("The binary search tree:");
        System.out.println(bst.toString());
        System.out.println();System.out.println("****************************************");
        //////////////////////////////////
        t1 = new BinaryTree<Integer>(0, null, null);
        t2 = new BinaryTree<Integer>(0, null, null);
        t3 = new BinaryTree<Integer>(0, t1, t2);
        t4 = new BinaryTree<Integer>(0, null, null);
        t5 = new BinaryTree<Integer>(0, null, null);
        t6 = new BinaryTree<Integer>(0, t5, t4);
        binTree = new BinaryTree<Integer>(0, t3, t6);

        System.out.println("TEST 3: Balanced binary tree");
        System.out.println("The binary tree:");
        System.out.println(binTree.toString());
        //System.out.println(func.countNode(binTree.root) + " - " + binTree.getLeftSubtree().countNode() + " " + binTree.getRightSubtree().countNode());
        
        arr = new Integer[func.countNode(binTree.root)];
        for(int i = 0; i < func.countNode(binTree.root); i++){
            arr[i] = rand.nextInt(100);
        }
        Arrays.sort(arr);
        System.out.println();
        System.out.println("The random numbers in the array: ");
        for(int i = 0; i < func.countNode(binTree.root); i++){
            System.out.print(arr[i] + " ");
        }
        System.out.println();System.out.println();

        bst = func.part1func(binTree, arr);
        System.out.println("The binary search tree:");
        System.out.println(bst.toString());
    }
}