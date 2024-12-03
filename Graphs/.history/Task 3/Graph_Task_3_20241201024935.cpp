#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 


template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // For unweighted graphs, use BFS to find the shortest path
    if (!weighted) {
        vector<shared_ptr<Vertex<T>>> path;
        vector<shared_ptr<Vertex<T>>> traversal = BFSTraversal(source);

        // To find the shortest path, we'll reconstruct it from BFS traversal
        unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;
        for (size_t i = 1; i < traversal.size(); ++i) {
            parent[traversal[i]] = traversal[i-1];  // Mapping each vertex to its parent
        }

        // Reconstruct the path from destination to source
        shared_ptr<Vertex<T>> current = destination;
        while (current) {
            path.insert(path.begin(), current);
            current = parent[current];
        }

        return path;
    }

    // For weighted graphs, use Dijkstra's algorithm
    vector<shared_ptr<Vertex<T>>> path;
    unordered_map<shared_ptr<Vertex<T>>, int> distances;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq;

    // Initialize distances for all vertices
    for (auto& vertex : vertices) {
        distances[vertex] = INT_MAX;
    }
    distances[source] = 0;
    pq.push({0, source});

    // Dijkstra's algorithm to find the shortest path
    while (!pq.empty()) {
        auto [dist, vertex] = pq.top();
        pq.pop();

        // If we reach the destination, stop
        if (vertex == destination) {
            break;
        }

        // Iterate over adjacent vertices
        for (auto adj : getAdjacentVertices(vertex)) {
            int edgeWeight = getEdge(vertex->getData(), adj->getData())->getWeight();
            int newDist = dist + edgeWeight;

            // If a shorter path is found, update the distance and previous vertex
            if (newDist < distances[adj]) {
                distances[adj] = newDist;
                previous[adj] = vertex;
                pq.push({newDist, adj});
            }
        }
    }

    // Reconstruct the shortest path
    shared_ptr<Vertex<T>> current = destination;
    while (current) {
        path.insert(path.begin(), current);  // Insert at the beginning to reverse the order
        current = previous[current];  // Move to the previous vertex
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

    // Define the DFS lambda function
    function<void(shared_ptr<Vertex<T>>)> dfs = [&](shared_ptr<Vertex<T>> vertex) {
        visited[vertex] = true;  // Mark the vertex as visited
        for (auto& neighbor : getAdjacentVertices(vertex)) {
            if (!visited[neighbor]) {
                dfs(neighbor);  // Recursively visit unvisited neighbors
            }
        }
    };

    // Traverse all vertices in the graph
    for (auto& vertex : vertices) {
        if (!visited[vertex]) {  // If the vertex is not visited
            vector<shared_ptr<Vertex<T>>> component;  // Create a new component

            // Run DFS to explore all reachable vertices from this vertex
            dfs(vertex);

            // After DFS, collect all vertices that were visited during this DFS into the component
            for (auto& v : visited) {
                if (v.second) {  // If the vertex was visited
                    component.push_back(v.first);
                    v.second = false;  // Reset visited status for future searches
                }
            }

            components.push_back(component);  // Add the discovered component to the list
        }
    }

    return components;
}
