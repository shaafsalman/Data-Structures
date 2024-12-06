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
        return {};
    }

    if (source == destination) {
        return {source};
    }

    if (vertices.empty() || edges.empty()) {
        return {};
    }

    if (weighted) {
        vector<int> distances(vertices.size(), INT_MAX);
        vector<shared_ptr<Vertex<T>>> parent(vertices.size(), nullptr);
        vector<bool> visited(vertices.size(), false);

        vector<shared_ptr<Vertex<T>>> vertexList;
        for (int i = 0; i < vertices.size(); ++i) {
            vertexList.push_back(vertices[i]);
        }

        int sourceIndex = -1;
        for (int i = 0; i < vertexList.size(); ++i) {
            if (vertexList[i] == source) {
                sourceIndex = i;
                break;
            }
        }

        distances[sourceIndex] = 0;

        queue<int> toVisit;
        toVisit.push(sourceIndex);

        while (!toVisit.empty()) {
            int currentIndex = toVisit.front();
            toVisit.pop();

            shared_ptr<Vertex<T>> currentVertex = vertexList[currentIndex];
            if (visited[currentIndex]) continue;

            visited[currentIndex] = true;

            vector<shared_ptr<Edge<T>>> edgesList = getEdges(currentVertex);
            for (int i = 0; i < edgesList.size(); ++i) {
                shared_ptr<Vertex<T>> neighbor = edgesList[i]->getDestination();
                int weight = edgesList[i]->getWeight();

                int neighborIndex = -1;
                for (int j = 0; j < vertexList.size(); ++j) {
                    if (vertexList[j] == neighbor) {
                        neighborIndex = j;
                        break;
                    }
                }

                int newDistance = distances[currentIndex] + weight;

                if (!visited[neighborIndex] && newDistance < distances[neighborIndex]) {
                    distances[neighborIndex] = newDistance;
                    parent[neighborIndex] = currentVertex;
                    toVisit.push(neighborIndex);
                }
            }
        }

        vector<shared_ptr<Vertex<T>>> path;
        int currentIndex = -1;
        for (int i = 0; i < vertexList.size(); ++i) {
            if (vertexList[i] == destination) {
                currentIndex = i;
                break;
            }
        }

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

        int pathSize = path.size();
        for (int i = 0; i < pathSize / 2; ++i) {
            swap(path[i], path[pathSize - i - 1]);
        }

        if (!path.empty() && path[path.size() - 1] == destination) {
            return path;
        }
    }

    return {}; 
}







template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::DFSTraversalReversed(shared_ptr<Vertex<T>> vertex) {
    
    vector<shared_ptr<Vertex<T>>> visitedVertices;
    if (!vertex) return visitedVertices;

    stack<shared_ptr<Vertex<T>>> toVisit;
    vector<shared_ptr<Vertex<T>>> visited;

    toVisit.push(vertex);

    while (!toVisit.empty()) {
        shared_ptr<Vertex<T>> current = toVisit.top();
        toVisit.pop();

        bool alreadyVisited = false;
        for (size_t i = 0; i < visited.size(); ++i) {
            if (visited[i] == current) {
                alreadyVisited = true;
                break;
            }
        }

        if (!alreadyVisited) {
            visited.push_back(current);
            visitedVertices.push_back(current);

            vector<shared_ptr<Vertex<T>>> adjacentVertices = getAdjacentVertices(current);
            for (size_t i = 0; i < adjacentVertices.size(); ++i) {
                toVisit.push(adjacentVertices[i]);
            }
        }
    }

    return visitedVertices;
}











