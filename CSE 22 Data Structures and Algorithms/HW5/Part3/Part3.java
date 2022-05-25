public class Part3{
    public static void main(String[] args) {
        System.out.println("Homework 5 Part 3 Driver Program");
        System.out.println("Create Heap1 and add 50, 40, 60, 70, 80, 30, 20, 10, 0");
        BinaryHeap<Integer> heap = new BinaryHeap<Integer>(50);
        //heap.add(5);
        heap.add(40);
        heap.add(60);
        heap.add(70);
        heap.add(80);
        heap.add(30);
        heap.add(20);
        heap.add(10);
        heap.add(0);
        System.out.println("Heap 1:");
        System.out.println(heap.toString());

        System.out.println();
        System.out.println("Delete 0, 30 and 60");
        heap.delete();
        heap.delete(30);
        heap.delete(60);
        System.out.println("Heap1 after deletion:");
        System.out.println(heap.toString());

        System.out.println();

        BinaryHeap<Integer> heap2 = new BinaryHeap<Integer>();
        System.out.println("Create Heap2 and add 55, 45, 65, 85, 75, 25, 35, 5, 15");
        heap2.add(55);
        heap2.add(45);
        heap2.add(65);
        heap2.add(85);
        heap2.add(75);
        heap2.add(25);
        heap2.add(35);
        heap2.add(5);
        heap2.add(15);
        System.out.println("Heap2:");
        System.out.println(heap2.toString());

        System.out.println();
        System.out.println("Delete 45, 25 and 85");
        System.out.println("Heap2 after deletion:");
        heap2.delete(45);
        heap2.delete(25);
        heap2.delete(85);
        System.out.println(heap2.toString());


        System.out.println();
        BinaryHeap<Integer> mergedHeap = heap.merge(heap2);
        System.out.println("Merge two heaps:");
        System.out.println(mergedHeap.toString());
        
    }
}