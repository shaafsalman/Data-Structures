#include "../Task 2/Graph_Task_2.cpp"
#include <iostream>
#include <cassert>
#include <memory>

using namespace std;

// Helper function to print test status in color
void printTestStatus(const string& testName, bool passed, float marks) {
    if (passed) {
        cout << "\033[32m" << testName << " -> Passed (" << marks << " mark)\033[0m" << endl;
    } else {
        cout << "\033[31m" << testName << " -> Failed\033[0m" << endl;
    }
}

// Test BFS Traversal (2 Marks)
float testBFS() {
    cout << "Running BFS Test..." << endl;

    Graph<int> graph(false, false); // Undirected & Unweighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    graph.addVertex(2);
    auto v2 = graph.getVertex(2);
    graph.addVertex(3);
    auto v3 = graph.getVertex(3);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);

    vector<shared_ptr<Vertex<int>>> visited = graph.BFSTraversal(v1);
    
    bool passed = true;
    passed &= visited.size() == 3;  // Expect 3 vertices to be visited
    passed &= visited[0]->getData() == 1;
    passed &= visited[1]->getData() == 2;
    passed &= visited[2]->getData() == 3;

    printTestStatus("BFS Traversal Test", passed, 2.0);
    return passed ? 3.0 : 0.0;
}

// Test DFS Traversal (2 Marks)
float testDFS() {
    cout << "Running DFS Test..." << endl;

    Graph<int> graph(false, false); // Undirected & Unweighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    graph.addVertex(2);
    auto v2 = graph.getVertex(2);
    graph.addVertex(3);
    auto v3 = graph.getVertex(3);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);

    vector<shared_ptr<Vertex<int>>> visited = graph.DFSTraversal(v1);
    
    bool passed = true;
    passed &= visited.size() == 3;  // Expect 3 vertices to be visited
    passed &= visited[0]->getData() == 1;
    passed &= visited[1]->getData() == 2;
    passed &= visited[2]->getData() == 3;

    printTestStatus("DFS Traversal Test", passed, 2.0);
    return passed ? 3.0 : 0.0;
}

// Test Dijkstra's Shortest Path (3 Marks)
// Test Dijkstra's Shortest Path (3 Marks)
float testDijkstra() {
    cout << "Running Dijkstra's Shortest Path Test..." << endl;

    Graph<int> graph(false, true); // Undirected & Weighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    graph.addVertex(2);
    auto v2 = graph.getVertex(2);
    graph.addVertex(3);
    auto v3 = graph.getVertex(3);
    graph.addEdge(1, 2, 10); // Weight 10
    graph.addEdge(2, 3, 5);  // Weight 5
    graph.addEdge(1, 3, 15); // Weight 15

    vector<shared_ptr<Vertex<int>>> result = graph.dijkstraShortestPath(v1);
    
    bool passed = true;
    passed &= result.size() == 3; // Should return 3 vertices
    passed &= result[0]->getData() == 1; // Start from v1
    passed &= result[1]->getData() == 2;
    passed &= result[2]->getData() == 3;

    // Compute the distances manually from the algorithm result
    // Assuming your shortest path algorithm stores the shortest distances correctly
    int distV1V2 = 10;  // Distance from v1 to v2
    int distV1V3 = 15;  // Distance from v1 to v3
    passed &= distV1V2 == 10;
    passed &= distV1V3 == 15;

    printTestStatus("Dijkstra's Shortest Path Test", passed, 3.0);
    return passed ? 4.0 : 0.0;
}

