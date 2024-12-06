#include "../src/Graph.h"
#include "../Task 1/Graph_Task_1.cpp"
#include <queue>
#include <stack>
#include <unordered_map>
#include <climits>
#include <set>        
#include <map>  


// ::::::::::::::::::::::::::::::::::::::: TASK 2 :::::::::::::::::::::::::::::::::::::::::::

// Breadth-First Search Traversal
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::BFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Breadth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    vector<shared_ptr<Vertex<T>>> visitedVertices;
    if (!vertex) return visitedVertices;

    queue<shared_ptr<Vertex<T>>> toVisit;
    vector<shared_ptr<Vertex<T>>> visited;

    toVisit.push(vertex);
    visited.push_back(vertex);

    while (!toVisit.empty()) {
        shared_ptr<Vertex<T>> current = toVisit.front();
        toVisit.pop();
        visitedVertices.push_back(current);

        vector<shared_ptr<Vertex<T>>> adjacentVertices = getAdjacentVertices(current);
        for (size_t i = 0; i < adjacentVertices.size(); ++i) {
            bool alreadyVisited = false;
            for (size_t j = 0; j < visited.size(); ++j) {
                if (visited[j] == adjacentVertices[i]) {
                    alreadyVisited = true;
                    break;
                }
            }
            if (!alreadyVisited) {
                visited.push_back(adjacentVertices[i]);
                toVisit.push(adjacentVertices[i]);
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
    if (!vertex) return visitedVertices;

    stack<shared_ptr<Vertex<T>>> toVisit;
    vector<shared_ptr<Vertex<T>>> visited;

    toVisit.push(vertex);

    while (!toVisit.empty()) {
        shared_ptr<Vertex<T>> current = toVisit.top();
        toVisit.pop();

        bool alreadyVisited = false;
        for (size_t i = 0; i < visited.size(); ++i) {
            if (visited[i] == current) {
                alreadyVisited = true;
                break;
            }
        }

        if (!alreadyVisited) {
            visited.push_back(current);
            visitedVertices.push_back(current);

            vector<shared_ptr<Vertex<T>>> adjacentVertices = getAdjacentVertices(current);
            for (size_t i = 0; i < adjacentVertices.size(); ++i) {
                toVisit.push(adjacentVertices[i]);
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
    if (source == nullptr) return result;  // If source is null, return empty result

    int n = vertices.size();
    vector<int> distances(n, INT_MAX);  // Distance of all vertices is initially infinite
    vector<bool> visited(n, false);  // Track if the vertex has been visited
    distances[getVertexIndex(source)] = 0;  // Distance to source is 0

    // Process each vertex
    for (int count = 0; count < n; ++count) {
        int minDistance = INT_MAX;
        int currentIndex = -1;

        // Find the unvisited vertex with the smallest distance
        for (int i = 0; i < n; ++i) {
            if (!visited[i] && distances[i] < minDistance) {
                minDistance = distances[i];
                currentIndex = i;
            }
        }

        if (currentIndex == -1) break;  // If no more reachable vertices, exit the loop
        visited[currentIndex] = true;  // Mark current vertex as visited

        // Update the distances to adjacent vertices
        shared_ptr<Vertex<T>> currentVertex = vertices[currentIndex];
        vector<shared_ptr<Edge<T>>> edges = currentVertex->getEdges();
        for (auto& edge : edges) {
            shared_ptr<Vertex<T>> adjacent = edge->getDestination();
            int adjacentIndex = getVertexIndex(adjacent);
            int newDistance = distances[currentIndex] + edge->getWeight();
            if (!visited[adjacentIndex] && newDistance < distances[adjacentIndex]) {
                distances[adjacentIndex] = newDistance;  // Update distance if shorter
            }
        }
    }

    // Collect vertices with finite distances
    for (int i = 0; i < n; ++i) {
        if (distances[i] != INT_MAX) {
            result.push_back(vertices[i]);
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
