#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// Edge class to store source, destination and weight of an edge

template <class T>
class Vertex;  // Forward declaration for Vertex class

template <class T>
class Edge {
    shared_ptr<Vertex<T>> source;
    shared_ptr<Vertex<T>> destination;
    int weight;
    bool directed;

public:
    Edge();
    Edge(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination, int weight, bool directed);
    shared_ptr<Vertex<T>> getSource();
    shared_ptr<Vertex<T>> getDestination();
    int getWeight();
    bool isDirected();
    void setWeight(int weight);
};

#endif