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
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversal(shared_ptr<Vertex<T>> vertex) {
    // Perform Depth First Search Traversal starting from the given vertex
    // Return the vertices in the order they were visited
    // vertex: starting vertex for the traversal

    // Solution:
}

template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited) {
    // Utility function for Depth First Search Traversal
    // vertex: current vertex
    // visited: vector to store the visited vertices

    // Solution:
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