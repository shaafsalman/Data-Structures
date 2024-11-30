#include "../src/Edge.h"

template <class T>
Edge<T>::Edge() {
    // Default constructor for the Edge class
    // Set the source, destination, weight and directed of the edge
    // source: source vertex of the edge
    // destination: destination vertex of the edge
    // weight: weight of the edge (0 for unweighted graphs)
    // directed: true if the edge is directed, false otherwise

    // Solution:
        
    this->source = nullptr;
    this->destination = nullptr;
    this->weight = 0; 
    this->directed = false;
}

template <class T>
Edge<T>::Edge(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination, int weight, bool directed) {
    // Constructor for the Edge class
    // Set the source, destination, weight and directed of the edge
    // source: source vertex of the edge
    // destination: destination vertex of the edge
    // weight: weight of the edge (0 for unweighted graphs)
    // directed: true if the edge is directed, false otherwise

    // Solution:
    this->source = source;
    this->destination = destination;
    this->weight = weight;
    this->directed = directed;
}

template <class T>
shared_ptr<Vertex<T>> Edge<T>::getSource() {
    // Return the source vertex of the edge

    // Solution:
    return source;
}

template <class T>
shared_ptr<Vertex<T>> Edge<T>::getDestination() {
    // Return the destination vertex of the edge

    // Solution:
}

template <class T>
int Edge<T>::getWeight() {
    // Return the weight of the edge 

    // Solution:
}

template <class T>
bool Edge<T>::isDirected() {
    // Return true if the edge is directed, false otherwise

    // Solution:
}

template <class T>
void Edge<T>::setWeight(int weight) {
    // Set the weight of the edge

    // Solution:
}