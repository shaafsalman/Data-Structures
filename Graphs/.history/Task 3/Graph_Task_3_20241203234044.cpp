#include "../Task 2/Graph_Task_2.cpp"
#include <functional> 
#include <unordered_set>






#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <functional>








template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    if (!source || !destination) {
        cout << "Debug: Source or Destination is null" << endl;
        return {};
    }

    if (source == destination) {
        cout << "Debug: Source equals Destination" << endl;
        return {source};
    }

    if (vertices.empty() || edges.empty()) {
        cout << "Debug: Graph is empty" << endl;
        return {};
    }

    if (weighted) {
        cout << "Debug: Using Dijkstra's Algorithm" << endl;

        priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<>> pq;
        unordered_map<shared_ptr<Vertex<T>>, int> distances;
        unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;

        for (auto vertex : vertices) {
            distances[vertex] = INT_MAX;
        }

        distances[source] = 0;
        pq.emplace(0, source);

        while (!pq.empty()) {
            auto [currentDistance, currentVertex] = pq.top();
            pq.pop();

            cout << "Debug: Visiting Vertex " << currentVertex->getData() << " with distance " << currentDistance << endl;

            if (currentVertex == destination) {
                cout << "Debug: Reached Destination" << endl;
                break;
            }

            for (auto edge : getEdges(currentVertex)) {
                auto neighbor = edge->getDestination();
                int weight = edge->getWeight();
                int newDistance = currentDistance + weight;

                cout << "Debug: Checking neighbor " << neighbor->getData() << " with edge weight " << weight << endl;

                if (newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                    parent[neighbor] = currentVertex;
                    pq.emplace(newDistance, neighbor);
                    cout << "Debug: Updated distance for " << neighbor->getData() << " to " << newDistance << endl;
                }
            }
        }

        vector<shared_ptr<Vertex<T>>> path;
        for (auto v = destination; v != nullptr; v = parent[v]) {
            path.push_back(v);
            if (v == source) break;
        }

        reverse(path.begin(), path.end());

        if (path.empty() || path.back() != destination) {
            cout << "Debug: Path is empty or does not reach destination" << endl;
            return {};
        }

        cout << "Debug: Path Found" << endl;
        for (auto vertex : path) {
            cout << vertex->getData() << " ";
        }
        cout << endl;

        return path;
    }

    cout << "Debug: Graph is unweighted - Using BFS" << endl;
    // Unweighted graph code (BFS) would go here if needed for testing unweighted graphs
    return {};
}




















template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    if (!directed) {
        cout << "Topological Sort is only applicable to directed graphs." << endl;
        return {};
    }

    // Calculate in-degrees
    unordered_map<shared_ptr<Vertex<T>>, int> inDegree;
    for (auto vertex : vertices) {
        inDegree[vertex] = 0;  // Initialize all vertices with in-degree 0
    }
    for (auto edge : edges) {
        inDegree[edge->getDestination()]++;
    }

    // Queue for vertices with in-degree 0
    queue<shared_ptr<Vertex<T>>> q;
    for (auto vertex : vertices) {
        if (inDegree[vertex] == 0) {
            q.push(vertex);
        }
    }

    vector<shared_ptr<Vertex<T>>> result;
    while (!q.empty()) {
        auto vertex = q.front();
        q.pop();
        result.push_back(vertex);
        
        for (auto adj : getAdjacentVertices(vertex)) {
            inDegree[adj]--;
            if (inDegree[adj] == 0) {
                q.push(adj);
            }
        }
    }
    
    // If there is a cycle, the result will have fewer vertices
    if (result.size() != vertices.size()) {
        cout << "Graph has a cycle, topological sort is not possible." << endl;
        return {};
    }
    return result;
}








