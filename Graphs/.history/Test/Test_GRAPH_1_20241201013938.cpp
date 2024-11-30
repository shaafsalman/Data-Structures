#include "../Task 1/Graph_Task_1.cpp"
#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

using namespace std;

// ANSI color codes
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string RESET = "\033[0m";

// Global marks variable
float totalMarks = 0;

// Helper function to describe the graph configuration
string describeGraph(bool directed, bool weighted) {
    string desc = (directed ? "Directed" : "Undirected");
    desc += (weighted ? " & Weighted" : " & Unweighted");
    return desc;
}

void testGraphParameterizedConstructor(bool directed, bool weighted) {
    try {
        Graph<int> graph(directed, weighted);
        assert(graph.isDirected() == directed);
        assert(graph.isWeighted() == weighted);

        totalMarks += 0.5;
        cout << GREEN << "Test 3.2 (" << describeGraph(directed, weighted) << ") -> Passed (0.5 mark)" << RESET << "\n";
    } catch (...) {
        cout << RED << "Test 3.2 (" << describeGraph(directed, weighted) << ") -> Failed" << RESET << "\n";
    }
}

void testAddVertexAndEdges(bool directed, bool weighted) {
    try {
        Graph<int> graph(directed, weighted);

        graph.addVertex(1);
        graph.addVertex(2);
        if (weighted) graph.addEdge(1, 2, 10); // Weighted
        else graph.addEdge(1, 2, 0);           // Unweighted

        auto vertices = graph.getAllVertices();
        auto edges = graph.getAllEdges();

        assert(vertices.size() == 2);
        assert(edges.size() == 1);
        if (weighted) assert(edges[0]->getWeight() == 10);

        totalMarks += 0.75;
        cout << GREEN << "Test 3.3 (" << describeGraph(directed, weighted) << ") -> Passed (0.75 mark)" << RESET << "\n";
    } catch (...) {
        cout << RED << "Test 3.3 (" << describeGraph(directed, weighted) << ") -> Failed" << RESET << "\n";
    }
}

void testRemoveVertex(bool directed, bool weighted) {
    try {
        Graph<int> graph(directed, weighted);

        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, (weighted ? 10 : 0));
        graph.removeVertex(1);

        auto vertices = graph.getAllVertices();
        auto edges = graph.getAllEdges();
        assert(vertices.size() == 1);
        assert(edges.empty());

        totalMarks += 0.75;
        cout << GREEN << "Test 3.4 (" << describeGraph(directed, weighted) << ") -> Passed (0.75 mark)" << RESET << "\n";
    } catch (...) {
        cout << RED << "Test 3.4 (" << describeGraph(directed, weighted) << ") -> Failed" << RESET << "\n";
    }
}

void testRemoveEdge(bool directed, bool weighted) {
    try {
        Graph<int> graph(directed, weighted);

        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, (weighted ? 10 : 0));
        graph.removeEdge(1, 2);

        auto edges = graph.getAllEdges();
        assert(edges.empty());

        totalMarks += 0.75;
        cout << GREEN << "Test 3.5 (" << describeGraph(directed, weighted) << ") -> Passed (0.75 mark)" << RESET << "\n";
    } catch (...) {
        cout << RED << "Test 3.5 (" << describeGraph(directed, weighted) << ") -> Failed" << RESET << "\n";
    }
}

void testAdjacencyMatrix(bool directed, bool weighted) {
    try {
        Graph<int> graph(directed, weighted);

        graph.addVertex(1); // Index 0
        graph.addVertex(2); // Index 1
        graph.addVertex(3); // Index 2

        if (weighted) {
            graph.addEdge(1, 2, 5);  // Weighted edge from 1->2
            graph.addEdge(2, 3, 10); // Weighted edge from 2->3
        } else {
            graph.addEdge(1, 2, 0); // Unweighted edge from 1->2
            graph.addEdge(2, 3, 0); // Unweighted edge from 2->3
        }


        graph.updateAdjacencyMatrix();

        auto matrix = graph.getAdjacencyMatrix();
        cout << "Adjacency Matrix: \n";
        for (const auto& row : matrix) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }

        // Assertions based on weighted and directed configurations
        if (weighted) {
            assert(matrix[0][1] == 5);  // Weight of edge 1->2
            if (directed) {
                assert(matrix[1][0] == 0); // No reverse edge for directed graph
            } else {
            assert(matrix[1][0] == 5);  // Symmetric for undirected graph
            }
            assert(matrix[1][2] == 10); // Weight of edge 2->3
            if (directed) {
                assert(matrix[2][1] == 0); // No reverse edge for directed graph
            } else {
            assert(matrix[2][1] == 10); // Symmetric for undirected graph
            }
        } else {
            assert(matrix[0][1] == 1);  // Edge exists (1->2)
            if (directed) {
                assert(matrix[1][0] == 0); // No reverse edge for directed graph
            } else {
            assert(matrix[1][0] == 1);  // Symmetric for undirected graph
            }
            assert(matrix[1][2] == 1);  // Edge exists (2->3)
            if (directed) {
                assert(matrix[2][1] == 0); // No reverse edge for directed graph
            } else {
            assert(matrix[2][1] == 1);  // Symmetric for undirected graph
            }
        }

        if (directed) {
            // For directed graphs, ensure no reverse edge unless explicitly added
            assert(matrix[2][1] == 0); // No edge from 3 to 2
        }

        totalMarks += 0.5;
        cout << GREEN << "Test 3.6 (" << describeGraph(directed, weighted) << ") -> Passed (0.5 mark)" << RESET << "\n";
    } catch (...) {
        cout << RED << "Test 3.6 (" << describeGraph(directed, weighted) << ") -> Failed" << RESET << "\n";
    }
}

void testAdjacencyList(bool directed, bool weighted) {
    try {
        Graph<int> graph(directed, weighted);

        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 0);

        auto vertices = graph.getAllVertices();
        auto edges = vertices[0]->getEdges();

        assert(edges.size() == 1);
        assert(edges[0]->getDestination()->getData() == 2);

        totalMarks += 0.5;
        cout << GREEN << "Test 3.7 (" << describeGraph(directed, weighted) << ") -> Passed (0.5 mark)" << RESET << "\n";
    } catch (...) {
        cout << RED << "Test 3.7 (" << describeGraph(directed, weighted) << ") -> Failed" << RESET << "\n";
    }
}

int main() {
    cout << "Running tests for Graph...\n";

    vector<pair<bool, bool>> configurations = {
        {false, false}, // Undirected & Unweighted
        {false, true},  // Undirected & Weighted
        {true, false},  // Directed & Unweighted
        {true, true}    // Directed & Weighted
    };

    for (auto [directed, weighted] : configurations) {
        cout << "\nTesting configuration: " << describeGraph(directed, weighted) << "\n";
        testGraphParameterizedConstructor(directed, weighted);
        testAddVertexAndEdges(directed, weighted);
        testRemoveVertex(directed, weighted);
        testRemoveEdge(directed, weighted);
        testAdjacencyMatrix(directed, weighted);
        testAdjacencyList(directed, weighted);
    }

    cout << "\nTotal Marks: " << totalMarks << "/15\n";
    return 0;
}