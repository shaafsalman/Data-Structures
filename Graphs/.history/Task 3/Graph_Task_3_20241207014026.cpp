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
    // Return empty path if source or destination is null
    if (!source || !destination) {
        return {};
    }

    // Return source if source equals destination
    if (source == destination) {
        return {source};
    }

    // Check if graph is empty
    if (vertices.empty() || edges.empty()) {
        return {};
    }

    if (weighted) {
        // Create a vector for storing the distances from the source vertex, initialized to a large number (infinity)
        vector<int> distances(vertices.size(), INT_MAX);
        vector<shared_ptr<Vertex<T>>> parent(vertices.size(), nullptr);
        vector<bool> visited(vertices.size(), false);

        // Create a vector of vertices to map each vertex to an index
        vector<shared_ptr<Vertex<T>>> vertexList;
        for (int i = 0; i < vertices.size(); ++i) {
            vertexList.push_back(vertices[i]);
        }

        // Find the index of the source vertex
        int sourceIndex = -1;
        for (int i = 0; i < vertexList.size(); ++i) {
            if (vertexList[i] == source) {
                sourceIndex = i;
                break;
            }
        }

        // Set the distance to the source as 0 and add it to the queue
        distances[sourceIndex] = 0;

        // Use a queue to simulate the processing of vertices in order of distance
        queue<int> toVisit;
        toVisit.push(sourceIndex);

        while (!toVisit.empty()) {
            int currentIndex = toVisit.front();
            toVisit.pop();

            shared_ptr<Vertex<T>> currentVertex = vertexList[currentIndex];
            if (visited[currentIndex]) continue;

            visited[currentIndex] = true;

            // Process all neighbors of the current vertex
            vector<shared_ptr<Edge<T>>> edgesList = getEdges(currentVertex);
            for (int i = 0; i < edgesList.size(); ++i) {
                shared_ptr<Vertex<T>> neighbor = edgesList[i]->getDestination();
                int weight = edgesList[i]->getWeight();

                // Find the index of the neighbor vertex
                int neighborIndex = -1;
                for (int j = 0; j < vertexList.size(); ++j) {
                    if (vertexList[j] == neighbor) {
                        neighborIndex = j;
                        break;
                    }
                }

                int newDistance = distances[currentIndex] + weight;

                // If a shorter path to the neighbor is found, update its distance and parent
                if (!visited[neighborIndex] && newDistance < distances[neighborIndex]) {
                    distances[neighborIndex] = newDistance;
                    parent[neighborIndex] = currentVertex;
                    toVisit.push(neighborIndex);
                }
            }
        }

        // Reconstruct the shortest path from destination to source using the parent array
        vector<shared_ptr<Vertex<T>>> path;
        int currentIndex = -1;
        for (int i = 0; i < vertexList.size(); ++i) {
            if (vertexList[i] == destination) {
                currentIndex = i;
                break;
            }
        }

        // Add vertices to the path starting from destination and moving towards the source
        while (currentIndex != -1) {
            path.push_back(vertexList[currentIndex]);
            int prevIndex = -1;
            for (int i = 0; i < vertexList.size(); ++i) {
                if (vertexList[i] == parent[currentIndex]) {
                    prevIndex = i;
                    break;
                }
            }
            currentIndex = prevIndex;
            if (currentIndex == sourceIndex) break;
        }

        // Reverse the path to get it from source to destination
        int pathSize = path.size();
        for (int i = 0; i < pathSize / 2; ++i) {
            swap(path[i], path[pathSize - i - 1]);
        }

        // Return the path if it's valid and reaches the destination
        if (!path.empty() && path[path.size() - 1] == destination) {
            return path;
        }
    }

    return {};  // Return empty path if no valid path is found
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
    vector<vector<shared_ptr<Vertex<T>>>> scc;
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    stack<shared_ptr<Vertex<T>>> finishOrder;

    // First DFS to fill the finish order
    for (auto vertex : vertices) {  // Ensure 'vertices' is a class member
        if (!visited[vertex]) {
            auto dfsOriginal = [&visited, &finishOrder](shared_ptr<Vertex<T>> vertex, auto&& dfsOriginal, Graph<T>& graph) -> void {
                visited[vertex] = true;
                for (auto neighbor : graph.getAdjacentVertices(vertex)) {
                    if (!visited[neighbor]) {
                        dfsOriginal(neighbor, dfsOriginal, graph);
                    }
                }
                finishOrder.push(vertex);
            };
            dfsOriginal(vertex, dfsOriginal, *this);  // Correct parameter passing
        }
    }

    // Second DFS on the reversed graph to find SCC
    unordered_map<shared_ptr<Vertex<T>>, bool> visitedReversed;
    while (!finishOrder.empty()) {
        shared_ptr<Vertex<T>> vertex = finishOrder.top();
        finishOrder.pop();

        if (!visitedReversed[vertex]) {
            vector<shared_ptr<Vertex<T>>> component;
            auto dfsReversed = [&visitedReversed, &component](shared_ptr<Vertex<T>> vertex, auto&& dfsReversed, Graph<T>& graph) -> void {
                visitedReversed[vertex] = true;
                component.push_back(vertex);
                for (auto neighbor : graph.getInAdjacentVertices(vertex)) {
                    if (!visitedReversed[neighbor]) {
                        dfsReversed(neighbor, dfsReversed, graph);
                    }
                }
            };
            dfsReversed(vertex, dfsReversed, *this);  // Pass parameters correctly
            scc.push_back(component);
        }
    }

    return scc;
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
















