#include "../Task 2/Graph_Task_2.cpp"
#include <queue>
#include <stack>
#include <unordered_map>

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    vector<shared_ptr<Vertex<T>>> path;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> parent;
    unordered_map<shared_ptr<Vertex<T>>, int> distance;
    
    if (weighted) {
        // Dijkstra's Algorithm for weighted graph
        priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq;
        
        for (auto& vertex : vertices) {
            distance[vertex] = INT_MAX;
        }
        distance[source] = 0;
        pq.push({0, source});
        
        while (!pq.empty()) {
            auto current = pq.top();
            pq.pop();
            auto u = current.second;
            
            if (u == destination) break;
            
            for (auto& edge : getEdges(u)) {
                auto v = edge->getDestination();
                int weight = edge->getWeight();
                
                if (distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    pq.push({distance[v], v});
                    parent[v] = u;
                }
            }
        }
        
        // Reconstruct path from destination to source
        shared_ptr<Vertex<T>> curr = destination;
        while (curr != nullptr) {
            path.push_back(curr);
            curr = parent[curr];
        }
        
        reverse(path.begin(), path.end());
    } else {
        // BFS for unweighted graph
        unordered_map<shared_ptr<Vertex<T>>, bool> visited;
        queue<shared_ptr<Vertex<T>>> q;
        
        q.push(source);
        visited[source] = true;
        parent[source] = nullptr;
        
        while (!q.empty()) {
            auto u = q.front();
            q.pop();
            
            if (u == destination) break;
            
            for (auto& adj : getAdjacentVertices(u)) {
                if (!visited[adj]) {
                    visited[adj] = true;
                    parent[adj] = u;
                    q.push(adj);
                }
            }
        }
        
        // Reconstruct path from destination to source
        shared_ptr<Vertex<T>> curr = destination;
        while (curr != nullptr) {
            path.push_back(curr);
            curr = parent[curr];
        }
        
        reverse(path.begin(), path.end());
    }
    
    return path;
}

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    vector<shared_ptr<Vertex<T>>> order;
    unordered_map<shared_ptr<Vertex<T>>, int> inDegree;
    queue<shared_ptr<Vertex<T>>> q;
    
    for (auto& vertex : vertices) {
        inDegree[vertex] = 0;
    }
    
    for (auto& edge : edges) {
        inDegree[edge->getDestination()]++;
    }
    
    for (auto& vertex : vertices) {
        if (inDegree[vertex] == 0) {
            q.push(vertex);
        }
    }
    
    while (!q.empty()) {
        auto u = q.front();
        q.pop();
        order.push_back(u);
        
        for (auto& adj : getAdjacentVertices(u)) {
            if (--inDegree[adj] == 0) {
                q.push(adj);
            }
        }
    }
    
    return order;
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    shared_ptr<Graph<T>> mst = make_shared<Graph<T>>(directed, weighted);
    
    if (!weighted) return mst; // Minimum spanning tree is only for weighted graphs
    
    unordered_map<shared_ptr<Vertex<T>>, bool> inMST;
    priority_queue<pair<int, shared_ptr<Edge<T>>>, vector<pair<int, shared_ptr<Edge<T>>>>, greater<pair<int, shared_ptr<Edge<T>>>>> pq;
    
    auto startVertex = vertices[0]; // You can choose any vertex to start
    
    inMST[startVertex] = true;
    
    // Add all edges from startVertex to the priority queue
    for (auto& edge : getEdges(startVertex)) {
        pq.push({edge->getWeight(), edge});
    }
    
    while (!pq.empty()) {
        auto edge = pq.top();
        pq.pop();
        
        auto v = edge.second->getDestination();
        
        if (!inMST[v]) {
            inMST[v] = true;
            mst->addEdge(edge.second->getSource()->getData(), v->getData(), edge.second->getWeight());
            
            // Add all edges from vertex v to the queue
            for (auto& adjEdge : getEdges(v)) {
                if (!inMST[adjEdge->getDestination()]) {
                    pq.push({adjEdge->getWeight(), adjEdge});
                }
            }
        }
    }
    
    return mst;
}

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> scc;
    stack<shared_ptr<Vertex<T>>> st;
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    
    // Step 1: Fill vertices in stack according to their finishing times (DFS)
    for (auto& vertex : vertices) {
        if (!visited[vertex]) {
            DFSTraversalHelper(vertex, visited, st);
        }
    }
    
    // Step 2: Transpose the graph (reverse all edges)
    shared_ptr<Graph<T>> transposedGraph = transposeGraph();
    
    // Step 3: DFS on transposed graph in order of stack
    fill(visited.begin(), visited.end(), false);
    
    while (!st.empty()) {
        auto vertex = st.top();
        st.pop();
        
        if (!visited[vertex]) {
            vector<shared_ptr<Vertex<T>>> component;
            DFSHelperOnTransposedGraph(vertex, visited, component, transposedGraph);
            scc.push_back(component);
        }
    }
    
    return scc;
}

