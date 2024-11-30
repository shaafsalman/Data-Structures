#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>
#include <memory>

#include "Edge.h"

// Vertex class to store the data of the vertex and the edges connected to it

template <class T>
class Vertex {
    T data;
    vector<shared_ptr<Edge<T>>> edges;
    bool visited = false;
    
public:
    Vertex();
    Vertex(T data);
    T getData();
    void addEdge(shared_ptr<Edge<T>> edge);
    void removeEdge(T destination);
    bool isVisited();
    void setVisited(bool visited);
    vector<shared_ptr<Edge<T>>> getEdges();
};

#endif