template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    if (!isWeighted()) {
        cout << "The graph must be weighted to compute a Minimum Spanning Tree." << endl;
        return nullptr;
    }

    auto mst = make_shared<Graph<T>>(false, true); // MST is always undirected and weighted

    // Use Kruskal's Algorithm for Minimum Spanning Tree
    vector<tuple<int, shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>>> edges;
    for (auto edge : getAllEdges()) {
        edges.push_back({edge->getWeight(), edge->getSource(), edge->getDestination()});
    }

    // Sort edges based on their weights
    sort(edges.begin(), edges.end(), [](const auto &a, const auto &b) {
        return get<0>(a) < get<0>(b);
    });

    // Union-Find data structures
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;
    for (auto vertex : getAllVertices()) {
        parent[vertex] = vertex;
    }

    function<shared_ptr<Vertex<T>>(shared_ptr<Vertex<T>>)> findParent = [&](shared_ptr<Vertex<T>> v) {
        if (parent[v] == v) return v;
        return parent[v] = findParent(parent[v]);
    };

    auto unionVertices = [&](shared_ptr<Vertex<T>> u, shared_ptr<Vertex<T>> v) {
        parent[findParent(u)] = findParent(v);
    };

    // Add vertices to the MST
    for (auto vertex : getAllVertices()) {
        mst->addVertex(vertex->getData());
    }

    // Process edges to construct MST
    for (auto &[weight, u, v] : edges) {
        if (findParent(u) != findParent(v)) {
            mst->addEdge(u->getData(), v->getData(), weight);
            unionVertices(u, v);
        }
    }

    return mst;
}


template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    vector<shared_ptr<Graph<T>>> allSpanningTrees;

    // Sort the edges based on weight if the graph is weighted
    vector<shared_ptr<Edge<T>>> allEdges = this->edges;
    sort(allEdges.begin(), allEdges.end(), [](const shared_ptr<Edge<T>>& a, const shared_ptr<Edge<T>>& b) {
        return a->getWeight() < b->getWeight(); // assuming Edge<T> has a `getWeight()` method
    });

    // Helper function to find the root of a set
    auto findRoot = [](vector<int>& parent, int i) {
        if (parent[i] == i)
            return i;
        return findRoot(parent, parent[i]);
    };

    // Helper function to perform union of two sets
    auto unionSets = [](vector<int>& parent, vector<int>& rank, int x, int y) {
        int rootX = findRoot(parent, x);
        int rootY = findRoot(parent, y);

        if (rootX != rootY) {
            // Union by rank
            if (rank[rootX] > rank[rootY])
                parent[rootY] = rootX;
            else if (rank[rootX] < rank[rootY])
                parent[rootX] = rootY;
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    };

    // Generate one spanning tree (MST) using Kruskal’s algorithm
    vector<int> parent(vertices.size());
    vector<int> rank(vertices.size(), 0);
    
    // Initialize each vertex as its own parent
    for (int i = 0; i < vertices.size(); i++) {
        parent[i] = i;
    }

    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>(this->directed, this->weighted);

    for (const auto& edge : allEdges) {
        int u = edge->getSource()->getData();
        int v = edge->getDestination()->getData();

        int rootU = findRoot(parent, u);
        int rootV = findRoot(parent, v);

        // If adding this edge doesn't form a cycle, include it in the MST
        if (rootU != rootV) {
            unionSets(parent, rank, rootU, rootV);
            mst->addEdge(u, v, edge->getWeight());
        }
    }

    allSpanningTrees.push_back(mst);  // Add the first found spanning tree to the result

    // This is a simplified version of finding a single spanning tree using Kruskal's algorithm.
    // To find multiple spanning trees, you will need to modify the algorithm and use backtracking or explore different edge combinations.
    
    // Return the list of all spanning trees found
    return allSpanningTrees;
}


// ::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::

// BONUS TASK 1 FOR 5 MARKS
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    // Find all the spanning trees of the graph
    // Return the spanning trees as a vector of Graph objects

    // Solution:
}

// BONUS TASK 2 FOR 5 MARKS

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    // Find all the connected components of the graph
    // A connected component is a set of vertices where each vertex is reachable from any other vertex in the same set
    // Return the connected components as a vector of vectors of vertices

    // Solution:
    vector<vector<shared_ptr<Vertex<T>>>> components;
    unordered_set<shared_ptr<Vertex<T>>> visited;

    for (auto vertex : vertices) {
        if (visited.find(vertex) == visited.end()) {
            // Perform DFS to find all vertices in the same component
            vector<shared_ptr<Vertex<T>>> component;
            stack<shared_ptr<Vertex<T>>> toVisit;
            toVisit.push(vertex);

            while (!toVisit.empty()) {
                auto currentVertex = toVisit.top();
                toVisit.pop();

                if (visited.find(currentVertex) == visited.end()) {
                    visited.insert(currentVertex);
                    component.push_back(currentVertex);

                    // Visit all adjacent vertices
                    for (auto adjVertex : getAdjacentVertices(currentVertex)) {
                        if (visited.find(adjVertex) == visited.end()) {
                            toVisit.push(adjVertex);
                        }
                    }
                }
            }

            components.push_back(component);
        }
    }

    return components;
}
