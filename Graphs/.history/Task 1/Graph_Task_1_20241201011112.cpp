#include "../src/Graph.h"
#include "Vertex.cpp"
#include <fstream>
#include <algorithm>

// Default constructor
template <class T>
Graph<T>::Graph() : directed(false), weighted(false) {}

// Parameterized constructor
template <class T>
Graph<T>::Graph(bool directed, bool weighted) : directed(directed), weighted(weighted) {}

// Add a vertex to the graph
template <class T>
void Graph<T>::addVertex(T data) {
    auto vertex = make_shared<Vertex<T>>(data);
    vertices.push_back(vertex);
}

// Add an edge to the graph
template <class T>
void Graph<T>::addEdge(T source, T destination, int weight) {
    auto srcVertex = getVertex(source);
    auto destVertex = getVertex(destination);

    if (!srcVertex) {
        srcVertex = make_shared<Vertex<T>>(source);
        vertices.push_back(srcVertex);
    }

    if (!destVertex) {
        destVertex = make_shared<Vertex<T>>(destination);
        vertices.push_back(destVertex);
    }

    auto edge = make_shared<Edge<T>>(srcVertex, destVertex, weight, directed);
    srcVertex->addEdge(edge);
    edges.push_back(edge);

    if (!directed) {
        auto reverseEdge = make_shared<Edge<T>>(destVertex, srcVertex, weight, directed);
        destVertex->addEdge(reverseEdge);
        edges.push_back(reverseEdge);
    }

    updateAdjacencyMatrix();
}

// Remove a vertex from the graph
template <class T>
void Graph<T>::removeVertex(T data) {
    auto it = find_if(vertices.begin(), vertices.end(), [&data](shared_ptr<Vertex<T>> vertex) {
        return vertex->getData() == data;
    });

    if (it != vertices.end()) {
        for (auto edge : (*it)->getEdges()) {
            removeEdge(edge->getSource()->getData(), edge->getDestination()->getData());
        }
        vertices.erase(it);
    }

    updateAdjacencyMatrix();
}

// Remove an edge from the graph
template <class T>
void Graph<T>::removeEdge(T source, T destination) {
    edges.erase(remove_if(edges.begin(), edges.end(), [&source, &destination](shared_ptr<Edge<T>> edge) {
        return edge->getSource()->getData() == source && edge->getDestination()->getData() == destination;
    }), edges.end());

    auto srcVertex = getVertex(source);
    if (srcVertex) {
        srcVertex->removeEdge(destination);
    }

    if (!directed) {
        auto destVertex = getVertex(destination);
        if (destVertex) {
            destVertex->removeEdge(source);
        }
    }

    updateAdjacencyMatrix();
}

// Get a vertex by data
template <class T>
shared_ptr<Vertex<T>> Graph<T>::getVertex(T data) {
    auto it = find_if(vertices.begin(), vertices.end(), [&data](shared_ptr<Vertex<T>> vertex) {
        return vertex->getData() == data;
    });
    return it != vertices.end() ? *it : nullptr;
}

// Get an edge by source and destination
template <class T>
shared_ptr<Edge<T>> Graph<T>::getEdge(T source, T destination) {
    auto it = find_if(edges.begin(), edges.end(), [&source, &destination](shared_ptr<Edge<T>> edge) {
        return edge->getSource()->getData() == source && edge->getDestination()->getData() == destination;
    });
    return it != edges.end() ? *it : nullptr;
}

// Get all vertices
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAllVertices() {
    return vertices;
}

// Get all edges
template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getAllEdges() {
    return edges;
}

// Get edges connected to a vertex
template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getEdges(shared_ptr<Vertex<T>> vertex) {
    return vertex ? vertex->getEdges() : vector<shared_ptr<Edge<T>>>();
}

// Get adjacent vertices of a vertex
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    vector<shared_ptr<Vertex<T>>> adjacent;
    if (vertex) {
        for (auto edge : vertex->getEdges()) {
            adjacent.push_back(edge->getDestination());
        }
    }
    return adjacent;
}

// Get incoming adjacent vertices of a vertex
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getInAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    vector<shared_ptr<Vertex<T>>> inAdjacent;
    for (auto edge : edges) {
        if (edge->getDestination() == vertex) {
            inAdjacent.push_back(edge->getSource());
        }
    }
    return inAdjacent;
}

// Get outgoing adjacent vertices of a vertex
template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getOutAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    return getAdjacentVertices(vertex);
}

// Check if graph is directed
template <class T>
bool Graph<T>::isDirected() {
    return directed;
}

// Check if graph is weighted
template <class T>
bool Graph<T>::isWeighted() {
    return weighted;
}

// Set graph direction
template <class T>
void Graph<T>::setDirected(bool directed) {
    this->directed = directed;
    updateAdjacencyMatrix();
}

// Set graph weight status
template <class T>
void Graph<T>::setWeighted(bool weighted) {
    this->weighted = weighted;
    updateAdjacencyMatrix();
}

// Update adjacency matrix
template <class T>
void Graph<T>::updateAdjacencyMatrix() {
    size_t size = vertices.size();
    adjacencyMatrix.assign(size, vector<int>(size, 0));

    for (auto edge : edges) {
        int srcIdx = distance(vertices.begin(), find(vertices.begin(), vertices.end(), edge->getSource()));
        int destIdx = distance(vertices.begin(), find(vertices.begin(), vertices.end(), edge->getDestination()));

        adjacencyMatrix[srcIdx][destIdx] = weighted ? edge->getWeight() : 1;
        if (!directed) {
            adjacencyMatrix[destIdx][srcIdx] = adjacencyMatrix[srcIdx][destIdx];
        }
    }
}

// Get adjacency matrix
template <class T>
vector<vector<int>> Graph<T>::getAdjacencyMatrix() {
    return adjacencyMatrix;
}

// Print adjacency matrix
template <class T>
void Graph<T>::printAdjacencyMatrix() {
    cout << "Adjacency Matrix:" << endl;
    for (const auto& row : adjacencyMatrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Print adjacency list
template <class T>
void Graph<T>::printAdjacencyList() {
    cout << "Adjacency List:" << endl;
    for (auto vertex : vertices) {
        cout << vertex->getData() << " -> ";
        for (auto edge : vertex->getEdges()) {
            cout << edge->getDestination()->getData() << " ";
        }
        cout << endl;
    }
}
