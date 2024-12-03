#include <iostream>
#include <memory>
#include "../Task 3/Graph_Task_3.cpp"

using namespace std;

int main() {
    int totalScore = 0;
    int bonus = 0;

    // Test 1: Directed, Weighted Graph - Topological Sort (5 marks)
    cout << "Test 1: Topological Sort (5 marks)" << endl;
    Graph<int> graph1(true, false); // Directed and Unweighted graph
    graph1.addVertex(1);
    graph1.addVertex(2);
    graph1.addVertex(3);
    graph1.addVertex(4);
    graph1.addEdge(1, 2);
    graph1.addEdge(1, 3);
    graph1.addEdge(3, 4);
    graph1.addEdge(2, 4);

    auto topologicalOrder = graph1.topologicalSort();
    cout << "Topological Order: ";
    for (const auto& vertex : topologicalOrder) {
        cout << vertex->getData() << " ";
    }
    cout << endl;
    if (topologicalOrder.size() == 4) {
        cout << "Topological Sort passed! +5 marks" << endl;
        totalScore += 5;
    } else {
        cout << "Topological Sort failed." << endl;
    }
    cout << endl;

    // // Test 2: Undirected, Weighted Graph - Minimum Spanning Tree (5 marks)
    // cout << "Test 2: Minimum Spanning Tree (5 marks)" << endl;
    // Graph<int> graph2(false, true); // Undirected and Weighted graph
    // graph2.addVertex(1);
    // graph2.addVertex(2);
    // graph2.addVertex(3);
    // graph2.addVertex(4);
    // graph2.addEdge(1, 2, 1);
    // graph2.addEdge(1, 3, 4);
    // graph2.addEdge(2, 3, 2);
    // graph2.addEdge(3, 4, 3);

    // auto mst = graph2.minimumSpanningTree();
    // cout << "Minimum Spanning Tree Edges:" << endl;
    // auto mstEdges = mst->getAllEdges();
    // int mstEdgeCount = 0;
    // for (const auto& edge : mstEdges) {
    //     cout << edge->getSource()->getData() << " -> " << edge->getDestination()->getData()
    //          << " [Weight: " << edge->getWeight() << "]" << endl;
    //     mstEdgeCount++;
    // }
    // if (mstEdgeCount == 3) {
    //     cout << "Minimum Spanning Tree passed! +5 marks" << endl;
    //     totalScore += 5;
    // } else {
    //     cout << "Minimum Spanning Tree failed." << endl;
    // }
    // cout << endl;

    // Test 3: Shortest Path (5 marks)
    cout << "Test 3: Shortest Path (5 marks)" << endl;
    Graph<int> graph3(true, true); // Directed and Weighted graph
    graph3.addVertex(1);
    graph3.addVertex(2);
    graph3.addVertex(3);
    graph3.addVertex(4);
    graph3.addEdge(1, 2, 1);
    graph3.addEdge(2, 3, 2);
    graph3.addEdge(1, 3, 4);
    graph3.addEdge(3, 4, 1);

    auto shortestPath = graph3.shortestPath(graph3.getVertex(1), graph3.getVertex(4));
    cout << "Shortest Path from 1 to 4: ";
    for (const auto& vertex : shortestPath) {
        cout << vertex->getData() << " ";
    }
    cout << endl;
    if (!shortestPath.empty() && shortestPath.size() == 3) { // Assuming correct path length
        cout << "Shortest Path passed! +5 marks" << endl;
        totalScore += 5;
    } else {
        cout << "Shortest Path failed." << endl;
    }
    cout << endl;












    // Bonus Test 1: Spanning Trees (5 bonus marks)
    cout << "Bonus Test 1: Spanning Trees (5 bonus marks)" << endl;
    Graph<int> graph4(false, true); // Undirected and Weighted graph
    graph4.addVertex(1);
    graph4.addVertex(2);
    graph4.addVertex(3);
    graph4.addEdge(1, 2, 1);
    graph4.addEdge(2, 3, 2);
    graph4.addEdge(1, 3, 3);

    auto spanningTrees = graph4.SpanningTrees();
    cout << "Spanning Trees count: " << spanningTrees.size() << endl;
    if (!spanningTrees.empty()) {
        cout << "Spanning Trees passed! +5 bonus marks" << endl;
        bonus += 5;
    } else {
        cout << "Spanning Trees failed." << endl;
    }
    cout << endl;

    // Bonus Test 2: Connected Components (5 bonus marks)
    cout << "Bonus Test 2: Connected Components (5 bonus marks)" << endl;
    Graph<int> graph5(false, false); // Undirected and Unweighted graph
    graph5.addVertex(1);
    graph5.addVertex(2);
    graph5.addVertex(3);
    graph5.addVertex(4);
    graph5.addVertex(5);
    graph5.addEdge(1, 2);
    graph5.addEdge(3, 4);

    auto components = graph5.connectedComponents();
    cout << "Connected Components count: " << components.size() << endl;
    for (size_t i = 0; i < components.size(); i++) {
        cout << "Component " << i + 1 << ": ";
        for (const auto& vertex : components[i]) {
            cout << vertex->getData() << " ";
        }
        cout << endl;
    }
    if (components.size() == 3) { // Assuming expected 3 components
        cout << "Connected Components passed! +5 bonus marks" << endl;
        bonus += 5;
    } else {
        cout << "Connected Components failed." << endl;
    }
    cout << endl;

    // Final Score
    cout << "Total Score: " << totalScore << "/15 " << endl;
    cout << "Bonus: " << bonus << "/10 " << endl;

    return 0;
}