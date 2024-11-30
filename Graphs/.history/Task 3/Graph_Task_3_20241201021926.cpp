#include "../Task 2/Graph_Task_2.cpp"

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Find the shortest path between the source and destination vertices
    // Return the vertices in the shortest path
    // source: source vertex for the shortest path
    // destination: destination vertex for the shortest path

    // Make sure to use the correct algorithm based on the type of graph (directed/undirected, weighted/unweighted)

    // Solution:
      if (!weighted) {
        return BFSTraversal(source);  // BFS traversal to get the shortest path in an unweighted graph
    }
    
    // For weighted graphs, use Dijkstra's algorithm
    vector<shared_ptr<Vertex<T>>> path;
    vector<int> distances(vertices.size(), INT_MAX);
    vector<shared_ptr<Vertex<T>>> previous(vertices.size(), nullptr);
    distances[find(vertices.begin(), vertices.end(), source) - vertices.begin()] = 0;

    // Min-heap or priority queue
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [dist, vertex] = pq.top();
        pq.pop();
        
        if (vertex == destination) {
            break;
        }

        for (auto adj : getAdjacentVertices(vertex)) {
            int edgeWeight = getEdge(vertex->getData(), adj->getData())->getWeight();
            int newDist = dist + edgeWeight;
            int adjIdx = find(vertices.begin(), vertices.end(), adj) - vertices.begin();
            if (newDist < distances[adjIdx]) {
                distances[adjIdx] = newDist;
                previous[adjIdx] = vertex;
                pq.push({newDist, adj});
            }
        }
    }
    
    // Reconstruct the shortest path
    shared_ptr<Vertex<T>> current = destination;
    while (current) {
        path.insert(path.begin(), current);
        current = previous[find(vertices.begin(), vertices.end(), current) - vertices.begin()];
    }
    return path;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    // Perform Topological Sort on the graph
    // Return the vertices in the topological order

    // Solution:
     if (!directed) {
        cout << "Topological Sort is only applicable to directed graphs." << endl;
        return {};
    }

    // Calculate in-degrees
    unordered_map<shared_ptr<Vertex<T>>, int> inDegree;
    for (auto vertex : vertices) {
        inDegree[vertex] = 0;  // Initialize all vertices with in-degree 0
    }
    for (auto edge : edges) {
        inDegree[edge->getDestination()]++;
    }

    // Queue for vertices with in-degree 0
    queue<shared_ptr<Vertex<T>>> q;
    for (auto vertex : vertices) {
        if (inDegree[vertex] == 0) {
            q.push(vertex);
        }
    }

    vector<shared_ptr<Vertex<T>>> result;
    while (!q.empty()) {
        auto vertex = q.front();
        q.pop();
        result.push_back(vertex);
        
        for (auto adj : getAdjacentVertices(vertex)) {
            inDegree[adj]--;
            if (inDegree[adj] == 0) {
                q.push(adj);
            }
        }
    }
    
    // If there is a cycle, the result will have fewer vertices
    if (result.size() != vertices.size()) {
        cout << "Graph has a cycle, topological sort is not possible." << endl;
        return {};
    }
    return result;
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    // Find the minimum spanning tree of the graph
    // Return the minimum spanning tree as a Graph object

    // Solution:
      if (directed) {
        cout << "Minimum Spanning Tree is not applicable for directed graphs." << endl;
        return nullptr;
    }

    auto mst = make_shared<Graph<T>>(false, true);  // Create an undirected weighted graph for the MST

    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](const shared_ptr<Edge<T>>& a, const shared_ptr<Edge<T>>& b) {
        return a->getWeight() < b->getWeight();
    });

    // Union-Find to detect cycles
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;
    auto findParent = [&parent](shared_ptr<Vertex<T>> vertex) {
        if (parent[vertex] == nullptr) {
            parent[vertex] = vertex;
        }
        if (parent[vertex] != vertex) {
            parent[vertex] = findParent(parent[vertex]);
        }
        return parent[vertex];
    };

    auto unionSets = [&parent](shared_ptr<Vertex<T>> u, shared_ptr<Vertex<T>> v) {
        shared_ptr<Vertex<T>> rootU = findParent(u);
        shared_ptr<Vertex<T>> rootV = findParent(v);
        if (rootU != rootV) {
            parent[rootU] = rootV;
        }
    };

    // Add edges to the MST
    for (auto edge : edges) {
        shared_ptr<Vertex<T>> u = edge->getSource();
        shared_ptr<Vertex<T>> v = edge->getDestination();
        if (findParent(u) != findParent(v)) {
            mst->addEdge(u->getData(), v->getData(), edge->getWeight());
            unionSets(u, v);
        }
    }

    return mst;

};

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    // Find all the strongly connected components of the graph
    // Return the strongly connected components as a vector of vectors of vertices

    // Solution:
     if (!directed) {
        cout << "SCCs are only applicable to directed graphs." << endl;
        return {};
    }

    // Step 1: Perform a DFS to get the finishing times of vertices
    vector<shared_ptr<Vertex<T>>> order;
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    
    function<void(shared_ptr<Vertex<T>>)> dfs1 = [&](shared_ptr<Vertex<T>> vertex) {
        visited[vertex] = true;
        for (auto adj : getAdjacentVertices(vertex)) {
            if (!visited[adj]) {
                dfs1(adj);
            }
        }
        order.push_back(vertex);  // Add to the order after visiting all neighbors
    };
    
    for (auto vertex : vertices) {
        if (!visited[vertex]) {
            dfs1(vertex);
        }
    }

    // Step 2: Transpose the graph
    shared_ptr<Graph<T>> transposed = make_shared<Graph<T>>(true, weighted);
    for (auto edge : edges) {
        transposed->addEdge(edge->getDestination()->getData(), edge->getSource()->getData(), edge->getWeight());
    }

    // Step 3: Perform DFS on the transposed graph in reverse finishing time order
    vector<vector<shared_ptr<Vertex<T>>>> sccs;
    visited.clear();
    
    function<void(shared_ptr<Vertex<T>>, vector<shared_ptr<Vertex<T>>>&)> dfs2 = [&](shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& component) {
        visited[vertex] = true;
        component.push_back(vertex);
        for (auto adj : transposed->getAdjacentVertices(vertex)) {
            if (!visited[adj]) {
                dfs2(adj, component);
            }
        }
    };

    for (auto vertex : order) {
        if (!visited[vertex]) {
            vector<shared_ptr<Vertex<T>>> component;
            dfs2(vertex, component);
            sccs.push_back(component);
        }
    }

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
    // You can generate spanning trees here using combinations of edges, or by applying Kruskal’s to different subgraphs
    return spanningTrees;
}

// BONUS TASK 2 FOR 5 MARKS
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    // Find all the connected components of the graph
    // Return the connected components as a vector of vectors of vertices

    // Solution:
}