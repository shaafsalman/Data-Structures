#include "../Task 1/Vertex.cpp"
#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

// ANSI color codes
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string RESET = "\033[0m";
float marks = 0;

void testEdgeWithInt() {
    try {
        auto vertex1 = make_shared<Vertex<int>>(1);
        auto vertex2 = make_shared<Vertex<int>>(2);
        Edge<int> edge(vertex1, vertex2, 10, true);

        assert(edge.getSource() == vertex1);
        assert(edge.getDestination() == vertex2);
        assert(edge.getWeight() == 10);
        assert(edge.isDirected() == true);

        cout << GREEN << "Test 1.1 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 1.1 -> Failed" << RESET << "\n";
    }
}

void testEdgeWithString() {
    try {
        auto vertex1 = make_shared<Vertex<string>>("A");
        auto vertex2 = make_shared<Vertex<string>>("B");
        Edge<string> edge(vertex1, vertex2, 5, false);

        assert(edge.getSource() == vertex1);
        assert(edge.getDestination() == vertex2);
        assert(edge.getWeight() == 5);
        assert(edge.isDirected() == false);

        edge.setWeight(7);
        assert(edge.getWeight() == 7);

        cout << GREEN << "Test 1.2 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 1.2 -> Failed" << RESET << "\n";
    }
}

void testEdgeWithDouble() {
    try {
        auto vertex1 = make_shared<Vertex<double>>(1.1);
        auto vertex2 = make_shared<Vertex<double>>(2.2);
        Edge<double> edge(vertex1, vertex2, 15, true);

        assert(edge.getSource() == vertex1);
        assert(edge.getDestination() == vertex2);
        assert(edge.getWeight() == 15);
        assert(edge.isDirected() == true);

        cout << GREEN << "Test 1.3 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 1.3 -> Failed" << RESET << "\n";
    }
}

void testEdgeDefaultConstructor() {
    try {
        Edge<int> edge;

        assert(edge.getSource() == nullptr);
        assert(edge.getDestination() == nullptr);
        assert(edge.getWeight() == 0);
        assert(edge.isDirected() == false);

        cout << GREEN << "Test 1.4 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 1.4 -> Failed" << RESET << "\n";
    }
}

void testEdgeSetWeight() {
    try {
        auto vertex1 = make_shared<Vertex<int>>(1);
        auto vertex2 = make_shared<Vertex<int>>(2);
        Edge<int> edge(vertex1, vertex2, 0, true);

        edge.setWeight(25);
        assert(edge.getWeight() == 25);

        cout << GREEN << "Test 1.5 -> Passed" << RESET << "\n";
        marks += 1;
    } catch (...) {
        cout << RED << "Test 1.5 -> Failed" << RESET << "\n";
    }
}

int main() {
    testEdgeWithInt();         
    testEdgeWithString();      
    testEdgeWithDouble();      
    testEdgeDefaultConstructor(); 
    testEdgeSetWeight();
    cout << "Marks: " << marks << "/5" << endl;
    return 0;
}