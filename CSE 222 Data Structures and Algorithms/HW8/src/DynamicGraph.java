public interface DynamicGraph extends Graph{
    
    /**
     * Generate a new vertex by given parameters
     * @param label label of vertex
     * @param weight weight of vertex
     * @return the generated vertex
     */
    Vertex newVertex(String label, double weight);

    /**
     * Add the given vertex to the graph
     * @param new_vertex Vertex to be added
     * @return true always
     */
    boolean addVertex (Vertex new_vertex);


    /**
     * Add an edge between the given two vertices in the graph.
     * @param vertexID1 id of source vertex
     * @param vertexID2 id of destination vertex
     * @param weight weight of the edge
     * @return true if it is added. false if edge already exists.
     */
    boolean addEdge (int vertexID1, int vertexID2, double weight);


    /**
     * Remove the edge between the given two vertices.
     * @param vertexID1 source vertex 
     * @param vertexID2 destination vertex
     * @return true if removed. false if it does not exist.
     */
    boolean removeEdge (int vertexID1, int vertexID2);


    /**
     * Remove the vertex from the graph with respect to the given vertex id.
     * @param vertexID vertex to be removed
     * @return true if removed. false if it does not exist.
     */
    boolean removeVertex (int vertexID);


    /**
     * Remove the vertices that have the given label from the graph.
     * @param label label of the vertex to be removed
     * @return true if removed, false if it does not exist.
     */
    boolean removeVertex (String label);


    /**
     * : Filter the vertices by the given user-defined property and returns a subgraph of the graph.
     * @param key 
     * @param filter
     * @return
     */
    //boolean filterVertices (String key, String filter);
    
    /**
     * : Generate the adjacency matrix representation of the graph and returns the matrix.
     * @return adjacency matrix representation of the graph
     */
    //Vertex[][] exportMatrix();

    /**
     * Print the graph in adjacency list format.
     * @return
     */
    String printGraph();
}