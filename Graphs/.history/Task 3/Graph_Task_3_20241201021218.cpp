#include "../Task 2/Graph_Task_2.cpp" // Include the task 2 file for BFS, DFS, etc.

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Edge case checks
    if (!source || !destination || source == destination) {
        return {source}; // If source and destination are the same, return the source
    }

    vector<shared_ptr<Vertex<T>>> path;
    if (directed && weighted) {
        // Use Dijkstra's algorithm for directed, weighted graphs
        return dijkstraShortestPath(source);
    } else if (!directed && !weighted) {
        // Use BFS for unweighted, undirected graphs
        return BFSTraversal(source);
    } else {
        // Handle other cases (like directed and unweighted or undirected and weighted)
        cout << "Functionality not available for the current graph type" << endl;
        return path;
    }
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    // Topological sort is only possible for Directed Acyclic Graphs (DAG)
    vector<shared_ptr<Vertex<T>>> result;

    if (directed) {
        // Call helper function to perform topological sort using DFS
        vector<shared_ptr<Vertex<T>>> visited;
        for (auto& vertex : vertices) {
            if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
                topologicalSortHelper(vertex, visited, result);
            }
        }
    } else {
        cout << "Functionality not available for undirected graphs" << endl;
    }
    
    return result;
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>(false, true); // Undirected and weighted

    if (!weighted) {
        cout << "Minimum Spanning Tree is not available for unweighted graphs" << endl;
        return mst;
    }

    // Use Prim’s or Kruskal’s algorithm to find the MST
    // Here, we'll use Prim's algorithm for simplicity
    vector<shared_ptr<Vertex<T>>> verticesInMST;
    vector<shared_ptr<Edge<T>>> edgesInMST;

    // Initialize Prim’s algorithm (start from an arbitrary vertex)
    // Assume there's at least one vertex in the graph
    if (!vertices.empty()) {
        shared_ptr<Vertex<T>> start = vertices[0];
        verticesInMST.push_back(start);
        
        while (verticesInMST.size() < vertices.size()) {
            shared_ptr<Edge<T>> minEdge = nullptr;
            shared_ptr<Vertex<T>> nextVertex = nullptr;

            // Find the smallest edge connecting a vertex in MST to a vertex not in MST
            for (auto& edge : edges) {
                if (find(verticesInMST.begin(), verticesInMST.end(), edge->getSource()) != verticesInMST.end() &&
                    find(verticesInMST.begin(), verticesInMST.end(), edge->getDestination()) == verticesInMST.end()) {
                    if (!minEdge || edge->getWeight() < minEdge->getWeight()) {
                        minEdge = edge;
                        nextVertex = edge->getDestination();
                    }
                }
            }

            if (minEdge) {
                edgesInMST.push_back(minEdge);
                verticesInMST.push_back(nextVertex);
            }
        }

        // Add edges and vertices to the MST graph
        for (auto& edge : edgesInMST) {
            mst->addEdge(edge->getSource()->getData(), edge->getDestination()->getData(), edge->getWeight());
        }
    }

    return mst;
}

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> scc;
    
    if (directed) {
        // Use Kosaraju's or Tarjan’s algorithm to find strongly connected components
        // For simplicity, we will use Kosaraju's algorithm
        vector<shared_ptr<Vertex<T>>> visited;
        stack<shared_ptr<Vertex<T>>> finishStack;

        // First DFS to fill the finish stack
        for (auto& vertex : vertices) {
            if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
                DFSTraversalHelper(vertex, visited, finishStack);
            }
        }

        // Reverse the graph (transpose)
        Graph<T> reversedGraph(directed, weighted);
        for (auto& edge : edges) {
            reversedGraph.addEdge(edge->getDestination()->getData(), edge->getSource()->getData(), edge->getWeight());
        }

        // Second DFS on the reversed graph using the finish stack order
        fill(visited.begin(), visited.end(), nullptr);
        while (!finishStack.empty()) {
            shared_ptr<Vertex<T>> vertex = finishStack.top();
            finishStack.pop();

            if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
                vector<shared_ptr<Vertex<T>>> component;
                reversedGraph.DFSTraversalHelper(vertex, visited, component);
                scc.push_back(component);
            }
        }
    } else {
        cout << "Functionality not available for undirected graphs" << endl;
    }
    
    return scc;
}

// ::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::

// BONUS TASK 1: Spanning Trees
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    vector<shared_ptr<Graph<T>>> spanningTrees;

    if (!directed) {
        // Handle undirected graph for spanning trees
        vector<vector<shared_ptr<Vertex<T>>>> components = connectedComponents();
        for (auto& component : components) {
            shared_ptr<Graph<T>> spanningTree = make_shared<Graph<T>>(false, weighted);
            for (auto& vertex : component) {
                spanningTree->addVertex(vertex->getData());
            }
            // Create the spanning tree from edges
            // (You can apply Kruskal’s or Prim’s for each component)
            spanningTrees.push_back(spanningTree);
        }
    } else {
        cout << "Functionality not available for directed graphs" << endl;
    }

    return spanningTrees;
}

// BONUS TASK 2: Connected Components
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> components;

    if (!directed) {
        // Use BFS or DFS to identify connected components for undirected graphs
        vector<shared_ptr<Vertex<T>>> visited;
        for (auto& vertex : vertices) {
            if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
                vector<shared_ptr<Vertex<T>>> component;
                BFSHelper(vertex, visited, component);
                components.push_back(component);
            }
        }
    } else {
        cout << "Functionality not available for directed graphs" << endl;
    }

    return components;
}
