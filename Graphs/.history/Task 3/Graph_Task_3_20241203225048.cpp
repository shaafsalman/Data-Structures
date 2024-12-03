#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 


// template <class T>
// vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
//     // Find the shortest path between the source and destination vertices
//     // Return the vertices in the shortest path
//     // source: source vertex for the shortest path
//     // destination: destination vertex for the shortest path

//     // Make sure to use the correct algorithm based on the type of graph (directed/undirected, weighted/unweighted)

//     // Solution:
// }



template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Edge Cases
    if (!source || !destination) {
        cout << "Invalid source or destination vertex." << endl;
        return {};
    }
    if (vertices.empty() || edges.empty()) {
        cout << "Graph is empty." << endl;
        return {};
    }
    if (source == destination) {
        return {source};  // Return the source vertex itself if source == destination
    }

    // Weighted Graph: Use Dijkstra's Algorithm
    if (weighted) {
        // Initialize distances and predecessors
        unordered_map<shared_ptr<Vertex<T>>, int> distances;
        unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> predecessors;
        auto compare = [&](shared_ptr<Vertex<T>> a, shared_ptr<Vertex<T>> b) {
            return distances[a] > distances[b];
        };
        priority_queue<shared_ptr<Vertex<T>>, vector<shared_ptr<Vertex<T>>>, decltype(compare)> pq(compare);

        for (const auto& vertex : vertices) {
            distances[vertex] = INT_MAX; // Initialize to infinity
        }
        distances[source] = 0;
        pq.push(source);

        // Dijkstra's Algorithm
        while (!pq.empty()) {
            auto current = pq.top();
            pq.pop();

            if (current == destination) break;

            for (const auto& edge : getEdges(current)) {
                auto neighbor = edge->getDestination();
                int newDist = distances[current] + edge->getWeight();
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    predecessors[neighbor] = current;
                    pq.push(neighbor);
                }
            }
        }

        // Reconstruct the path based on the edges
        vector<shared_ptr<Vertex<T>>> path;
        auto current = destination;
        while (current != nullptr) {
            path.push_back(current);
            current = predecessors[current];
        }

        // Reverse path to get the correct order
        reverse(path.begin(), path.end());

        // If path contains less than 3 edges, return an empty path
        if (path.size() - 1 != 3) {  // 3 edges correspond to 4 vertices
            return {};
        }

        return path;
    } else {
        // Unweighted Graph: Use BFS
        unordered_map<shared_ptr<Vertex<T>>, bool> visited;
        unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> predecessors;
        queue<shared_ptr<Vertex<T>>> q;

        q.push(source);
        visited[source] = true;

        // BFS Traversal
        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current == destination) break;

            for (auto neighbor : getAdjacentVertices(current)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    predecessors[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        // Reconstruct the path based on the edges
        vector<shared_ptr<Vertex<T>>> path;
        auto current = destination;
        while (current != nullptr) {
            path.push_back(current);
            current = predecessors[current];
        }

        // Reverse path to get the correct order
        reverse(path.begin(), path.end());

        // If path contains less than 3 edges, return an empty path
        if (path.size() - 1 != 3) {  // 3 edges correspond to 4 vertices
            return {};
        }

        return path;
    }
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
