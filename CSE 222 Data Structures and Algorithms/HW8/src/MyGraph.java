import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.ListIterator;

public class MyGraph implements DynamicGraph{
    private boolean isDirected;
    private ArrayList<Vertex> vertices;
    private ArrayList<LinkedList<Edge>> edges;

    public MyGraph(){
        isDirected = false;
        vertices = new ArrayList<Vertex>();
        edges = new ArrayList<LinkedList<Edge>>();
    }

    public MyGraph(boolean isDirected){
        this.isDirected = isDirected;
        vertices = new ArrayList<Vertex>();
        edges = new ArrayList<LinkedList<Edge>>();
    }

    public int getNumV(){
        return vertices.size();
    }

    public boolean isDirected(){
        return isDirected;
    }

    public void insert(Edge edge){
        int src = edge.getSource();
        int dst = edge.getDest();

        if(src >= 0 && src < vertices.size() && dst >= 0 && dst < vertices.size()){
            edges.get(src).add(edge);
        }
    }

    public boolean isEdge(int source, int dest){
        ListIterator<Edge> iter;
        Edge e;
        
        if(source >= 0 && source < vertices.size()){
            iter = edges.get(source).listIterator();
            while(iter.hasNext()){
                e = iter.next();
                if(e.getSource() == source && e.getDest() == dest){
                    return true;
                }
            }
        }
        
        return false;
    }


    public Edge getEdge(int source, int dest){
        ListIterator<Edge> iter;
        Edge e;

        if(source >= 0 && source < vertices.size()){
            iter = edges.get(source).listIterator();
            while(iter.hasNext()){
                e = iter.next();
                if(e.getSource() == source && e.getDest() == dest){
                    return e;
                }
            }
        }
        return new Edge(0, 0, Double.POSITIVE_INFINITY);
    }


    public Iterator<Edge> edgeIterator(int source){
        ListIterator<Edge> iter;
        if(source >= 0 && source < vertices.size()){
            iter = edges.get(source).listIterator();
            return iter;
        }
        return null;
    }

    public Vertex newVertex(String label, double weight){
        Vertex v = new Vertex(label, weight);
        return v;
    }

    public boolean addVertex(Vertex new_vertex){
        vertices.add(new_vertex);
        edges.add(new LinkedList<Edge>());
        return true;
    }

    public boolean addEdge(int vertexID1, int vertexID2, double weight){
        Edge e;
        ListIterator<Edge> iter;
        if(vertexID1 >= 0 && vertexID1 < vertices.size() && vertexID2 >= 0 && vertexID2 < vertices.size()){
            e = new Edge(vertexID1, vertexID2, weight);
            iter = edges.get(vertexID1).listIterator();
            while(iter.hasNext()){
                Edge e2 = iter.next();
                if(e2.getSource() == vertexID1 && e2.getDest() == vertexID2){
                    return false;
                }
            }
            edges.get(vertexID1).add(e);
            if(!isDirected){
                e = new Edge(vertexID2, vertexID1, weight);
                edges.get(vertexID2).add(e);
            }
            return true;
        }
        return false;
    }

    public boolean removeEdge(int vertexID1, int vertexID2){
        Edge e;
        ListIterator<Edge> iter;
        if(vertexID1 >= 0 && vertexID1 < vertices.size() && vertexID2 >= 0 && vertexID2 < vertices.size()){
            iter = edges.get(vertexID1).listIterator();
            while(iter.hasNext()){
                e = iter.next();
                if(e.getSource() == vertexID1 && e.getDest() == vertexID2){
                    iter.remove();
                    if(!isDirected()){
                        iter = edges.get(vertexID2).listIterator();
                        while(iter.hasNext()){
                            e = iter.next();
                            if(e.getSource() == vertexID2 && e.getDest() == vertexID1){
                                iter.remove();
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    public boolean removeVertex(int vertexID){
        Edge e;
        ListIterator<Edge> iter;
        if(vertexID >= 0 && vertexID < vertices.size()){
            vertices.remove(vertexID);
            edges.remove(vertexID);
            for(int i = 0; i < edges.size(); i++){
                iter = edges.get(i).listIterator();
                while(iter.hasNext()){
                    e = iter.next();
                    if(e.getSource() == vertexID || e.getDest() == vertexID){
                        iter.remove();
                    }
                    if(e.getSource() > vertexID && e.getDest() > vertexID){
                        e.setSource(e.getSource() - 1);
                        e.setDest(e.getDest() - 1);
                    }
                    else if(e.getSource() > vertexID){
                        e.setSource(e.getSource() - 1);
                    }
                    else if(e.getDest() > vertexID){
                        e.setDest(e.getDest() - 1);
                    }
                }
            }
            return true;
        }
        return false;
    }

    public boolean removeVertex(String label){
        for(int i = 0; i < vertices.size(); i++){
            if(vertices.get(i).getLabel().equals(label)){
                return removeVertex(i);
            }
        }
        return false;
    }
    
    public String printGraph(){
        StringBuilder sb = new StringBuilder();
        ListIterator<Edge> iter;
        Edge e;
        Vertex v1;
        Vertex v2;
        for(int i = 0; i < vertices.size(); i++){
            sb.append("V(" + vertices.get(i).getLabel());
            if(vertices.get(i).getWeight() != Double.POSITIVE_INFINITY){
                sb.append( ", " + vertices.get(i).getWeight());
            }
            sb.append("):-> ");
            iter = edges.get(i).listIterator();
            while(iter.hasNext()){
                e = iter.next();
                v1 = vertices.get(e.getSource());
                v2 = vertices.get(e.getDest());
                sb.append("E(" + v1.getLabel() + ", " + v2.getLabel());
                if(e.getWeight() != Double.POSITIVE_INFINITY){
                    sb.append(", " + e.getWeight());
                }
                sb.append(")");

                if(iter.hasNext()){
                    sb.append(" -> ");
                }
            }
            sb.append("\n");
        }
        return sb.toString();
    }

}