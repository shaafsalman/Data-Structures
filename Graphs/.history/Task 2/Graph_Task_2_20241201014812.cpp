#include "../src/Graph.h"
#include "../Task 1/Graph_Task_1.cpp"
#include <queue>
#include <stack>
#include <unordered_map>   
#include <limits>    

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::BFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Breadth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visited;  // Stores the visited vertices in order
    queue<shared_ptr<Vertex<T>>> q;  // Queue for BFS
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;  // Map to track visited vertices

    // Start BFS from the given vertex
    q.push(vertex);
    isVisited[vertex] = true;

    while (!q.empty()) {
        auto currentVertex = q.front();  // Get the front vertex in the queue
        q.pop();  // Remove the vertex from the queue
        visited.push_back(currentVertex);  // Add the current vertex to the visited list

        // Traverse all adjacent vertices of the current vertex
        for (auto adjacentVertex : getAdjacentVertices(currentVertex)) {
            // If adjacent vertex hasn't been visited, mark it and add to queue
            if (!isVisited[adjacentVertex]) {
                q.push(adjacentVertex);
                isVisited[adjacentVertex] = true;
            }
        }
    }

    return visited;  // Return the visited vertices in BFS order
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Depth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visited;  // Stores the visited vertices in order
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;  // Map to track visited vertices

    // Start DFS traversal from the given vertex using the helper function
    DFSTraversalHelper(vertex, visited, isVisited);

    return visited;  // Return the visited vertices in DFS order
}

template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited, unordered_map<shared_ptr<Vertex<T>>, bool>& isVisited) {
    // Utility function for Depth First Search Traversal (recursive)
    // vertex: current vertex being visited
    // visited: list of visited vertices
    // isVisited: map to check if a vertex has been visited

    isVisited[vertex] = true;  // Mark the vertex as visited
    visited.push_back(vertex);  // Add vertex to the visited list

    // Visit all adjacent vertices that haven't been visited yet
    for (auto adjacentVertex : getAdjacentVertices(vertex)) {
        if (!isVisited[adjacentVertex]) {
            // Recur for each unvisited adjacent vertex
            DFSTraversalHelper(adjacentVertex, visited, isVisited);
        }
    }
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::dijkstraShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Dijkstra's Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    unordered_map<shared_ptr<Vertex<T>>, int> distances;  // Map to store the shortest distance from source
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;  // Map to store previous vertex for path reconstruction
    vector<shared_ptr<Vertex<T>>> visited;  // Stores visited vertices
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq;  // Min-heap for the priority queue

    // Initialize distances and priority queue
    for (auto vertex : vertices) {
        distances[vertex] = std::numeric_limits<int>::max();  // Set initial distances to infinity
    }
    distances[source] = 0;  // Distance from source to itself is zero
    pq.push({0, source});  // Push the source vertex with distance 0 into the priority queue

    while (!pq.empty()) {
        auto [currentDist, currentVertex] = pq.top();  // Get the vertex with the smallest distance
        pq.pop();  // Remove it from the priority queue

        if (find(visited.begin(), visited.end(), currentVertex) != visited.end()) {
            continue;  // If the vertex has already been visited, skip it
        }

        visited.push_back(currentVertex);  // Add vertex to visited list

        // Update distances for adjacent vertices
        for (auto adjacentVertex : getAdjacentVertices(currentVertex)) {
            int edgeWeight = getEdge(currentVertex->getData(), adjacentVertex->getData())->getWeight();  // Get the edge weight
            int newDist = currentDist + edgeWeight;  // Calculate new distance through the current vertex

            if (newDist < distances[adjacentVertex]) {
                // If the new distance is smaller, update it
                distances[adjacentVertex] = newDist;
                previous[adjacentVertex] = currentVertex;  // Set current vertex as the previous vertex for reconstruction
                pq.push({newDist, adjacentVertex});  // Push the updated vertex into the priority queue
            }
        }
    }

    // Reconstruct the shortest path based on the 'previous' map
    vector<shared_ptr<Vertex<T>>> shortestPath;
    for (auto vertex = source; vertex != nullptr; vertex = previous[vertex]) {
        shortestPath.push_back(vertex);  // Add each vertex to the path
    }

    reverse(shortestPath.begin(), shortestPath.end());  // Reverse to get the path from source to destination
    return shortestPath;  // Return the shortest path
}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// BONUS TASK FOR 5 MARKS
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    unordered_map<shared_ptr<Vertex<T>>, int> distances;  // Map to store shortest distances
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;  // Map to store previous vertex for path reconstruction
    vector<shared_ptr<Vertex<T>>> visited;  // Stores visited vertices

    // Initialize distances
    for (auto vertex : vertices) {
        distances[vertex] = std::numeric_limits<int>::max();  // Set all initial distances to infinity
    }
    distances[source] = 0;  // Distance from source to itself is zero

    // Relax edges |V| - 1 times
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        for (auto edge : edges) {
            auto u = edge->getSource();  // Get the source vertex of the edge
            auto v = edge->getDestination();  // Get the destination vertex of the edge
            int weight = edge->getWeight();  // Get the weight of the edge

            // If a shorter path is found, update the distance
            if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;  // Update the previous vertex for path reconstruction
            }
        }
    }

    // Check for negative weight cycles
    for (auto edge : edges) {
        auto u = edge->getSource();
        auto v = edge->getDestination();
        int weight = edge->getWeight();

        // If a shorter path is still found, it means a negative cycle exists
        if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
            cout << "Graph contains a negative weight cycle!" << endl;
            return {};  // Return an empty vector to indicate a negative cycle
        }
    }

    // Reconstruct the shortest path based on the 'previous' map
    vector<shared_ptr<Vertex<T>>> shortestPath;
    for (auto vertex = source; vertex != nullptr; vertex = previous[vertex]) {
        shortestPath.push_back(vertex);  // Add each vertex to the path
    }

    reverse(shortestPath.begin(), shortestPath.end());  // Reverse to get the path from source to destination
    return shortestPath;  // Return the shortest path
}
