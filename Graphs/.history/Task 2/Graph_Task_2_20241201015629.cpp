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

    vector<shared_ptr<Vertex<T>>> visited;
    queue<shared_ptr<Vertex<T>>> q;
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;

    q.push(vertex);
    isVisited[vertex] = true;

    while (!q.empty()) {
        shared_ptr<Vertex<T>> currentVertex = q.front();
        q.pop();
        visited.push_back(currentVertex);

        // Traverse all adjacent vertices
        vector<shared_ptr<Vertex<T>>> adjVertices = getAdjacentVertices(currentVertex);
        for (auto adj : adjVertices) {
            if (!isVisited[adj]) {
                q.push(adj);
                isVisited[adj] = true;
            }
        }
    }

    return visited;
}

// template <class T>
// vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
//     // Perform Depth First Search Traversal starting from the given vertex
//     // Return the vertices in the order they were visited
//     // vertex: starting vertex for the traversal

//     vector<shared_ptr<Vertex<T>>> visited;
//     unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;
//     DFSTraversalHelper(vertex, visited, isVisited);
//     return visited;
// }

template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited, unordered_map<shared_ptr<Vertex<T>>, bool>& isVisited) {
    // Utility function for Depth First Search Traversal
    // vertex: current vertex
    // visited: vector to store the visited vertices
    // isVisited: map to track the visited status of each vertex

    visited.push_back(vertex);
    isVisited[vertex] = true;

    // Traverse all adjacent vertices
    vector<shared_ptr<Vertex<T>>> adjVertices = getAdjacentVertices(vertex);
    for (auto adj : adjVertices) {
        if (!isVisited[adj]) {
            DFSTraversalHelper(adj, visited, isVisited);
        }
    }
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::dijkstraShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Dijkstra's Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    // Initialize distances
    unordered_map<shared_ptr<Vertex<T>>, int> distances;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;
    auto cmp = [&distances](shared_ptr<Vertex<T>> a, shared_ptr<Vertex<T>> b) { return distances[a] > distances[b]; };
    priority_queue<shared_ptr<Vertex<T>>, vector<shared_ptr<Vertex<T>>>, decltype(cmp)> pq(cmp);

    for (auto v : getAllVertices()) {
        distances[v] = INT_MAX;
    }
    distances[source] = 0;
    pq.push(source);

    while (!pq.empty()) {
        shared_ptr<Vertex<T>> current = pq.top();
        pq.pop();

        // Update distances to adjacent vertices
        vector<shared_ptr<Vertex<T>>> adjVertices = getAdjacentVertices(current);
        for (auto adj : adjVertices) {
            int weight = getEdge(current->getData(), adj->getData())->getWeight();
            int alt = distances[current] + weight;
            if (alt < distances[adj]) {
                distances[adj] = alt;
                previous[adj] = current;
                pq.push(adj);
            }
        }
    }

    // Reconstruct the shortest path in ascending order
    vector<shared_ptr<Vertex<T>>> path;
    for (auto v = source; v != nullptr; v = previous[v]) {
        path.insert(path.begin(), v);
    }

    return path;
}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// BONUS TASK FOR 5 MARKS
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    unordered_map<shared_ptr<Vertex<T>>, int> distances;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;

    for (auto v : getAllVertices()) {
        distances[v] = INT_MAX;
    }
    distances[source] = 0;

    // Relax edges |V| - 1 times
    for (int i = 1; i < getAllVertices().size(); ++i) {
        for (auto edge : getAllEdges()) {
            shared_ptr<Vertex<T>> u = edge->getSource();
            shared_ptr<Vertex<T>> v = edge->getDestination();
            int weight = edge->getWeight();
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
            }
        }
    }

    // Check for negative-weight cycles
    for (auto edge : getAllEdges()) {
        shared_ptr<Vertex<T>> u = edge->getSource();
        shared_ptr<Vertex<T>> v = edge->getDestination();
        int weight = edge->getWeight();
        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            throw std::runtime_error("Graph contains negative weight cycle");
        }
    }

    // Reconstruct the shortest path in ascending order
    vector<shared_ptr<Vertex<T>>> path;
    for (auto v = source; v != nullptr; v = previous[v]) {
        path.insert(path.begin(), v);
    }

    return path;
}
