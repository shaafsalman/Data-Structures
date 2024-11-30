#include "../src/Graph.h"
#include "Vertex.cpp"
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
    // directed: true if the graph is directed, false otherwise
    // weighted: true if the graph is weighted, false otherwise

    // There are 4 cases for the graph:
    // 1. Directed and Weighted
    // 2. Directed and Unweighted
    // 3. Undirected and Weighted
    // 4. Undirected and Unweighted

    // Solution:
        this->directed = directed;
    this->weighted = weighted;
}

template <class T> 
void Graph<T>::addVertex(T data) {
    // Create a new vertex with the given data and add it to the graph
    // data: data of the vertex to be added

    // Solution:
       if (getVertex(data) != nullptr) {
        cout << "Vertex already exists." << endl;
        return;
    }
    vertices.push_back(make_shared<Vertex<T>>(data));
    updateAdjacencyMatrix();
}

template <class T>
void Graph<T>::addEdge(T source, T destination, int weight) {
    // Add an edge between the source and destination vertices with the given weight
    // source: data of the source vertex
    // destination: data of the destination vertex
    // weight: weight of the edge (0 for unweighted graphs)

    // Solution:
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
    // Remove the vertex with the given data from the graph
    // data: data of the vertex to be removed

    // Solution:
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
    // Remove the edge between the source and destination vertices
    // source: data of the source vertex
    // destination: data of the destination vertex

    // Solution:
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
    // data: data of the vertex to be returned

    // Solution:
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

    // Solution:
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

    // Solution:
        return vertices;

}

template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getAllEdges() {
    // Return all the edges in the graph

    // Solution:
}

template <class T>
vector<shared_ptr<Edge<T>>> Graph<T>::getEdges(shared_ptr<Vertex<T>> vertex) {
    // Return all the edges connected to the given vertex
    // vertex: vertex whose edges are to be returned

    // Solution:
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    // Return all the adjacent vertices of the given vertex
    // vertex: vertex whose adjacent vertices are to be returned

    // Solution:
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getInAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    // Return all the incoming adjacent vertices of the given vertex
    // vertex: vertex whose incoming adjacent vertices are to be returned

    // Solution:
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::getOutAdjacentVertices(shared_ptr<Vertex<T>> vertex) {
    // Return all the outgoing adjacent vertices of the given vertex
    // vertex: vertex whose outgoing adjacent vertices are to be returned

    // Solution:
}

template <class T>
bool Graph<T>::isDirected() {
    // Return true if the graph is directed, false otherwise

    // Solution:
}

template <class T>
bool Graph<T>::isWeighted() {
    // Return true if the graph is weighted, false otherwise

    // Solution:
}

template <class T>
void Graph<T>::setDirected(bool directed) {
    // set the direction status

    // Solution:
}

template <class T>
void Graph<T>::setWeighted(bool weighted) {
    // set the weighted status

    // Solution:
}

template <class T>
void Graph<T>::updateAdjacencyMatrix() {
    // Update the adjacency matrix of the graph
    // 1 if edge exists, 0 otherwise for undirected graphs
    // weight of the edge for weighted graphs, 0 otherwise

    // Solution:
}

template <class T>
vector<vector<int>> Graph<T>::getAdjacencyMatrix() {
    // Return the adjacency matrix of the graph

    // Solution:
}

template <class T>
void Graph<T>::printAdjacencyMatrix() {
    // Print the adjacency matrix of the graph with row and column headers shown

    // Solution:
}

template <class T>
void Graph<T>::printAdjacencyList() {
    // Print the adjacency list of the graph

    // Solution:
}


// PROVIDED FUNCTION FOR HELP - DO NOT MODIFY - USE ONLY TO VISUALISE THE GRAPH - OPTIONAL FUNCTION YOU CAN USE BY DOWNLOADING GRAPHVIZ 
// LINKS PROVIDED IN THE FUNCTION - YOU CAN IGNORE THIS FUNCTION IF YOU DO NOT WANT TO VISUALISE THE GRAPH
template <class T>
void Graph<T>::printGraph(string fileName) {
    // Print the graph in a Graphviz DOT format with arrows for directed graphs and lines for undirected graphs
    // https://graphviz.org/download/source/ -> Graphviz Download link for Macbook Users (Download latest stable version)
    // https://graphviz.org/download/ -> Graphviz Download link for Windows Users (Download latest stable version)
    // Follow the commands to install Graphviz on your system to visualise the graph
    // printGraph() function will create a file named graph.dot in the current directory
    // dot -Tsvg graph.dot -o graph.svg -> This would convert your dot file to a svg image

    // Solution:
    ofstream dotFile(fileName);
    dotFile << (directed ? "digraph" : "graph") << " G {" << endl;
    dotFile << "    graph [pad=\"0.5\", nodesep=\"0.8\", ranksep=\"1.2\", margin=0.5];" << endl;
    dotFile << "    node [shape=ellipse, style=filled, color=lightblue, fontname=\"Helvetica\", fontsize=16, width=0.5, height=0.5];" << endl;
    dotFile << "    edge [color=dimgray, penwidth=2.0, arrowhead=open];" << endl;
    dotFile << "    rankdir=LR;" << endl;
    dotFile << "    label=\"Graph\";" << endl;
    dotFile << "    labelloc=top;" << endl;


    for (auto edge : edges) {
        dotFile << "    " << edge->getSource()->getData() << (directed ? " -> " : " -- ") << edge->getDestination()->getData();
        if (weighted) {
            dotFile << " [label=\"" << edge->getWeight() << "\"]";
        }
        dotFile << ";" << endl;
    }
    dotFile << "}" << endl;
    dotFile.close();
    cout << "Graph exported to graph.dot (Graphviz format)" << endl;
}