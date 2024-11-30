#include "../src/Graph.h"
#include "Vertex.cpp"
#include <fstream>
#include <unordered_map>
#include <memory>
#include <algorithm>

template <class T>
Graph<T>::Graph() : directed(false), weighted(false) {}

template <class T>
Graph<T>::Graph(bool directed, bool weighted) : directed(directed), weighted(weighted) {}

template <class T>
void Graph<T>::addVertex(T data) {
    // Check if the vertex already exists
    if (std::find_if(vertices.begin(), vertices.end(), 
        [&data](const std::pair<T, std::shared_ptr<Vertex<T>>>& pair) { return pair.first == data; }) == vertices.end()) {
        std::shared_ptr<Vertex<T>> vertex = std::make_shared<Vertex<T>>(data);
        vertices[data] = vertex;
    }
}

template <class T>
void Graph<T>::addEdge(T source, T destination, int weight) {
    // Add edge only if both vertices exist
    if (vertices.find(source) != vertices.end() && vertices.find(destination) != vertices.end()) {
        std::shared_ptr<Vertex<T>> sourceVertex = vertices[source];
        std::shared_ptr<Vertex<T>> destinationVertex = vertices[destination];

        std::shared_ptr<Edge<T>> edge = std::make_shared<Edge<T>>(sourceVertex, destinationVertex, weight, directed);
        sourceVertex->addEdge(edge);
        destinationVertex->addEdge(edge);
        edges.push_back(edge);

        if (!directed) {
            // For undirected graphs, also add reverse edge
            std::shared_ptr<Edge<T>> reverseEdge = std::make_shared<Edge<T>>(destinationVertex, sourceVertex, weight, directed);
            destinationVertex->addEdge(reverseEdge);
            sourceVertex->addEdge(reverseEdge);
            edges.push_back(reverseEdge);
        }
    }
}

template <class T>
void Graph<T>::removeVertex(T data) {
    // Find the vertex
    if (vertices.find(data) != vertices.end()) {
        std::shared_ptr<Vertex<T>> vertex = vertices[data];

        // Remove all edges associated with this vertex
        auto vertexEdges = vertex->getEdges();
        for (auto& edge : vertexEdges) {
            std::shared_ptr<Vertex<T>> otherVertex = edge->getSource() == vertex ? edge->getDestination() : edge->getSource();
            otherVertex->removeEdge(data);
        }

        vertices.erase(data);
    }
}

template <class T>
void Graph<T>::removeEdge(T source, T destination) {
    if (vertices.find(source) != vertices.end() && vertices.find(destination) != vertices.end()) {
        std::shared_ptr<Vertex<T>> sourceVertex = vertices[source];
        std::shared_ptr<Vertex<T>> destinationVertex = vertices[destination];

        // Remove edge from the source and destination vertices
        sourceVertex->removeEdge(destination);
        destinationVertex->removeEdge(source);
    }
}

template <class T>
std::shared_ptr<Vertex<T>> Graph<T>::getVertex(T data) {
    if (vertices.find(data) != vertices.end()) {
        return vertices[data];
    }
    return nullptr;
}

template <class T>
std::shared_ptr<Edge<T>> Graph<T>::getEdge(T source, T destination) {
    for (auto& edge : edges) {
        if ((edge->getSource()->getData() == source && edge->getDestination()->getData() == destination) ||
            (edge->getSource()->getData() == destination && edge->getDestination()->getData() == source)) {
            return edge;
        }
    }
    return nullptr;
}

template <class T>
std::vector<std::shared_ptr<Vertex<T>>> Graph<T>::getAllVertices() {
    std::vector<std::shared_ptr<Vertex<T>>> allVertices;
    for (auto& pair : vertices) {
        allVertices.push_back(pair.second);
    }
    return allVertices;
}

template <class T>
std::vector<std::shared_ptr<Edge<T>>> Graph<T>::getAllEdges() {
    return edges;
}

template <class T>
std::vector<std::shared_ptr<Edge<T>>> Graph<T>::getEdges(std::shared_ptr<Vertex<T>> vertex) {
    return vertex->getEdges();
}

