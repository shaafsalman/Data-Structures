#include "../Task 2/Graph_Task_2.cpp"
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    vector<shared_ptr<Vertex<T>>> path;

    if (source == nullptr || destination == nullptr || vertices.empty()) {
        return path;  // Return empty path if source, destination, or graph is empty.
    }

    if (source == destination) {
        path.push_back(source);
        return path;
    }

    if (isUnweighted()) {
        // BFS for unweighted graphs
        unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;
        unordered_set<shared_ptr<Vertex<T>>> visited;
        queue<shared_ptr<Vertex<T>>> q;
        
        q.push(source);
        visited.insert(source);
        
        while (!q.empty()) {
            auto vertex = q.front();
            q.pop();
            
            if (vertex == destination) {
                // Reconstruct the path
                while (vertex != nullptr) {
                    path.push_back(vertex);
                    vertex = parent[vertex];
                }
                reverse(path.begin(), path.end());
                return path;
            }

            for (auto neighbor : vertex->neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = vertex;
                    q.push(neighbor);
                }
            }
        }
    } else if (isWeighted()) {
        // Dijkstra's algorithm for weighted graphs
        unordered_map<shared_ptr<Vertex<T>>, int> distances;
        unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;
        unordered_set<shared_ptr<Vertex<T>>> visited;
        auto comp = [&](shared_ptr<Vertex<T>> a, shared_ptr<Vertex<T>> b) {
            return distances[a] > distances[b];
        };
        priority_queue<shared_ptr<Vertex<T>>, vector<shared_ptr<Vertex<T>>>, decltype(comp)> pq(comp);

        for (auto v : vertices) {
            distances[v] = INT_MAX;
        }
        distances[source] = 0;
        pq.push(source);

        while (!pq.empty()) {
            auto vertex = pq.top();
            pq.pop();

            if (visited.find(vertex) != visited.end()) continue;
            visited.insert(vertex);

            for (auto edge : vertex->edges) {
                auto neighbor = edge->getOtherVertex(vertex);
                int newDist = distances[vertex] + edge->getWeight();
                
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    parent[neighbor] = vertex;
                    pq.push(neighbor);
                }
            }
        }

        // Reconstruct the path
        auto vertex = destination;
        while (vertex != nullptr) {
            path.push_back(vertex);
            vertex = parent[vertex];
        }
        reverse(path.begin(), path.end());
    }

    return path;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    vector<shared_ptr<Vertex<T>>> sortedVertices;
    
    if (vertices.empty()) {
        return sortedVertices;  // Return an empty vector if the graph has no vertices.
    }

    if (hasCycles()) {
        return sortedVertices;  // Return empty if the graph has cycles.
    }

    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    stack<shared_ptr<Vertex<T>>> stack;
    
    for (auto vertex : vertices) {
        if (!visited[vertex]) {
            topologicalSortHelper(vertex, visited, stack);
        }
    }

    while (!stack.empty()) {
        sortedVertices.push_back(stack.top());
        stack.pop();
    }

    return sortedVertices;
}

template <class T>
void Graph<T>::topologicalSortHelper(shared_ptr<Vertex<T>> vertex, unordered_map<shared_ptr<Vertex<T>>, bool>& visited, stack<shared_ptr<Vertex<T>>>& stack) {
    visited[vertex] = true;

    for (auto neighbor : vertex->neighbors) {
        if (!visited[neighbor]) {
            topologicalSortHelper(neighbor, visited, stack);
        }
    }

    stack.push(vertex);
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    if (vertices.empty()) {
        return nullptr;  // Return nullptr if the graph has no vertices.
    }

    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>();
    unordered_set<shared_ptr<Vertex<T>>> inMST;
    auto comp = [](shared_ptr<Edge<T>> a, shared_ptr<Edge<T>> b) {
        return a->getWeight() > b->getWeight();
    };
    priority_queue<shared_ptr<Edge<T>>, vector<shared_ptr<Edge<T>>>, decltype(comp)> pq(comp);
    
    // Starting with an arbitrary vertex
    auto startVertex = *vertices.begin();
    inMST.insert(startVertex);
    
    for (auto edge : startVertex->edges) {
        pq.push(edge);
    }

    while (!pq.empty()) {
        auto edge = pq.top();
        pq.pop();
        
        auto neighbor = edge->getOtherVertex(startVertex);
        if (inMST.find(neighbor) == inMST.end()) {
            mst->addEdge(edge);
            inMST.insert(neighbor);
            
            for (auto e : neighbor->edges) {
                if (inMST.find(e->getOtherVertex(neighbor)) == inMST.end()) {
                    pq.push(e);
                }
            }
        }
    }

    return mst;
}

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> sccs;
    
    if (vertices.empty()) {
        return sccs;  // Return an empty vector if the graph has no vertices.
    }

    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    stack<shared_ptr<Vertex<T>>> finishStack;

    // First DFS to order vertices by finish time
    for (auto vertex : vertices) {
        if (!visited[vertex]) {
            dfsForSCC(vertex, visited, finishStack);
        }
    }

    // Reverse the graph
    shared_ptr<Graph<T>> reversedGraph = reverseGraph();

    // Second DFS on reversed graph
    visited.clear();
    while (!finishStack.empty()) {
        auto vertex = finishStack.top();
        finishStack.pop();

        if (!visited[vertex]) {
            vector<shared_ptr<Vertex<T>>> scc;
            reversedGraph->dfsForSCC(vertex, visited, scc);
            sccs.push_back(scc);
        }
    }

    return sccs;
}

