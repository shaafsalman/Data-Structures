#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 


template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Make sure source and destination are valid
    if (!source || !destination) {
        return {}; // Return an empty vector if either vertex is nullptr
    }

    // If source and destination are the same, return the source as the only vertex in the path
    if (source == destination) {
        return {source};
    }

    // Make sure the graph is not empty
    if (vertices.empty()) {
        return {}; // Return an empty vector if the graph has no vertices
    }

    // Initialize structures to store the shortest path, visited vertices, and distances
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous; // To reconstruct the path
    unordered_map<shared_ptr<Vertex<T>>, double> distances; // Distance from source to each vertex
    for (auto& vertex : vertices) {
        distances[vertex] = numeric_limits<double>::infinity(); // Initialize all distances to infinity
    }
    distances[source] = 0; // Distance to the source is 0

    // Priority queue (min-heap) to select the vertex with the smallest distance
    auto compare = [](const pair<shared_ptr<Vertex<T>>, double>& a, const pair<shared_ptr<Vertex<T>>, double>& b) {
        return a.second > b.second;
    };
    priority_queue<pair<shared_ptr<Vertex<T>>, double>, vector<pair<shared_ptr<Vertex<T>>, double>>, decltype(compare)> pq(compare);

    pq.push({source, 0}); // Push the source vertex with a distance of 0

    // Dijkstra's algorithm for finding the shortest path
    while (!pq.empty()) {
        auto current = pq.top().first;
        pq.pop();

        // If we reach the destination, reconstruct and return the path
        if (current == destination) {
            vector<shared_ptr<Vertex<T>>> path;
            for (auto at = destination; at != nullptr; at = previous[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end()); // Reverse the path to get the correct order
            return path;
        }

        // Iterate over adjacent vertices
        for (auto& neighbor : current->getNeighbors()) {
            double newDist = distances[current] + current->getEdgeWeight(neighbor); // Calculate the tentative distance
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = current;
                pq.push({neighbor, newDist});
            }
        }
    }

    // If no path exists, return an empty vector
    return {};
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