template <class T>
vector<shared_ptr<Graph<T>>> Graph<T>::SpanningTrees() {
    vector<shared_ptr<Graph<T>>> spanningTrees;
    
    // Find all spanning trees using DFS or BFS from each vertex
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    
    for (auto& vertex : vertices) {
        if (!visited[vertex]) {
            shared_ptr<Graph<T>> tree = make_shared<Graph<T>>(directed, weighted);
            DFSForSpanningTree(vertex, visited, tree);
            spanningTrees.push_back(tree);
        }
    }
    
    return spanningTrees;
}

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::connectedComponents() {
    vector<vector<shared_ptr<Vertex<T>>>> connectedComps;
    unordered_map<shared_ptr<Vertex<T>>, bool> visited;
    
    for (auto& vertex : vertices) {
        if (!visited[vertex]) {
            vector<shared_ptr<Vertex<T>>> component;
            DFSForConnectedComponents(vertex, visited, component);
            connectedComps.push_back(component);
        }
    }
    
    return connectedComps;
}

// Helper Functions for DFS, Transpose, etc.

template <class T>
void Graph<T>::DFSTraversalHelper(shared_ptr<Vertex<T>> vertex, unordered_map<shared_ptr<Vertex<T>>, bool>& visited, stack<shared_ptr<Vertex<T>>>& st) {
    visited[vertex] = true;
    
    for (auto& adj : getAdjacentVertices(vertex)) {
        if (!visited[adj]) {
            DFSTraversalHelper(adj, visited, st);
        }
    }
    
    st.push(vertex);
}

template <class T>
void Graph<T>::DFSHelperOnTransposedGraph(shared_ptr<Vertex<T>> vertex, unordered_map<shared_ptr<Vertex<T>>, bool>& visited, vector<shared_ptr<Vertex<T>>>& component, shared_ptr<Graph<T>> transposedGraph) {
    visited[vertex] = true;
    component.push_back(vertex);
    
    for (auto& adj : transposedGraph->getAdjacentVertices(vertex)) {
        if (!visited[adj]) {
            DFSHelperOnTransposedGraph(adj, visited, component, transposedGraph);
        }
    }
}

template <class T>
shared_ptr<Graph<T>> Graph<T>::transposeGraph() {
    shared_ptr<Graph<T>> transposed = make_shared<Graph<T>>(directed, weighted);
    
    for (auto& edge : edges) {
        transposed->addEdge(edge->getDestination()->getData(), edge->getSource()->getData(), edge->getWeight());
    }
    
    return transposed;
}

template <class T>
void Graph<T>::DFSForSpanningTree(shared_ptr<Vertex<T>> vertex, unordered_map<shared_ptr<Vertex<T>>, bool>& visited, shared_ptr<Graph<T>>& tree) {
    visited[vertex] = true;
    
    for (auto& adj : getAdjacentVertices(vertex)) {
        if (!visited[adj]) {
            tree->addEdge(vertex->getData(), adj->getData(), 0);  // Assuming unweighted edges
            DFSForSpanningTree(adj, visited, tree);
        }
    }
}

template <class T>
void Graph<T>::DFSForConnectedComponents(shared_ptr<Vertex<T>> vertex, unordered_map<shared_ptr<Vertex<T>>, bool>& visited, vector<shared_ptr<Vertex<T>>>& component) {
    visited[vertex] = true;
    component.push_back(vertex);
    
    for (auto& adj : getAdjacentVertices(vertex)) {
        if (!visited[adj]) {
            DFSForConnectedComponents(adj, visited, component);
        }
    }
}
