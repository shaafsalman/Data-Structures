#include "../src/Graph.h"
#include "../Task 1/Graph_Task_1.cpp"
#include <queue>
#include <stack>


// ::::::::::::::::::::::::::::::::::::::: TASK 2 :::::::::::::::::::::::::::::::::::::::::::

// Breadth-First Search Traversal
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::BFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Breadth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visitedVertices;
    if (vertex == nullptr) return visitedVertices;

    queue<shared_ptr<Vertex<T>>> toVisit;
    set<shared_ptr<Vertex<T>>> visited;

    toVisit.push(vertex);
    visited.insert(vertex);

    while (!toVisit.empty()) {
        shared_ptr<Vertex<T>> current = toVisit.front();
        toVisit.pop();
        visitedVertices.push_back(current);

        vector<shared_ptr<Vertex<T>>> adjacentVertices = getAdjacentVertices(current);
        for (auto& adj : adjacentVertices) {
            if (visited.find(adj) == visited.end()) {
                visited.insert(adj);
                toVisit.push(adj);
            }
        }
    }

    return visitedVertices;
}

// Depth-First Search Traversal
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Depth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visitedVertices;
    if (vertex == nullptr) return visitedVertices;

    stack<shared_ptr<Vertex<T>>> toVisit;
    set<shared_ptr<Vertex<T>>> visited;

    toVisit.push(vertex);

    while (!toVisit.empty()) {
        shared_ptr<Vertex<T>> current = toVisit.top();
        toVisit.pop();
        
        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            visitedVertices.push_back(current);
            
            vector<shared_ptr<Vertex<T>>> adjacentVertices = getAdjacentVertices(current);
            for (auto& adj : adjacentVertices) {
                if (visited.find(adj) == visited.end()) {
                    toVisit.push(adj);
                }
            }
        }
    }

    return visitedVertices;
}

// Depth-First Search Traversal Helper
template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited) {
    // Utility function for Depth First Search Traversal
    // vertex: current vertex
    // visited: vector to store the visited vertices

    if (vertex == nullptr) return;

    visited.push_back(vertex);
    vertex->setVisited(true);

    vector<shared_ptr<Edge<T>>> edges = vertex->getEdges();
    for (auto& edge : edges) {
        shared_ptr<Vertex<T>> adjacent = edge->getDestination();
        if (!adjacent->isVisited()) {
            DFSTraversalHelper(adjacent, visited);
        }
    }
}

// Dijkstra's Shortest Path Algorithm
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::dijkstraShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Dijkstra's Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    vector<shared_ptr<Vertex<T>>> result;
    if (source == nullptr) return result;

    map<shared_ptr<Vertex<T>>, int> distances;
    for (auto& vertex : vertices) {
        distances[vertex] = INT_MAX;
    }
    distances[source] = 0;

    set<shared_ptr<Vertex<T>>> visited;
    set<shared_ptr<Vertex<T>>> toVisit;
    toVisit.insert(source);

    while (!toVisit.empty()) {
        shared_ptr<Vertex<T>> current = *toVisit.begin();
        toVisit.erase(toVisit.begin());
        visited.insert(current);

        vector<shared_ptr<Edge<T>>> edges = current->getEdges();
        for (auto& edge : edges) {
            shared_ptr<Vertex<T>> adjacent = edge->getDestination();
            if (visited.find(adjacent) == visited.end()) {
                int newDist = distances[current] + edge->getWeight();
                if (newDist < distances[adjacent]) {
                    distances[adjacent] = newDist;
                    toVisit.insert(adjacent);
                }
            }
        }
    }

    for (auto& vertex : vertices) {
        if (distances[vertex] != INT_MAX) {
            result.push_back(vertex);
        }
    }

    return result;
}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// Bellman-Ford Algorithm
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    vector<shared_ptr<Vertex<T>>> result;
    if (source == nullptr) return result;

    map<shared_ptr<Vertex<T>>, int> distances;
    for (auto& vertex : vertices) {
        distances[vertex] = INT_MAX;
    }
    distances[source] = 0;

    int numVertices = vertices.size();
    for (int i = 1; i < numVertices; ++i) {
        for (auto& edge : edges) {
            shared_ptr<Vertex<T>> u = edge->getSource();
            shared_ptr<Vertex<T>> v = edge->getDestination();
            int weight = edge->getWeight();

            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }

    for (auto& vertex : vertices) {
        if (distances[vertex] != INT_MAX) {
            result.push_back(vertex);
        }
    }

    return result;
}
