#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // For unweighted graphs, use BFS to find the shortest path
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
};

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    // Find all the strongly connected components of the graph
    // Return the strongly connected components as a vector of vectors of vertices

    // Solution:
}

// ::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::

// BONUS TASK 1 FOR 5 MARKS
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    // Find all the spanning trees of the graph
    // Return the spanning trees as a vector of Graph objects

    // Solution:
}

// BONUS TASK 2 FOR 5 MARKS
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    // Find all the connected components of the graph
    // Return the connected components as a vector of vectors of vertices

    vector<vector<shared_ptr<Vertex<T>>>> components;
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;

    function<void(shared_ptr<Vertex<T>>)> dfs = [&](shared_ptr<Vertex<T>> vertex) {
        visited[vertex] = true;
        for (auto& neighbor : getAdjacentVertices(vertex)) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };

    for (auto& vertex : vertices) {
        if (!visited[vertex]) {
            vector<shared_ptr<Vertex<T>>> component;
            dfs(vertex);
            for (auto& v : visited) {
                if (v.second) {
                    component.push_back(v.first);
                    visited[v.first] = false;
                }
            }
            components.push_back(component);
        }
    }

    return components;
}
