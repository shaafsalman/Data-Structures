#include "../Task 1/Vertex.cpp"
#include <iostream>
#include <memory>
#include <cassert>
#include <vector>

using namespace std;

// ANSI color codes
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string RESET = "\033[0m";
float marks = 0;
void testVertexDefaultConstructor() {
    try {
        Vertex<int> vertex;

        assert(vertex.getData() == 0); // Default-initialized value for int
        assert(vertex.getEdges().empty());
        assert(vertex.isVisited() == false);

        cout << GREEN << "Test 2.1 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 2.1 -> Failed" << RESET << "\n";
    }
}

void testVertexParameterizedConstructor() {
    try {
        Vertex<string> vertex("Vertex1");

        assert(vertex.getData() == "Vertex1");
        assert(vertex.getEdges().empty());
        assert(vertex.isVisited() == false);

        cout << GREEN << "Test 2.2 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 2.2 -> Failed" << RESET << "\n";
    }
}

void testAddEdge() {
    try {
        auto vertex1 = make_shared<Vertex<int>>(1);
        auto vertex2 = make_shared<Vertex<int>>(2);

        auto edge = make_shared<Edge<int>>(vertex1, vertex2, 10, false);

        vertex1->addEdge(edge);

        assert(vertex1->getEdges().size() == 1);
        assert(vertex1->getEdges()[0] == edge);

        cout << GREEN << "Test 2.3 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 2.3 -> Failed" << RESET << "\n";
    }
}

void testRemoveEdge() {
    try {
        auto vertex1 = make_shared<Vertex<int>>(1);
        auto vertex2 = make_shared<Vertex<int>>(2);

        auto edge = make_shared<Edge<int>>(vertex1, vertex2, 10, false);

        vertex1->addEdge(edge);
        vertex1->removeEdge(2);

        assert(vertex1->getEdges().empty());

        cout << GREEN << "Test 2.4 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 2.4 -> Failed" << RESET << "\n";
    }
}

void testVisitedStatus() {
    try {
        Vertex<int> vertex(5);

        assert(vertex.isVisited() == false);

        vertex.setVisited(true);
        assert(vertex.isVisited() == true);

        cout << GREEN << "Test 2.5 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 2.5 -> Failed" << RESET << "\n";
    }
}

int main() {
    testVertexDefaultConstructor();  // Test default constructor
    testVertexParameterizedConstructor(); // Test parameterized constructor
    testAddEdge();                   // Test adding an edge
    testRemoveEdge();                // Test removing an edge
    testVisitedStatus();             // Test visited flag functionality
    cout << "Marks: " << marks << "/5" << endl;
    return 0;
}