template <class T>
void Graph<T>::dfsForSCC(shared_ptr<Vertex<T>> vertex, unordered_map<shared_ptr<Vertex<T>>, bool>& visited, stack<shared_ptr<Vertex<T>>>& finishStack) {
    visited[vertex] = true;

    for (auto neighbor : vertex->neighbors) {
        if (!visited[neighbor]) {
            dfsForSCC(neighbor, visited, finishStack);
        }
    }

    finishStack.push(vertex);
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::reverseGraph() {
    shared_ptr<Graph<T>> reversedGraph = make_shared<Graph<T>>();

    for (auto vertex : vertices) {
        for (auto neighbor : vertex->neighbors) {
            reversedGraph->addEdge(make_shared<Edge<T>>(neighbor, vertex));
        }
    }

    return reversedGraph;
}

// BONUS TASK 1: Find all the spanning trees of the graph.
template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    vector<shared_ptr<Graph<T>>> spanningTrees;
    
    if (vertices.empty()) {
        return spanningTrees;  // Return empty if the graph has no vertices.
    }

    unordered_set<shared_ptr<Vertex<T>>> visited;
    
    // For each unvisited vertex, start a DFS/BFS to find the connected component and add it as a spanning tree.
    for (auto vertex : vertices) {
        if (visited.find(vertex) == visited.end()) {
            shared_ptr<Graph<T>> spanningTree = make_shared<Graph<T>>();
            dfsForSpanningTree(vertex, visited, spanningTree);
            spanningTrees.push_back(spanningTree);
        }
    }

    return spanningTrees;
}

template <class T>
void Graph<T>::dfsForSpanningTree(shared_ptr<Vertex<T>> vertex, unordered_set<shared_ptr<Vertex<T>>>& visited, shared_ptr<Graph<T>>& spanningTree) {
    visited.insert(vertex);
    spanningTree->addVertex(vertex);

    for (auto neighbor : vertex->neighbors) {
        if (visited.find(neighbor) == visited.end()) {
            spanningTree->addEdge(make_shared<Edge<T>>(vertex, neighbor));
            dfsForSpanningTree(neighbor, visited, spanningTree);
        }
    }
}

// BONUS TASK 2: Find all the connected components of the graph.
template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> components;
    
    if (vertices.empty()) {
        return components;  // Return empty if the graph has no vertices.
    }

    unordered_set<shared_ptr<Vertex<T>>> visited;

    // Use DFS/BFS to find connected components.
    for (auto vertex : vertices) {
        if (visited.find(vertex) == visited.end()) {
            vector<shared_ptr<Vertex<T>>> component;
            dfsForConnectedComponent(vertex, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

template <class T>
void Graph<T>::dfsForConnectedComponent(shared_ptr<Vertex<T>> vertex, unordered_set<shared_ptr<Vertex<T>>>& visited, vector<shared_ptr<Vertex<T>>>& component) {
    visited.insert(vertex);
    component.push_back(vertex);

    for (auto neighbor : vertex->neighbors) {
        if (visited.find(neighbor) == visited.end()) {
            dfsForConnectedComponent(neighbor, visited, component);
        }
    }
}
