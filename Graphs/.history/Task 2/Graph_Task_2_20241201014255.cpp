#include "../src/Graph.h"
#include "../Task 1/Graph_Task_1.cpp"
#include <queue>
#include <stack>

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::BFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Breadth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    // Solution:
        vector<shared_ptr<Vertex<T>>> visited;
    queue<shared_ptr<Vertex<T>>> q;
    unordered_map<shared_ptr<Vertex<T>>, bool> isVisited;

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
}

// ::::::::::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::::::::::

// BONUS TASK FOR 5 MARKS
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::bellmanFordShortestPath(shared_ptr<Vertex<T>> source) {
    // Perform Bellman-Ford Algorithm starting from the given source vertex
    // Return the vertices in the ascending order of their distance from the source vertex
    // source: source vertex for the algorithm

    // Solution:
}