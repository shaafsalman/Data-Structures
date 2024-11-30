#include "../src/Graph.h"
#include "../Task 1/Graph_Task_1.cpp"
#include <queue>
#include <stack>
#include <unordered_map>
#include <limits>
#include <algorithm> // for reverse

// :::::::::::::::::::::::::::::: TASK 2 :::::::::::::::::::::::::::::::::::

// Breadth-First Search Traversal
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::BFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Breadth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visited;           // Stores the visited vertices in order
    queue<shared_ptr<Vertex<T>>> q;                  // Queue for BFS
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited; // Map to track visited vertices

    q.push(vertex);                                  // Push the start vertex to queue
    isVisited[vertex] = true;                        // Mark start vertex as visited

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        visited.push_back(current);                   // Add current vertex to visited list

        // Traverse all adjacent vertices of the current vertex
        for (auto adjacentVertex : getAdjacentVertices(current)) {
            if (!isVisited[adjacentVertex]) {
                q.push(adjacentVertex);              // Push unvisited adjacent vertices to queue
                isVisited[adjacentVertex] = true;    // Mark adjacent vertex as visited
            }
        }
    }

    return visited;                                  // Return the visited vertices
}

// Depth-First Search Traversal
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Depth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visited;           // Stores the visited vertices in order
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited; // Map to track visited vertices

    DFSTraversalHelper(vertex, visited, isVisited);  // Call the helper function for DFS

    return visited;                                  // Return the visited vertices
}

// Utility function for Depth First Search Traversal
template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited, unordered_map<shared_ptr<Vertex<T>>, bool>& isVisited) {
    // vertex: current vertex
    // visited: vector to store the visited vertices
    // isVisited: map to check if a vertex has been visited

    isVisited[vertex] = true;                        // Mark current vertex as visited
    visited.push_back(vertex);                        // Add current vertex to visited list

    // Visit all adjacent vertices that haven't been visited yet
    for (auto adjacentVertex : getAdjacentVertices(vertex)) {
        if (!isVisited[adjacentVertex]) {
            DFSTraversalHelper(adjacentVertex, visited, isVisited); // Recurse for unvisited adjacent vertices
        }
    }
}

// Dijkstra's Algorithm - returns vertices in the ascending order of their distance from the source vertex
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::dijkstraShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Dijkstra's Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    unordered_map<shared_ptr<Vertex<T>>, int> distances;  // Map to store distances of vertices
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous; // Map to store the previous vertex in the shortest path
    vector<shared_ptr<Vertex<T>>> visited;                // Vector to store the visited vertices
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq; // Min-priority queue for Dijkstra

    // Initialize distances to infinity and previous to null
    for (auto vertex : vertices) {
        distances[vertex] = std::numeric_limits<int>::max();
    }
    distances[source] = 0;
    pq.push({0, source});  // Push the source vertex to the priority queue

    while (!pq.empty()) {
        auto [currentDist, currentVertex] = pq.top();
        pq.pop();

        if (find(visited.begin(), visited.end(), currentVertex) != visited.end()) {
            continue;  // Skip already visited vertices
        }

        visited.push_back(currentVertex);  // Add vertex to visited list

        // Update distances for adjacent vertices
        for (auto adjacentVertex : getAdjacentVertices(currentVertex)) {
            int edgeWeight = getEdge(currentVertex->getData(), adjacentVertex->getData())->getWeight();
            int newDist = currentDist + edgeWeight;

            if (newDist < distances[adjacentVertex]) {
                distances[adjacentVertex] = newDist;  // Update the distance
                previous[adjacentVertex] = currentVertex;  // Update the previous vertex
                pq.push({newDist, adjacentVertex});  // Push the updated vertex to the priority queue
            }
        }
    }

    // Reconstruct the shortest path from the 'previous' map
    vector<shared_ptr<Vertex<T>>> shortestPath;
    for (auto vertex = source; vertex != nullptr; vertex = previous[vertex]) {
        shortestPath.push_back(vertex);
    }

    reverse(shortestPath.begin(), shortestPath.end());  // Reverse the path to get the correct order
    return shortestPath;  // Return the shortest path
}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// Bellman-Ford Algorithm - returns vertices in the ascending order of their distance from the source vertex
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    unordered_map<shared_ptr<Vertex<T>>, int> distances;  // Map to store distances of vertices
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous; // Map to store the previous vertex in the shortest path
    vector<shared_ptr<Vertex<T>>> visited;                // Vector to store the visited vertices

    // Initialize distances to infinity
    for (auto vertex : vertices) {
        distances[vertex] = std::numeric_limits<int>::max();
    }
    distances[source] = 0;

    // Relax edges |V| - 1 times
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        for (auto edge : edges) {
            auto u = edge->getSource();
            auto v = edge->getDestination();
            int weight = edge->getWeight();

            if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;  // Update the previous vertex
            }
        }
    }

    // Check for negative weight cycles
    for (auto edge : edges) {
        auto u = edge->getSource();
        auto v = edge->getDestination();
        int weight = edge->getWeight();

        if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
            cout << "Graph contains a negative weight cycle!" << endl;
            return {};  // Return an empty vector to indicate a negative cycle
        }
    }

    // Reconstruct the shortest path from the 'previous' map
    vector<shared_ptr<Vertex<T>>> shortestPath;
    for (auto vertex = source; vertex != nullptr; vertex = previous[vertex]) {
        shortestPath.push_back(vertex);
    }

    reverse(shortestPath.begin(), shortestPath.end());  // Reverse the path to get the correct order
    return shortestPath;  // Return the shortest path
}
