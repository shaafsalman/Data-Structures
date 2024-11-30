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

    // Solution:
      vector<shared_ptr<Vertex<T>>> visited;
    queue<shared_ptr<Vertex<T>>> q;
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;  // Fix: isVisited correctly declared

    // Start BFS from the given vertex
    q.push(startVertex);
    isVisited[startVertex] = true;

    while (!q.empty()) {
        auto vertex = q.front();
        q.pop();
        visited.push_back(vertex);

        // Traverse all adjacent vertices of the current vertex
        for (auto adjacentVertex : getAdjacentVertices(vertex)) {
            if (!isVisited[adjacentVertex]) {
                q.push(adjacentVertex);
                isVisited[adjacentVertex] = true;
            }
        }
    }

    return visited;

}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Depth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    // Solution:
    vector<shared_ptr<Vertex<T>>> visited;
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;
    
    DFSTraversalHelper(startVertex, visited, isVisited);

    return visited;
}

template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited) {
    // Utility function for Depth First Search Traversal
    // vertex: current vertex
    // visited: vector to store the visited vertices

    // Solution:
     isVisited[vertex] = true;
    visited.push_back(vertex);

    // Visit all adjacent vertices that haven't been visited yet
    for (auto adjacentVertex : getAdjacentVertices(vertex)) {
        if (!isVisited[adjacentVertex]) {
            DFSTraversalHelper(adjacentVertex, visited, isVisited);
        }
    }
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::dijkstraShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Dijkstra's Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    // Solution:
     unordered_map<shared_ptr<Vertex<T>>, int> distances;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;
    vector<shared_ptr<Vertex<T>>> visited;
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq;

    // Initialize distances and priority queue
    for (auto vertex : vertices) {
        distances[vertex] = std::numeric_limits<int>::max();
    }
    distances[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDist, currentVertex] = pq.top();
        pq.pop();

        if (find(visited.begin(), visited.end(), currentVertex) != visited.end()) {
            continue;
        }

        visited.push_back(currentVertex);

        // Update distances for adjacent vertices
        for (auto adjacentVertex : getAdjacentVertices(currentVertex)) {
            int edgeWeight = getEdge(currentVertex->getData(), adjacentVertex->getData())->getWeight();
            int newDist = currentDist + edgeWeight;

            if (newDist < distances[adjacentVertex]) {
                distances[adjacentVertex] = newDist;
                previous[adjacentVertex] = currentVertex;
                pq.push({newDist, adjacentVertex});
            }
        }
    }

    // Reconstruct the shortest path based on the 'previous' map
    vector<shared_ptr<Vertex<T>>> shortestPath;
    for (auto vertex = source; vertex != nullptr; vertex = previous[vertex]) {
        shortestPath.push_back(vertex);
    }

    reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;

}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// BONUS TASK FOR 5 MARKS
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    // Solution:
     unordered_map<shared_ptr<Vertex<T>>, int> distances;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;
    vector<shared_ptr<Vertex<T>>> visited;

    // Initialize distances
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
                previous[v] = u;
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

    // Reconstruct the shortest path based on the 'previous' map
    vector<shared_ptr<Vertex<T>>> shortestPath;
    for (auto vertex = source; vertex != nullptr; vertex = previous[vertex]) {
        shortestPath.push_back(vertex);
    }

    reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;

}