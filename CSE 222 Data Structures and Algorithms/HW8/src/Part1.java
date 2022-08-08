public class Part1{
    public static void main(String[] args) {
        MyGraph g = new MyGraph(false);
        g.addVertex(g.newVertex("Eskişehir", 788));
        g.addVertex(g.newVertex("İstanbul", 40));
        g.addVertex(g.newVertex("Ankara", 938));
        g.addVertex(g.newVertex("İzmir", 2));
        g.addVertex(g.newVertex("Kocaeli", 4));

        g.addEdge(0, 1, 350);
        g.addEdge(0, 2, 250);
        g.addEdge(0, 3, 500);
        g.addEdge(0, 4, 250);
        
        g.addEdge(1, 2, 500);
        g.addEdge(1, 3, 400);
        g.addEdge(1, 4, 100);

        g.addEdge(2, 3, 400);
        g.addEdge(2, 4, 350);

        g.addEdge(3, 4, 350);
        
        System.out.println("Test Add:");
        System.out.print(g.printGraph());
        System.out.println();
        
        g.removeEdge(3, 4);
        g.removeEdge(0, 1);
        g.removeEdge(0, 2);

        System.out.println("Test remove edge:");
        System.out.println(g.printGraph());

        g.removeVertex("İzmir");
        g.removeVertex(0);

        System.out.println("Test remove vertex:");
        System.out.print(g.printGraph());
    
    }
}