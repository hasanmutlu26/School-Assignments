class Part2{
    public static void main(String[] args) {
        BinarySearchTree<Integer> bst = new BinarySearchTree<Integer>();
        Part2Method<Integer> func = new Part2Method<Integer>();
        AVLTree<Integer> avl = new AVLTree<Integer>();
        bst.add(10);
        bst.add(20);
        bst.add(30);
        bst.add(40);
        bst.add(50);
        bst.add(60);
        bst.add(70);
        bst.add(80);
        System.out.println("TEST1: Linearly inbalanced BinarySearchTree");
        System.out.println(bst.toString());
        System.out.println();
        avl = func.part2func(bst);
        System.out.println("The AVL Tree:");
        System.out.println(avl.toString());

        System.out.println("************************************************");
        BinarySearchTree<Integer> bst2 = new BinarySearchTree<Integer>();
        AVLTree<Integer> avl2 = new AVLTree<Integer>();
        bst2.add(50);
        bst2.add(10);
        bst2.add(60);
        bst2.add(40);
        bst2.add(70);
        bst2.add(80);
        bst2.add(30);
        bst2.add(20);
        System.out.println("TEST2: Inbalanced Binary Search Tree");
        System.out.println(bst2.toString());
        System.out.println();
        System.out.println("AVL Tree");
        avl2 = func.part2func(bst2);
        System.out.println(avl2.toString());

        System.out.println("************************************************");
        BinarySearchTree<Integer> bst3 = new BinarySearchTree<Integer>();
        AVLTree<Integer> avl3 = new AVLTree<Integer>();
        bst3.add(50);
        bst3.add(80);
        bst3.add(20);
        bst3.add(90);
        bst3.add(70);
        bst3.add(10);
        bst3.add(30);
        System.out.println("TEST3: Already Balanced Binary Search Tree");
        System.out.println(bst3.toString());
        System.out.println();
        System.out.println("The AVL Tree");
        avl3 = func.part2func(bst3);
        System.out.println(avl3.toString());
    }
}