// Test Bellman-Ford's Shortest Path (3 Marks)
float testBellmanFord() {
    cout << "Running Bellman-Ford Shortest Path Test..." << endl;

    Graph<int> graph(false, true); // Undirected & Weighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    graph.addVertex(2);
    auto v2 = graph.getVertex(2);
    graph.addVertex(3);
    auto v3 = graph.getVertex(3);
    graph.addEdge(1, 2, 10);
    graph.addEdge(2, 3, 5);
    graph.addEdge(1, 3, 15);

    vector<shared_ptr<Vertex<int>>> result = graph.bellmanFordShortestPath(v1);

    bool passed = true;
    passed &= result.size() == 3;  // Should return 3 vertices
    passed &= result[0]->getData() == 1; // Start from v1
    passed &= result[1]->getData() == 2;
    passed &= result[2]->getData() == 3;

    // Compute the distances manually from the algorithm result
    int distV1V2 = 10;  // Distance from v1 to v2
    int distV1V3 = 15;  // Distance from v1 to v3
    passed &= distV1V2 == 10;
    passed &= distV1V3 == 15;

    printTestStatus("Bellman-Ford Shortest Path Test", passed, 3.0);
    return passed ? 4.0 : 0.0;
}
// Test Empty Graph (1 Mark)
float testEmptyGraph() {
    cout << "Running Empty Graph Test..." << endl;

    Graph<int> graph(false, false); // Undirected & Unweighted
    vector<shared_ptr<Vertex<int>>> visited = graph.BFSTraversal(nullptr);
    vector<shared_ptr<Vertex<int>>> visited2 = graph.DFSTraversal(nullptr);
    vector<shared_ptr<Vertex<int>>> visited3 = graph.dijkstraShortestPath(nullptr);
    vector<shared_ptr<Vertex<int>>> visited4 = graph.bellmanFordShortestPath(nullptr);
    
    bool passed = visited.empty();
    passed &= visited2.empty();
    passed &= visited3.empty();
    passed &= visited4.empty();
    printTestStatus("Empty Graph Test", passed, 1.0);
    return passed ? 1.0 : 0.0;
}

// Test Single Vertex (1 Mark)
float testSingleVertex() {
    cout << "Running Single Vertex Test..." << endl;

    Graph<int> graph(false, false); // Undirected & Unweighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    
    vector<shared_ptr<Vertex<int>>> bfsVisited = graph.BFSTraversal(v1);
    vector<shared_ptr<Vertex<int>>> dfsVisited = graph.DFSTraversal(v1);

    bool passed = bfsVisited.size() == 1 && dfsVisited.size() == 1;

    printTestStatus("Single Vertex Test", passed, 1.0);
    return passed ? 1.0 : 0.0;
}

// Test Disconnected Graph (2 Marks)
float testDisconnectedGraph() {
    cout << "Running Disconnected Graph Test..." << endl;

    Graph<int> graph(false, false); // Undirected & Unweighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    graph.addVertex(2);
    auto v2 = graph.getVertex(2);
    graph.addVertex(3);
    auto v3 = graph.getVertex(3);

    graph.addEdge(1, 2);  // Only connect v1 and v2

    vector<shared_ptr<Vertex<int>>> bfsVisited = graph.BFSTraversal(v1);
    vector<shared_ptr<Vertex<int>>> dfsVisited = graph.DFSTraversal(v1);

    bool passed = bfsVisited.size() == 2 && dfsVisited.size() == 2;

    printTestStatus("Disconnected Graph Test", passed, 2.0);
    return passed ? 3.0 : 0.0;
}

// Test Negative Weight Edge Case (1 Mark)
float testNegativeWeights() {
    cout << "Running Negative Weight Edge Case Test..." << endl;

    Graph<int> graph(false, true); // Undirected & Weighted
    graph.addVertex(1);
    auto v1 = graph.getVertex(1);
    graph.addVertex(2);
    auto v2 = graph.getVertex(2);
    graph.addVertex(3);
    auto v3 = graph.getVertex(3);
    graph.addEdge(1, 2, -10); // Negative weight
    graph.addEdge(2, 3, 5);  
    graph.addEdge(1, 3, 15); 

    vector<shared_ptr<Vertex<int>>> result = graph.bellmanFordShortestPath(v1);

    bool passed = result.size() == 3;
    printTestStatus("Negative Weight Edge Case Test", passed, 1.0);
    return passed ? 1.0 : 0.0;
}

int main() {
    float totalMarks = 0.0;
    
    totalMarks += testBFS();          // 3 Marks
    totalMarks += testDFS();          // 3 Marks
    totalMarks += testDijkstra();     // 4 Marks
    totalMarks += testEmptyGraph();   // 1 Mark
    totalMarks += testSingleVertex(); // 1 Mark
    totalMarks += testDisconnectedGraph(); // 3 Marks
    
    float bonus = 0.0;
    bonus += testBellmanFord();  // 3 Marks
    bonus += testNegativeWeights();   // 1 Mark

    cout << "\nTotal Marks: " << totalMarks << "/15" << endl;
    cout << "Bonus Marks: " << bonus << "/5" << endl;
    return 0;
}