#include "../src/Vertex.h"
#include "Edge.cpp"

template <class T>
Vertex<T>::Vertex() {
    // Default constructor for the Vertex class
    
    // Solution:
    this->data = T();
    this->edges = vector<shared_ptr<Edge<T>>>();
    this->visited = false;

}

template <class T>
Vertex<T>::Vertex(T data) {
    // Constructor for the Vertex class
    // Set the data of the vertex
    // data: data of the vertex of the object type T

    // Solution:
    this->data = data;
    this->edges = vector<shared_ptr<Edge<T>>>();
    this->visited = false;
}

template <class T>
T Vertex<T>::getData() {
    // Return the data of the vertex
    
    // Solution:
    return data;
}

template <class T>
void Vertex<T>::addEdge(shared_ptr<Edge<T>> edge) {
    // Add an edge to the vertex
    // edge: edge to be added to the vertex

    // Solution:
    edges.push_back(edge);
}

template <class T>
void Vertex<T>::removeEdge(T destination) {
    // Remove the edge between the vertex and the destination vertex
    // destination: data of the destination vertex

    // Solution:
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->getDestination()->getData() == destination) {
            edges.erase(edges.begin() + i);
            break;
        }
    }
}

template <class T>
bool Vertex<T>::isVisited() {
    // Return true if the vertex is visited, false otherwise

    // Solution:
    return visited;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    // Set the visited status of the vertex
    // visited: true if the vertex is visited, false otherwise

    // Solution:
    this->visited = visited;
}

template <class T>
vector<shared_ptr<Edge<T>>> Vertex<T>::getEdges() {
    // Return the edges connected to the vertex

    // Solution:
    return edges;
}