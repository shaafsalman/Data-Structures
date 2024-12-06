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
        // cout << "Debug: Source or Destination is null" << endl;
        return {};
    }

    if (source == destination) {
        // cout << "Debug: Source equals Destination" << endl;
        return {source};
    }

    if (vertices.empty() || edges.empty()) {
        // cout << "Debug: Graph is empty" << endl;
        return {};
    }

    if (weighted) {
        // cout << "Debug: Using Dijkstra's Algorithm" << endl;

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

            // cout << "Debug: Visiting Vertex " << currentVertex->getData() << " with distance " << currentDistance << endl;

            if (currentVertex == destination) {
                // cout << "Debug: Reached Destination" << endl;
                break;
            }

            for (auto edge : getEdges(currentVertex)) {
                auto neighbor = edge->getDestination();
                int weight = edge->getWeight();
                int newDistance = currentDistance + weight;

                // cout << "Debug: Checking neighbor " << neighbor->getData() << " with edge weight " << weight << endl;

                if (newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                    parent[neighbor] = currentVertex;
                    pq.emplace(newDistance, neighbor);
                    // cout << "Debug: Updated distance for " << neighbor->getData() << " to " << newDistance << endl;
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
            // cout << "Debug: Path is empty or does not reach destination" << endl;
            return {};
        }

        for (auto vertex : path) {
            // cout << vertex->getData() << " ";
        }
        // cout << endl;

        return path;
    }

    return {};
}




















template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    if (!directed) {
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
        // cout << "Graph has a cycle, topological sort is not possible." << endl;
        return {};
    }
    return result;
}








template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    if (!isWeighted()) {
        // cout << "The graph must be weighted to compute a Minimum Spanning Tree." << endl;
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
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    // Vector to store strongly connected components
    vector<vector<shared_ptr<Vertex<T>>>> sccs;

    // Step 1: Perform DFS on the original graph to get the finishing order
    stack<shared_ptr<Vertex<T>>> finishOrder;
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;

    // Helper function to fill the finishing order stack
    auto dfsOriginal = [&visited, &finishOrder](shared_ptr<Vertex<T>> vertex, auto&& dfsOriginal) -> void {
        visited[vertex] = true;
        for (auto neighbor :  this->getAdjacentVertices(vertex)) {
            if (!visited[neighbor]) {
                dfsOriginal(neighbor, dfsOriginal);
            }
        }
        finishOrder.push(vertex);
    };

    for (auto vertex : vertices) {
        if (!visited[vertex]) {
            dfsOriginal(vertex, dfsOriginal);
        }
    }

    // Step 2: Transpose the graph (reverse the edges)
    Graph<T> transposedGraph;
    transposedGraph.setDirected(true);  // The transposed graph is directed

    // Add vertices and edges to the transposed graph
    for (auto vertex : vertices) {
        transposedGraph.addVertex(vertex->getData());
    }

    for (auto edge : edges) {
        transposedGraph.addEdge(edge->getDestination()->getData(), edge->getSource()->getData(), edge->getWeight());
    }

    // Step 3: Perform DFS on the transposed graph in the order of the finishing times
    visited.clear();
    while (!finishOrder.empty()) {
        shared_ptr<Vertex<T>> vertex = finishOrder.top();
        finishOrder.pop();

        if (!visited[vertex]) {
            vector<shared_ptr<Vertex<T>>> scc;
            // Perform DFS on the transposed graph and collect all the vertices in this SCC
            vector<shared_ptr<Vertex<T>>> dfsVisited;
            transposedGraph.DFSTraversalHelper(vertex, dfsVisited);

            // Add all visited vertices to the SCC
            for (auto& v : dfsVisited) {
                scc.push_back(v);
            }
            sccs.push_back(scc);  // Add the found SCC to the result
        }
    }

    return sccs;  // Return the strongly connected components
}



// ::::::::::::::::::::::::::::::: BONUS TASKS :::::::::::::::::::::::::::::::::::

// BONUS TASK 1 FOR 5 MARKS
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    // Vector to store the spanning trees
    vector<shared_ptr<Graph<T>>> spanningTrees;

    // Helper DFS function to explore the graph and build a spanning tree
    std::function<void(shared_ptr<Vertex<T>>, std::unordered_map<shared_ptr<Vertex<T>>, bool>&, shared_ptr<Graph<T>>)> 
    dfsSpanningTree = [&](shared_ptr<Vertex<T>> vertex, 
                           std::unordered_map<shared_ptr<Vertex<T>>, bool>& visited, 
                           shared_ptr<Graph<T>> tree) {
        visited[vertex] = true;
        tree->addVertex(vertex->getData());  // Add the current vertex to the tree
        for (auto neighbor : this->getAdjacentVertices(vertex)) {  // Use 'this' to access member function
            if (!visited[neighbor]) {
                tree->addEdge(vertex->getData(), neighbor->getData());  // Add edge to the tree
                dfsSpanningTree(neighbor, visited, tree);  // Recursive call
            }
        }
    };

    // If the graph has no vertices, return an empty vector
    if (vertices.empty()) {
        return spanningTrees;
    }

    // Initialize visited map
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;

    // Iterate over all vertices, performing DFS to find each connected component
    for (auto vertex : vertices) {
        if (!visited[vertex]) {
            // Create a new graph object to store the spanning tree for the component
            shared_ptr<Graph<T>> tree = make_shared<Graph<T>>();
            dfsSpanningTree(vertex, visited, tree);  // Start DFS from the unvisited vertex
            spanningTrees.push_back(tree);  // Add the tree to the result
        }
    }

    return spanningTrees;  // Return the vector of spanning trees
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
