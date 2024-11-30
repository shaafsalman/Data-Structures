#include "../Task 2/Graph_Task_2.cpp"  // Assuming you have the helper implementations like DFS, BFS, etc.

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    vector<shared_ptr<Vertex<T>>> path;
    
    // Edge case: if source and destination are the same, return the source vertex
    if (source == destination) {
        path.push_back(source);
        return path;
    }

    if (directed && weighted) {
        // Use Dijkstra's algorithm for directed and weighted graph
        path = dijkstraShortestPath(source);
    } else if (!directed && !weighted) {
        // Use BFS for unweighted and undirected graph
        path = BFSTraversal(source);
    } else {
        cout << "Functionality not available for the current graph type" << endl;
    }
    
    return path;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    vector<shared_ptr<Vertex<T>>> result;
    
    if (directed) {
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
    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>(false, true);  // Undirected and weighted graph

    if (!weighted) {
        cout << "Minimum Spanning Tree is not available for unweighted graphs" << endl;
        return mst;
    }

    vector<shared_ptr<Vertex<T>>> verticesInMST;
    vector<shared_ptr<Edge<T>>> edgesInMST;

    if (!vertices.empty()) {
        shared_ptr<Vertex<T>> start = vertices[0];
        verticesInMST.push_back(start);

        while (verticesInMST.size() < vertices.size()) {
            shared_ptr<Edge<T>> minEdge = nullptr;
            shared_ptr<Vertex<T>> nextVertex = nullptr;

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
        vector<shared_ptr<Vertex<T>>> visited;
        stack<shared_ptr<Vertex<T>>> finishStack;

        for (auto& vertex : vertices) {
            if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
                DFSTraversalHelper(vertex, visited, finishStack);
            }
        }

        Graph<T> reversedGraph(directed, weighted);
        for (auto& edge : edges) {
            reversedGraph.addEdge(edge->getDestination()->getData(), edge->getSource()->getData(), edge->getWeight());
        }

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
        vector<vector<shared_ptr<Vertex<T>>>> components = connectedComponents();
        for (auto& component : components) {
            shared_ptr<Graph<T>> spanningTree = make_shared<Graph<T>>(false, weighted);
            for (auto& vertex : component) {
                spanningTree->addVertex(vertex->getData());
            }
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