template <class T>
std::vector<std::shared_ptr<Vertex<T>>> Graph<T>::getAdjacentVertices(std::shared_ptr<Vertex<T>> vertex) {
    std::vector<std::shared_ptr<Vertex<T>>> adjacentVertices;
    for (auto& edge : vertex->getEdges()) {
        std::shared_ptr<Vertex<T>> otherVertex = (edge->getSource() == vertex) ? edge->getDestination() : edge->getSource();
        adjacentVertices.push_back(otherVertex);
    }
    return adjacentVertices;
}

template <class T>
std::vector<std::shared_ptr<Vertex<T>>> Graph<T>::getInAdjacentVertices(std::shared_ptr<Vertex<T>> vertex) {
    std::vector<std::shared_ptr<Vertex<T>>> inAdjacentVertices;
    for (auto& edge : vertex->getEdges()) {
        if (edge->getDestination() == vertex) {
            inAdjacentVertices.push_back(edge->getSource());
        }
    }
    return inAdjacentVertices;
}

template <class T>
std::vector<std::shared_ptr<Vertex<T>>> Graph<T>::getOutAdjacentVertices(std::shared_ptr<Vertex<T>> vertex) {
    std::vector<std::shared_ptr<Vertex<T>>> outAdjacentVertices;
    for (auto& edge : vertex->getEdges()) {
        if (edge->getSource() == vertex) {
            outAdjacentVertices.push_back(edge->getDestination());
        }
    }
    return outAdjacentVertices;
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
    adjacencyMatrix.clear();
    int size = vertices.size();
    adjacencyMatrix.resize(size, std::vector<int>(size, 0));

    std::unordered_map<T, int> vertexIndex;
    int index = 0;
    for (auto& pair : vertices) {
        vertexIndex[pair.first] = index++;
    }

    for (auto& edge : edges) {
        int sourceIndex = vertexIndex[edge->getSource()->getData()];
        int destinationIndex = vertexIndex[edge->getDestination()->getData()];
        int weight = weighted ? edge->getWeight() : 1;

        adjacencyMatrix[sourceIndex][destinationIndex] = weight;
        if (!directed) {
            adjacencyMatrix[destinationIndex][sourceIndex] = weight;
        }
    }
}

template <class T>
std::vector<std::vector<int>> Graph<T>::getAdjacencyMatrix() {
    return adjacencyMatrix;
}

template <class T>
void Graph<T>::printAdjacencyMatrix() {
    updateAdjacencyMatrix();
    for (auto& row : adjacencyMatrix) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

template <class T>
void Graph<T>::printAdjacencyList() {
    for (auto& pair : vertices) {
        std::cout << pair.first << ": ";
        auto vertexEdges = pair.second->getEdges();
        for (auto& edge : vertexEdges) {
            std::cout << edge->getDestination()->getData() << " ";
        }
        std::cout << std::endl;
    }
}

// Provided function for visualizing the graph
template <class T>
void Graph<T>::printGraph(std::string fileName) {
    std::ofstream dotFile(fileName);
    dotFile << (directed ? "digraph" : "graph") << " G {" << std::endl;
    dotFile << "    graph [pad=\"0.5\", nodesep=\"0.8\", ranksep=\"1.2\", margin=0.5];" << std::endl;
    dotFile << "    node [shape=ellipse, style=filled, color=lightblue, fontname=\"Helvetica\", fontsize=16, width=0.5, height=0.5];" << std::endl;
    dotFile << "    edge [color=dimgray, penwidth=2.0, arrowhead=open];" << std::endl;
    dotFile << "    rankdir=LR;" << std::endl;
    dotFile << "    label=\"Graph\";" << std::endl;
    dotFile << "    labelloc=top;" << std::endl;

    for (auto edge : edges) {
        dotFile << "    " << edge->getSource()->getData() << (directed ? " -> " : " -- ") << edge->getDestination()->getData();
        if (weighted) {
            dotFile << " [label=\"" << edge->getWeight() << "\"]";
        }
        dotFile << ";" << std::endl;
    }
    dotFile << "}" << std::endl;
    dotFile.close();
    std::cout << "Graph exported to graph.dot (Graphviz format)" << std::endl;
}
