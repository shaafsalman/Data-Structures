#include "../src/Graph.h"
#include "../Task 1/Graph_Task_1.cpp"
#include <queue>
#include <stack>
#include <unordered_map>  
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::BFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Breadth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visitedVertices;
    if (!vertex) return visitedVertices; // Handle null vertex

    queue<shared_ptr<Vertex<T>>> queue;
    queue.push(vertex);
    vertex->setVisited(true);
    visitedVertices.push_back(vertex);

    while (!queue.empty()) {
        shared_ptr<Vertex<T>> current = queue.front();
        queue.pop();

        for (auto& edge : current->getEdges()) {
            shared_ptr<Vertex<T>> adjacent = edge->getDestination();
            if (!adjacent->isVisited()) {
                adjacent->setVisited(true);
                visitedVertices.push_back(adjacent);
                queue.push(adjacent);
            }
        }
    }

    // Reset visited status after traversal
    for (auto& v : visitedVertices) {
        v->setVisited(false);
    }

    return visitedVertices;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Depth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visitedVertices;
    if (!vertex) return visitedVertices; // Handle null vertex

    stack<shared_ptr<Vertex<T>>> stack;
    stack.push(vertex);
    vertex->setVisited(true);
    visitedVertices.push_back(vertex);

    while (!stack.empty()) {
        shared_ptr<Vertex<T>> current = stack.top();
        stack.pop();

        bool unvisitedNeighbor = false;
        for (auto& edge : current->getEdges()) {
            shared_ptr<Vertex<T>> adjacent = edge->getDestination();
            if (!adjacent->isVisited()) {
                adjacent->setVisited(true);
                visitedVertices.push_back(adjacent);
                stack.push(adjacent);
                unvisitedNeighbor = true;
                break;
            }
        }

        // If no unvisited neighbor, pop current vertex
        if (!unvisitedNeighbor) {
            stack.pop();
        }
    }

    // Reset visited status after traversal
    for (auto& v : visitedVertices) {
        v->setVisited(false);
    }

    return visitedVertices;
}

template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited) {
    // Utility function for Depth First Search Traversal
    // vertex: current vertex
    // visited: vector to store the visited vertices

    if (vertex->isVisited()) return;

    vertex->setVisited(true);
    visited.push_back(vertex);

    for (auto& edge : vertex->getEdges()) {
        shared_ptr<Vertex<T>> adjacent = edge->getDestination();
        if (!adjacent->isVisited()) {
            DFSTraversalHelper(adjacent, visited);
        }
    }
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::dijkstraShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Dijkstra's Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    vector<shared_ptr<Vertex<T>>> result;
    if (!source) return result;

    // Initialize distance and predecessor maps
    unordered_map<shared_ptr<Vertex<T>>, int> distances;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> predecessors;
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>> ;

    // Initialize all distances as infinity except for the source vertex
    for (auto& vertex : vertices) {
        distances[vertex] = INT_MAX;
    }
    distances[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();

        if (dist > distances[current]) continue;

        for (auto& edge : current->getEdges()) {
            shared_ptr<Vertex<T>> adjacent = edge->getDestination();
            int newDist = dist + edge->getWeight();

            if (newDist < distances[adjacent]) {
                distances[adjacent] = newDist;
                predecessors[adjacent] = current;
                pq.push({newDist, adjacent});
            }
        }
    }

    // Reconstruct the shortest path from the source
    for (auto& vertex : vertices) {
        if (distances[vertex] != INT_MAX) {
            result.push_back(vertex);
        }
    }

    return result;
}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// BONUS TASK FOR 5 MARKS
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    vector<shared_ptr<Vertex<T>>> result;
    if (!source) return result;

    // Initialize distance map
    unordered_map<shared_ptr<Vertex<T>>, int> distances;
    for (auto& vertex : vertices) {
        distances[vertex] = INT_MAX;
    }
    distances[source] = 0;

    // Relax edges repeatedly
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        for (auto& edge : edges) {
            shared_ptr<Vertex<T>> u = edge->getSource();
            shared_ptr<Vertex<T>> v = edge->getDestination();
            int weight = edge->getWeight();

            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }

    // Check for negative weight cycles
    for (auto& edge : edges) {
        shared_ptr<Vertex<T>> u = edge->getSource();
        shared_ptr<Vertex<T>> v = edge->getDestination();
        int weight = edge->getWeight();

        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            // Negative weight cycle detected, but we don't handle it here
            // This can be extended to handle negative cycles.
            break;
        }
    }

    // Collect vertices in ascending order of distance
    for (auto& vertex : vertices) {
        if (distances[vertex] != INT_MAX) {
            result.push_back(vertex);
        }
    }

    return result;
}
