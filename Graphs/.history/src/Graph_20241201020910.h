#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <memory> 

#include "Vertex.h"
#include "Edge.h"

using namespace std;

// Graph class to store the vertices and edges of the graph

template <class T>
class Graph {
    vector<shared_ptr<Vertex<T>>> vertices; // vector to store all the vertices of the graph
    vector<shared_ptr<Edge<T>>> edges; // vector to store all the edges of the graph
    vector<vector<int>> adjacencyMatrix; // 1 if edge exists, 0 otherwise for undirected graphs
    vector<vector<int>> adjacencyMatrixWeighted; // weight of the edge for weighted graphs, 0 otherwise
    bool directed; // true if the graph is directed, false otherwise
    bool weighted; // true if the graph is weighted, false otherwise

public:
    // Some functions are only for directed or undirected graphs you are supposed to cater to that in your implementation 
    // You can just print "Functionality not available for directed/undirected graphs" and return a default value if a function is not available for a directed/undirected graph

    // ::::::::::::::::::::::::::::::::::::: TASK 1 :::::::::::::::::::::::::::::::::::::::::::
    Graph(); 
    Graph(bool directed, bool weighted);
    void addVertex(T data);
    void addEdge(T source, T destination, int weight=0);
    void removeVertex(T data);
    void removeEdge(T source, T destination);
    shared_ptr<Vertex<T>> getVertex(T data);
    shared_ptr<Edge<T>> getEdge(T source, T destination);
    vector<shared_ptr<Vertex<T>>> getAllVertices();
    vector<shared_ptr<Edge<T>>> getAllEdges();
    vector<shared_ptr<Edge<T>>> getEdges(shared_ptr<Vertex<T>> vertex);
    vector<shared_ptr<Vertex<T>>> getAdjacentVertices(shared_ptr<Vertex<T>> vertex);
    vector<shared_ptr<Vertex<T>>> getInAdjacentVertices(shared_ptr<Vertex<T>> vertex);
    vector<shared_ptr<Vertex<T>>> getOutAdjacentVertices(shared_ptr<Vertex<T>> vertex);
    bool isDirected();
    bool isWeighted();
    void setDirected(bool directed);
    void setWeighted(bool weighted);
    void updateAdjacencyMatrix();
    vector<vector<int>> getAdjacencyMatrix();
    void printAdjacencyMatrix();
    void printAdjacencyList();
    void printGraph(string fileName);

    // Task 1 Helper functions

    // ::::::::::::::::::::::::::::::::::::: TASK 2 :::::::::::::::::::::::::::::::::::::::::::
    // breadth-First Search Traversal
    vector<shared_ptr<Vertex<T>>> BFSTraversal(shared_ptr<Vertex<T>> vertex);
    // Depth-First Search Traversal
    vector<shared_ptr<Vertex<T>>> DFSTraversal(shared_ptr<Vertex<T>> vertex);
    void DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, vector<shared_ptr<Vertex<T>>>& visited);
    // Dijkstra's Algorithm - returns vertices in the ascending order of their distance from the source vertex
    vector<shared_ptr<Vertex<T>>> dijkstraShortestPath(shared_ptr<Vertex<T>> source);

    // ::::::::::::::::::::::::::::::: TASK 2 BONUS TASKS :::::::::::::::::::::::::::::::::::
    // Bellman-Ford Algorithm - returns vertices in the ascending order of their distance from the source vertex
    vector<shared_ptr<Vertex<T>>> bellmanFordShortestPath(shared_ptr<Vertex<T>> source);

    // Task 2 Helper functions


    // ::::::::::::::::::::::::::::::::::::: TASK 3 :::::::::::::::::::::::::::::::::::::::::::
    // Shortest Path between two vertices
    vector<shared_ptr<Vertex<T>>> shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination);
    // Minimum Spanning Tree - get the minimum spanning tree of the graph
    shared_ptr<Graph<T>> minimumSpanningTree();
    // Topological Sort - get the topological sort of the graph
    vector<shared_ptr<Vertex<T>>> topologicalSort();
    // Strongly Connected Components - get all the strongly connected components of the graph
    vector<vector<shared_ptr<Vertex<T>>>> stronglyConnectedComponents();

    // ::::::::::::::::::::::::::::::: TASK 3 BONUS TASKS :::::::::::::::::::::::::::::::::::
    // Spanning Trees - get all the spanning trees of the graph
    vector<shared_ptr<Graph<T>>> SpanningTrees();
    // Connected Components - get all the connected components of the graph
    vector<vector<shared_ptr<Vertex<T>>>> connectedComponents();

    // Task 3 Helper functions
    // ::::::::::::::::::::::::::::::: TASK 3 HELPER FUNCTIONS :::::::::::::::::::::::::::::::::::

// Function to find the minimum spanning tree (MST) of the graph
template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree();

// Function to perform topological sort on a directed acyclic graph (DAG)
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort();

// Function to find all strongly connected components (SCC) in a directed graph
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents();

// Function to find all the spanning trees of the graph
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees();

// Function to find all connected components in an undirected graph
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents();


};

#endif