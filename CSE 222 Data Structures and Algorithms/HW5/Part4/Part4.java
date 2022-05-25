public class Part4{
    public static void main(String[] args) {
        System.out.println("Homework 5 Part 4 Driver Program");
        BinarySearchArrayTree<Integer> tree = new BinarySearchArrayTree<Integer>(50);
        System.out.println("Create tree1 and add \n50, 75, 25, 87, 95, 12, 35, 45, 65, 60, 55, 90, 100");
        tree.add(75);
        tree.add(25);
        tree.add(87);
        tree.add(95);
        tree.add(12);
        tree.add(35);
        tree.add(45);
        tree.add(65);
        tree.add(60);
        tree.add(55);
        tree.add(90);
        tree.add(100);

        System.out.println("Tree1: ");
        System.out.println(tree.toString());

        System.out.println();
        BinarySearchArrayTree<Integer> tree2 = new BinarySearchArrayTree<Integer>();
        System.out.println("Create tree2 and add \n1, 2, 3, 4, 5, 6");
        tree2.add(1);
        tree2.add(2);
        tree2.add(3);
        tree2.add(4);
        tree2.add(5);
        tree2.add(6);
        System.out.println("Tree2: (It must be linear)");
        System.out.println(tree2.toString());
        System.out.println();

        System.out.print("Tree 1 contains 55: ");
        if(tree.contains(55)){
            System.out.println("True");
        }
        else{
            System.out.println("False");
        }
        System.out.print("Tree 1 contains 48: ");
        if(tree.contains(48)){
            System.out.println("True");
        }
        else{
            System.out.println("False");
        }

    }
}