template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    if (!directed) {
        return {};
    }

    vector<int> inDegree(vertices.size(), 0);
    vector<shared_ptr<Vertex<T>>> vertexList;
    
    for (int i = 0; i < vertices.size(); ++i) {
        vertexList.push_back(vertices[i]);
    }

    for (int i = 0; i < edges.size(); ++i) {
        shared_ptr<Vertex<T>> destination = edges[i]->getDestination();
        int destinationIndex = -1;

        for (int j = 0; j < vertexList.size(); ++j) {
            if (vertexList[j] == destination) {
                destinationIndex = j;
                break;
            }
        }

        if (destinationIndex != -1) {
            inDegree[destinationIndex]++;
        }
    }

    // Queue for V with in-degree 0
    queue<int> q;
    for (int i = 0; i < vertexList.size(); ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    vector<shared_ptr<Vertex<T>>> result;

    // topological sort
    while (!q.empty()) {
        int currentIndex = q.front();
        q.pop();
        
        result.push_back(vertexList[currentIndex]);

        // Get adjacent vertices and decrease their in-degree
        vector<shared_ptr<Vertex<T>>> adjVertices = getAdjacentVertices(vertexList[currentIndex]);
        for (int i = 0; i < adjVertices.size(); ++i) {
            int adjIndex = -1;

            for (int j = 0; j < vertexList.size(); ++j) {
                if (vertexList[j] == adjVertices[i]) {
                    adjIndex = j;
                    break;
                }
            }

            if (adjIndex != -1) {
                inDegree[adjIndex]--;
                if (inDegree[adjIndex] == 0) {
                    q.push(adjIndex);
                }
            }
        }
    }

    if (result.size() != vertices.size()) {
        return {}; 
    }

    return result;
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    if (!isWeighted()) {
        return nullptr;
    }

    auto mst = make_shared<Graph<T>>(false, true); 

    vector<shared_ptr<Edge<T>>> edgeList = getAllEdges();
    vector<pair<int, pair<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>>>> edges; 
    
    for (int i = 0; i < edgeList.size(); ++i) {
        edges.push_back({edgeList[i]->getWeight(), {edgeList[i]->getSource(), edgeList[i]->getDestination()}});
    }

    sort(edges.begin(), edges.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    // Union-Find
    int n = vertices.size();
    vector<int> parent(n), rank(n, 0);

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    // Find the parent 
    function<int(int)> findParent = [&](int vertex) -> int {
        if (parent[vertex] != vertex) {
            parent[vertex] = findParent(parent[vertex]); 
        }
        return parent[vertex];
    };

    // Union by rank
    auto unionVertices = [&](int u, int v) {
        int rootU = findParent(u);
        int rootV = findParent(v);

        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    };

    // Add V to the MST
    for (int i = 0; i < n; ++i) {
        mst->addVertex(vertices[i]->getData());
    }

    for (auto &[weight, edge] : edges) {
        shared_ptr<Vertex<T>> u = edge.first;
        shared_ptr<Vertex<T>> v = edge.second;

        int uIndex = -1, vIndex = -1;
        for (int i = 0; i < vertices.size(); ++i) {
            if (vertices[i] == u) uIndex = i;
            if (vertices[i] == v) vIndex = i;
        }

        if (findParent(uIndex) != findParent(vIndex)) {
            mst->addEdge(u->getData(), v->getData(), weight);
            unionVertices(uIndex, vIndex);
        }
    }

    return mst;
}






template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> scc;
    
    int n = vertices.size();
    vector<bool> visited(n, false);  // Array for visited vertices
    
    // First DFS to fill the finish order
    vector<shared_ptr<Vertex<T>>> finishOrder;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            // Perform DFS traversal and fill the finishOrder
            vector<shared_ptr<Vertex<T>>> traversal = DFSTraversal(vertices[i]);
            for (auto vertex : traversal) {
                finishOrder.push_back(vertex);
            }
        }
    }

    // Second DFS on the reversed graph to find SCC
    vector<bool> visitedReversed(n, false);
    while (!finishOrder.empty()) {
        auto vertex = finishOrder.back();
        finishOrder.pop_back();

        int idx = -1;
        for (int i = 0; i < vertices.size(); ++i) {
            if (vertices[i] == vertex) {
                idx = i;
                break;
            }
        }

        if (!visitedReversed[idx]) {
            // Perform DFS on reversed graph
            vector<shared_ptr<Vertex<T>>> component;
            vector<shared_ptr<Vertex<T>>> traversalReversed = DFSTraversalReversed(vertex);
            for (auto vertex : traversalReversed) {
                component.push_back(vertex);
            }
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












