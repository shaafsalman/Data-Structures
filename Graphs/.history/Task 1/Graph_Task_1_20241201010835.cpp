#include "../src/Graph.h"
#include "Vertex.cpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <class T>
Graph<T>::Graph() {
    // Default constructor for the Graph class
    directed = false;
    weighted = false;
}

template <class T>
Graph<T>::Graph(bool directed, bool weighted) {
    // Constructor for the Graph class
    // directed: true if the graph is directed, false otherwise
    // weighted: true if the graph is weighted, false otherwise

    this->directed = directed;
    this->weighted = weighted;
}

template <class T> 
void Graph<T>::addVertex(T data) {
    // Create a new vertex with the given data and add it to the graph
    // data: data of the vertex to be added

    auto vertex = make_shared<Vertex<T>>(data);
    vertices.push_back(vertex);
    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::addEdge(T source, T destination, int weight) {
    // Add an edge between the source and destination vertices with the given weight
    // source: data of the source vertex
    // destination: data of the destination vertex
    // weight: weight of the edge (0 for unweighted graphs)

    auto srcVertex = getVertex(source);
    auto destVertex = getVertex(destination);

    if (!srcVertex || !destVertex) return;

    auto edge = make_shared<Edge<T>>(srcVertex, destVertex, weight);
    edges.push_back(edge);

    if (!directed) {
        auto reverseEdge = make_shared<Edge<T>>(destVertex, srcVertex, weight);
        edges.push_back(reverseEdge);
    }

    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::removeVertex(T data) {
    // Remove the vertex with the given data from the graph
    // data: data of the vertex to be removed

    auto vertex = getVertex(data);
    if (!vertex) return;

    edges.erase(remove_if(edges.begin(), edges.end(), [vertex](shared_ptr<Edge<T>> edge) {
        return edge->getSource() == vertex || edge->getDestination() == vertex;
    }), edges.end());

    vertices.erase(remove_if(vertices.begin(), vertices.end(), [vertex](shared_ptr<Vertex<T>> v) {
        return v == vertex;
    }), vertices.end());

    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::removeEdge(T source, T destination) {
    // Remove the edge between the source and destination vertices
    // source: data of the source vertex
    // destination: data of the destination vertex

    edges.erase(remove_if(edges.begin(), edges.end(), [source, destination, this](shared_ptr<Edge<T>> edge) {
        return edge->getSource()->getData() == source && edge->getDestination()->getData() == destination;
    }), edges.end());

    updateAdjacencyMatrix();
}

template <class T>
shared_ptr<Vertex<T>> Graph<T>::getVertex(T data) {
    // Return the vertex with the given data
    // data: data of the vertex to be returned

    for (auto vertex : vertices) {
        if (vertex->getData() == data) {
            return vertex;
        }
    }
    return nullptr;
}

template <class T>
shared_ptr<Edge<T>> Graph<T>::getEdge(T source, T destination) {
    // Return the edge between the source and destination vertices
    // source: data of the source vertex
    // destination: data of the destination vertex

    for (auto edge : edges) {
        if (edge->getSource()->getData() == source && edge->getDestination()->getData() == destination) {
            return edge;
        }
    }
    return nullptr;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAllVertices() {
    // Return all the vertices in the graph
    return vertices;
}

template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getAllEdges() {
    // Return all the edges in the graph
    return edges;
}

template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getEdges(shared_ptr<Vertex<T>> vertex) {
    // Return all the edges connected to the given vertex
    // vertex: vertex whose edges are to be returned

    vector<shared_ptr<Edge<T>>> vertexEdges;
    for (auto edge : edges) {
        if (edge->getSource() == vertex || edge->getDestination() == vertex) {
            vertexEdges.push_back(edge);
        }
    }
    return vertexEdges;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    // Return all the adjacent vertices of the given vertex
    // vertex: vertex whose adjacent vertices are to be returned

    vector<shared_ptr<Vertex<T>>> adjacentVertices;
    for (auto edge : edges) {
        if (edge->getSource() == vertex) {
            adjacentVertices.push_back(edge->getDestination());
        }
    }
    return adjacentVertices;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getInAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    // Return all the incoming adjacent vertices of the given vertex
    // vertex: vertex whose incoming adjacent vertices are to be returned

    vector<shared_ptr<Vertex<T>>> inAdjacentVertices;
    for (auto edge : edges) {
        if (edge->getDestination() == vertex) {
            inAdjacentVertices.push_back(edge->getSource());
        }
    }
    return inAdjacentVertices;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getOutAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    // Return all the outgoing adjacent vertices of the given vertex
    // vertex: vertex whose outgoing adjacent vertices are to be returned

    vector<shared_ptr<Vertex<T>>> outAdjacentVertices;
    for (auto edge : edges) {
        if (edge->getSource() == vertex) {
            outAdjacentVertices.push_back(edge->getDestination());
        }
    }
    return outAdjacentVertices;
}

template <class T>
bool Graph<T>::isDirected() {
    // Return true if the graph is directed, false otherwise
    return directed;
}

template <class T>
bool Graph<T>::isWeighted() {
    // Return true if the graph is weighted, false otherwise
    return weighted;
}

template <class T>
void Graph<T>::setDirected(bool directed) {
    // set the direction status
    this->directed = directed;
    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::setWeighted(bool weighted) {
    // set the weighted status
    this->weighted = weighted;
    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::updateAdjacencyMatrix() {
    // Update the adjacency matrix of the graph
    // 1 if edge exists, 0 otherwise for undirected graphs
    // weight of the edge for weighted graphs, 0 otherwise

    adjacencyMatrix.assign(vertices.size(), vector<int>(vertices.size(), 0));
    adjacencyMatrixWeighted.assign(vertices.size(), vector<int>(vertices.size(), 0));

    for (auto edge : edges) {
        int srcIdx = distance(vertices.begin(), find(vertices.begin(), vertices.end(), edge->getSource()));
        int destIdx = distance(vertices.begin(), find(vertices.begin(), vertices.end(), edge->getDestination()));

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
    // Return the adjacency matrix of the graph
    return adjacencyMatrixWeighted;
}

template <class T>
void Graph<T>::printAdjacencyMatrix() {
    // Print the adjacency matrix of the graph with row and column headers shown

    cout << "Adjacency Matrix:" << endl;
    for (size_t i = 0; i < adjacencyMatrixWeighted.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrixWeighted[i].size(); ++j) {
            cout << adjacencyMatrixWeighted[i][j] << " ";
        }
        cout << endl;
    }
}

template <class T>
void Graph<T>::printAdjacencyList() {
    // Print the adjacency list of the graph

    for (auto vertex : vertices) {
        cout << vertex->getData() << ": ";
        auto adjacentVertices = getAdjacentVertices(vertex);
        for (auto adjVertex : adjacentVertices) {
            cout << adjVertex->getData() << " ";
        }
        cout << endl;
    }
}
