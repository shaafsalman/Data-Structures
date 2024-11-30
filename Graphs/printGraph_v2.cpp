// Copy the function to your graph class
// This is the same as the printGraph() function but here it can print data that is wrapped in a shared_ptr
// You'll need this for TASK 4 GraphViz visualisations



template <class T>
void Graph<T>::printGraph_v2(string fileName) {
    // Print the graph in a Graphviz DOT format with arrows for directed graphs and lines for undirected graphs
    // https://graphviz.org/download/source/ -> Graphviz Download link for Macbook Users (Download latest stable version)
    // https://graphviz.org/download/ -> Graphviz Download link for Windows Users (Download latest stable version)
    // Follow the commands to install Graphviz on your system to visualise the graph
    // printGraph() function will create a file named graph.dot in the current directory
    // dot -Tsvg graph.dot -o graph.svg -> This would convert your dot file to a svg image

    // Solution:
    ofstream dotFile(fileName);
    dotFile << (directed ? "digraph" : "graph") << " G {" << endl;
    dotFile << "    graph [pad=\"0.5\", nodesep=\"0.8\", ranksep=\"1.2\", margin=0.5];" << endl;
    dotFile << "    node [shape=ellipse, style=filled, color=lightblue, fontname=\"Helvetica\", fontsize=16, width=0.5, height=0.5];" << endl;
    dotFile << "    edge [color=dimgray, penwidth=2.0, arrowhead=open];" << endl;
    dotFile << "    rankdir=LR;" << endl;
    dotFile << "    label=\"Graph\";" << endl;
    dotFile << "    labelloc=top;" << endl;


    for (auto edge : edges) {
        dotFile << "    " << *(edge->getSource()->getData()) << (directed ? " -> " : " -- ") << *(edge->getDestination()->getData());
        if (weighted) {
            dotFile << " [label=\"" << edge->getWeight() << "\"]";
        }
        dotFile << ";" << endl;
    }
    dotFile << "}" << endl;
    dotFile.close();
    cout << "Graph exported to graph.dot (Graphviz format)" << endl;
}