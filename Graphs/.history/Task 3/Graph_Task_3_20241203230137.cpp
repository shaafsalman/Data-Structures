#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 
#include <unordered_set>  
#include <unordered_map>
#include <vector>
#include <memory>
#include <queue>
#include <stack>
#include <iostream>


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
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    // Edge case: If the graph has no vertices, return an empty vector
    if (vertices.empty()) {
        std::cout << "Graph is empty." << std::endl;
        return {};  // Return an empty vector
    }

    // Vector to store the spanning trees for each connected component
    vector<shared_ptr<Graph<T>>> spanningTrees;

    // A set to keep track of visited vertices
    std::unordered_set<shared_ptr<Vertex<T>>> visited;

    // Helper function to perform DFS and build a spanning tree for the component
    auto dfs = [&](shared_ptr<Vertex<T>> vertex, shared_ptr<Graph<T>> componentGraph) {
        std::stack<shared_ptr<Vertex<T>>> stack;
        stack.push(vertex);
        visited.insert(vertex);

        while (!stack.empty()) {
            auto current = stack.top();
            stack.pop();

            // For each adjacent vertex, add the edge to the spanning tree
            for (auto edge : getEdges(current)) {
                auto neighbor = edge->getDestination();

                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    componentGraph->addVertex(neighbor->getData());  // Add vertex to the tree
                    componentGraph->addEdge(current->getData(), neighbor->getData(), edge->getWeight());  // Add edge to the tree
                    stack.push(neighbor);
                }
            }
        }
    };

    // Iterate through each vertex to find all connected components
    for (auto vertex : vertices) {
        if (visited.find(vertex) == visited.end()) {
            // For each unvisited vertex, create a new graph for the spanning tree
            shared_ptr<Graph<T>> componentGraph = make_shared<Graph<T>>(directed, weighted);
            componentGraph->addVertex(vertex->getData());  // Add the initial vertex

            // Perform DFS to build the spanning tree for this component
            dfs(vertex, componentGraph);

            // Add the resulting spanning tree to the list of spanning trees
            spanningTrees.push_back(componentGraph);
        }
    }

    // Return the list of spanning trees
    return spanningTrees;
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
