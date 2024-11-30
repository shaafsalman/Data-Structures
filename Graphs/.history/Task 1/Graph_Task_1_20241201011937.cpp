#include "../src/Graph.h"
#include "Vertex.cpp"
#include "Edge.cpp"
#include <fstream>
#include <algorithm>

template <class T>
Graph<T>::Graph() {
    // Default constructor for the Graph class
    this->directed = false;
    this->weighted = false;
}

template <class T>
Graph<T>::Graph(bool directed, bool weighted) {
    // Constructor for the Graph class
    this->directed = directed;
    this->weighted = weighted;
}

template <class T>
void Graph<T>::addVertex(T data) {
    // Add a new vertex to the graph
    if (getVertex(data) != nullptr) {
        cout << "Vertex already exists." << endl;
        return;
    }
    vertices.push_back(make_shared<Vertex<T>>(data));
    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::addEdge(T source, T destination, int weight) {
    // Add an edge between source and destination vertices
    auto srcVertex = getVertex(source);
    auto destVertex = getVertex(destination);

    if (!srcVertex || !destVertex) {
        cout << "Source or destination vertex not found." << endl;
        return;
    }

    if (!weighted && weight != 0) {
        cout << "Functionality not available for unweighted graphs." << endl;
        return;
    }

    auto newEdge = make_shared<Edge<T>>(srcVertex, destVertex, weight, directed);
    edges.push_back(newEdge);
    srcVertex->addEdge(newEdge);
    if (!directed) {
        destVertex->addEdge(make_shared<Edge<T>>(destVertex, srcVertex, weight, false));
    }
    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::removeVertex(T data) {
    // Remove a vertex and all associated edges
    auto vertex = getVertex(data);
    if (!vertex) {
        cout << "Vertex not found." << endl;
        return;
    }

    // Remove all edges connected to this vertex
    edges.erase(
        remove_if(edges.begin(), edges.end(), [vertex](shared_ptr<Edge<T>> edge) {
            return edge->getSource() == vertex || edge->getDestination() == vertex;
        }),
        edges.end()
    );

    // Remove the vertex itself
    vertices.erase(
        remove_if(vertices.begin(), vertices.end(), [vertex](shared_ptr<Vertex<T>> v) {
            return v == vertex;
        }),
        vertices.end()
    );

    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::removeEdge(T source, T destination) {
    // Remove an edge between source and destination vertices
    edges.erase(
        remove_if(edges.begin(), edges.end(), [source, destination](shared_ptr<Edge<T>> edge) {
            return edge->getSource()->getData() == source && edge->getDestination()->getData() == destination;
        }),
        edges.end()
    );

    updateAdjacencyMatrix();
}

template <class T>
shared_ptr<Vertex<T>> Graph<T>::getVertex(T data) {
    // Return the vertex with the given data
    for (auto vertex : vertices) {
        if (vertex->getData() == data) {
            return vertex;
        }
    }
    return nullptr;
}

template <class T>
shared_ptr<Edge<T>> Graph<T>::getEdge(T source, T destination) {
    // Return the edge between source and destination vertices
    for (auto edge : edges) {
        if (edge->getSource()->getData() == source && edge->getDestination()->getData() == destination) {
            return edge;
        }
    }
    return nullptr;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAllVertices() {
    return vertices;
}

template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getAllEdges() {
    return edges;
}

template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getEdges(shared_ptr<Vertex<T>> vertex) {
    return vertex->getEdges();
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    vector<shared_ptr<Vertex<T>>> adjacent;
    for (auto edge : vertex->getEdges()) {
        adjacent.push_back(edge->getDestination());
    }
    return adjacent;
}

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

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getOutAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    return getAdjacentVertices(vertex);
}

template <class T>
bool Graph<T>::isDirected() {
    return directed;
}

template <class T>
bool Graph<T>::isWeighted() {
    return weighted;
}

template <class T>
void Graph<T>::setDirected(bool directed) {
    this->directed = directed;
}

template <class T>
void Graph<T>::setWeighted(bool weighted) {
    this->weighted = weighted;
}

template <class T>
void Graph<T>::updateAdjacencyMatrix() {
    int size = vertices.size();
    adjacencyMatrix.assign(size, vector<int>(size, 0));
    adjacencyMatrixWeighted.assign(size, vector<int>(size, 0));

    for (auto edge : edges) {
        int srcIdx = find(vertices.begin(), vertices.end(), edge->getSource()) - vertices.begin();
        int destIdx = find(vertices.begin(), vertices.end(), edge->getDestination()) - vertices.begin();

        adjacencyMatrix[srcIdx][destIdx] = 1;
        adjacencyMatrixWeighted[srcIdx][destIdx] = edge->getWeight();

        if (!directed) {
            adjacencyMatrix[destIdx][srcIdx] = 1;
            adjacencyMatrixWeighted[destIdx][srcIdx] = edge->getWeight();
        }
    }
}

template <class T>
vector<vector<int>> Graph<T>::getAdjacencyMatrix() {
    return adjacencyMatrix;
}

template <class T>
void Graph<T>::printAdjacencyMatrix() {
    for (const auto &row : adjacencyMatrix) {
        for (const auto &value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

template <class T>
void Graph<T>::printAdjacencyList() {
    for (auto vertex : vertices) {
        cout << vertex->getData() << ": ";
        for (auto edge : vertex->getEdges()) {
            cout << edge->getDestination()->getData() << " ";
        }
        cout << endl;
    }
}
