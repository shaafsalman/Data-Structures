#include "../Task 2/Graph_Task_2.cpp"

template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::shortestPath(shared_ptr<Vertex<T>> source, shared_ptr<Vertex<T>> destination) {
    // Find the shortest path between the source and destination vertices
    // Return the vertices in the shortest path
    // source: source vertex for the shortest path
    // destination: destination vertex for the shortest path

    // Make sure to use the correct algorithm based on the type of graph (directed/undirected, weighted/unweighted)

    // Solution:z

     vector<shared_ptr<Vertex<T>>> path;
    unordered_map<shared_ptr<Vertex<T>>, shared_ptr<Vertex<T>>> previous;
    unordered_map<shared_ptr<Vertex<T>>, int> distance;
    priority_queue<pair<int, shared_ptr<Vertex<T>>>, vector<pair<int, shared_ptr<Vertex<T>>>>, greater<pair<int, shared_ptr<Vertex<T>>>>> pq;

    // Initialize distances to infinity and source distance to 0
    for (auto& vertex : vertices) {
        distance[vertex] = INT_MAX;
    }
    distance[source] = 0;

    pq.push({0, source});

    while (!pq.empty()) {
        auto current = pq.top().second;
        pq.pop();

        if (current == destination) {
            break;  // Stop when we reach the destination
        }

        for (auto& neighbor : getAdjacentVertices(current)) {
            int alt = distance[current] + (weighted ? getEdge(current->getData(), neighbor->getData())->getWeight() : 1);
            if (alt < distance[neighbor]) {
                distance[neighbor] = alt;
                previous[neighbor] = current;
                pq.push({alt, neighbor});
            }
        }
    }

    // Reconstruct the path from destination to source
    for (auto at = destination; at != nullptr; at = previous[at]) {
        path.push_back(at);
    }

    reverse(path.begin(), path.end());
    return path;
    
}




template <class T>
vector<shared_ptr<Vertex<T>>> Graph<T>::topologicalSort() {
    // Perform Topological Sort on the graph
    // Return the vertices in the topological order

    // Solution:
}










template <class T>
shared_ptr<Graph<T>> Graph<T>::minimumSpanningTree() {
    // Find the minimum spanning tree of the graph
    // Return the minimum spanning tree as a Graph object

    // Solution:
};

template <class T>
vector<vector<shared_ptr<Vertex<T>>>> Graph<T>::stronglyConnectedComponents() {
    // Find all the strongly connected components of the graph
    // Return the strongly connected components as a vector of vectors of vertices

    // Solution:
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
    // Return the connected components as a vector of vectors of vertices

    // Solution:
}