#include "../Task 2/Graph_Task_2.cpp"

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Find the shortest path between the source and destination vertices
    // Return the vertices in the shortest path
    // source: source vertex for the shortest path
    // destination: destination vertex for the shortest path

    // Make sure to use the correct algorithm based on the type of graph (directed/undirected, weighted/unweighted)

    // Solution:
    vector<shared_ptr<Vertex<T>>> path;
    if (source == nullptr || destination == nullptr) return path;

    // Assuming Dijkstra's or BFS based on graph type can be used here
    // Implementation of appropriate algorithm based on the graph type
    // (e.g., Dijkstra for weighted graphs or BFS for unweighted)

    return path;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    // Perform Topological Sort on the graph
    // Return the vertices in the topological order

    // Solution:
    vector<shared_ptr<Vertex<T>>> sortedVertices;

    // For topological sorting, use DFS or Kahn's algorithm (for detecting cycles)
    // Implementation of topological sort algorithm for directed acyclic graphs (DAG)
    
    return sortedVertices;
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    // Find the minimum spanning tree of the graph
    // Return the minimum spanning tree as a Graph object

    // Solution:
    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>();

    // Use Kruskal's or Prim's algorithm to find the minimum spanning tree
    // Implementation of minimum spanning tree algorithm
    
    return mst;
}

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    // Find all the strongly connected components of the graph
    // Return the strongly connected components as a vector of vectors of vertices

    // Solution:
    vector<vector<shared_ptr<Vertex<T>>>> sccs;

    // Use Kosaraju's or Tarjan's algorithm to find strongly connected components
    // Implementation of SCC detection algorithm
    
    return sccs;
}

// ::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::

// BONUS TASK 1 FOR 5 MARKS
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    // Find all the spanning trees of the graph
    // Return the spanning trees as a vector of Graph objects

    // Solution:
    vector<shared_ptr<Graph<T>>> spanningTrees;

    // Use a variation of DFS/BFS or a spanning tree algorithm like Kruskal's
    // to find all spanning trees in the graph
    
    return spanningTrees;
}

// BONUS TASK 2 FOR 5 MARKS
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    // Find all the connected components of the graph
    // Return the connected components as a vector of vectors of vertices

    // Solution:
    vector<vector<shared_ptr<Vertex<T>>>> components;

    // Use DFS/BFS for undirected graphs to find all connected components
    // Implementation for finding connected components
    
    return components;
}
