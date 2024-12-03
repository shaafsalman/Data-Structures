#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 


template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Find the shortest path between the source and destination vertices
    // Return the vertices in the shortest path
    // source: source vertex for the shortest path
    // destination: destination vertex for the shortest path

    // Make sure to use the correct algorithm based on the type of graph (directed/undirected, weighted/unweighted)

    // Solution:
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
    // Check if the graph is empty
    if (vertices.empty()) {
        cout << "Graph is empty." << endl;
        return nullptr;
    }

    // Initialize an empty graph to hold the MST
    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>(directed, weighted);

    // A priority queue to store edges, ordered by their weight
    auto compare = [](shared_ptr<Edge<T>> edge1, shared_ptr<Edge<T>> edge2) {
        return edge1->getWeight() > edge2->getWeight();
    };
    priority_queue<shared_ptr<Edge<T>>, vector<shared_ptr<Edge<T>>>, decltype(compare)> pq(compare);

    // A set to keep track of the visited vertices
    unordered_set<shared_ptr<Vertex<T>>> visited;

    // Start with an arbitrary vertex (let's use the first one)
    auto startVertex = vertices[0];
    visited.insert(startVertex);

    // Add all the edges of the start vertex to the priority queue
    for (auto& edge : getEdges(startVertex)) {
        pq.push(edge);
    }

    // Perform Prim's algorithm
    while (!pq.empty()) {
        // Get the edge with the minimum weight
        auto edge = pq.top();
        pq.pop();

        // Get the destination vertex of the edge
        auto neighbor = edge->getDestination();

        // If the destination vertex is not visited, add it to the MST
        if (visited.find(neighbor) == visited.end()) {
            // Add the edge to the MST
            mst->addEdge(edge->getSource()->getData(), neighbor->getData(), edge->getWeight());

            // Mark the neighbor as visited
            visited.insert(neighbor);

            // Add all the edges from the new vertex to the priority queue
            for (auto& newEdge : getEdges(neighbor)) {
                if (visited.find(newEdge->getDestination()) == visited.end()) {
                    pq.push(newEdge);
                }
            }
        }
    }

    // Return the MST
    return mst;
}
















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
