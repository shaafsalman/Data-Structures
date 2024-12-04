#include "SocialNetwork.h"

Human::Human() {
    // Constructor for the Human class
    // Set the name and age of the human
    // name
    // age
    // Friends: create the default graph to store the friends of the human

    // Solution:
    this->UniqueID = uniqueID_counter++;
    this->name = "Unknown";
    this->age = 0;
}

Human::Human(string name, int age) {
    // Constructor for the Human class
    // Set the name and age of the human
    // name: name of the human
    // age: age of the human
    // Friends: create the default graph to store the friends of the human
    // UniqueID: assign a unique ID to the human

    // Solution:
     this->UniqueID = uniqueID_counter++;
    this->name = name;
    this->age = age;
}  

Human::Human (const Human& human) {
    // Copy Constructor for the Human class
    // Copy the name, age, Friends and UniqueID of the human passed as argument

    // Solution:
      this->UniqueID = human.UniqueID;
    this->name = human.name;
    this->age = human.age;
}

int Human::getUniqueID() {
    // return the UniqueID of the Human

    // Solution:
        return this->UniqueID;
}


string Human::getName() {
    // return Name of the Human

    // Solution:
        return this->name;

}

int Human::getAge() {
    // return age of the Human

    // Solution:
            return this->age;

}

void Human::addFriend(shared_ptr<Human> Friend) {
    // Add a friend (pointer to Human Object)

    // Solution:
     Friends.addVertex(Friend);

}

void Human::removeFriend(shared_ptr<Human> Friend) {
    // search and remove the friend from the Graph

    // Solution:
        Friends.removeVertex(Friend);

}

vector<shared_ptr<Human>> Human::getFriends() {
    // return the friends of the human

    // Solution:
     vector<shared_ptr<Human>> friendList;
    for (auto vertex : Friends.getAllVertices()) {
        friendList.push_back(vertex->getData());
    }
    return friendList;
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

SocialNetwork::SocialNetwork() {
    // Construct the Social Network
    // Initialize the Graph and the number of humans

    // Solution:
        NumberOfHumans = 0;

}

void SocialNetwork::addHuman(shared_ptr<Human> human) {
    // Add the humans and their networks
    
    // Solution:
    cout << "Adding human: " << *human << endl;
    Network.addVertex(human);
    NumberOfHumans++;
}

void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    // Remove any and all connections to this human. Make sure not to affect other people's networks

    // Solution:
       Network.removeVertex(human);
    NumberOfHumans--;
}

void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // create a friendship between these two humans and add it to the graph.

    // Solution:
        // cout << "Adding friendship: " << *human1 << " <-> " << *human2 << endl;
    Network.addEdge(human1, human2, 1); 
    
    Network.addEdge(human1, human2); 
    Network.addEdge(human2, human1);
    human1->addFriend(human2);
    human2->addFriend(human1);
}

void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // remove that single friendship but do not do anything else

    // Solution:
       Network.removeEdge(human1, human2);
    human1->removeFriend(human2);
    human2->removeFriend(human1);
}

vector<shared_ptr<Human>> SocialNetwork::getFriends(shared_ptr<Human> human) {
    // return the friends of the human passed as argument

    // Solution:
        return human->getFriends();

}

vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // return the mutual friends (a central connection between Human 1 and Human 2) of the people passed (Not directly connected to each other)

    // Solution:
      vector<shared_ptr<Human>> mutualFriends;
    vector<shared_ptr<Human>> friends1 = human1->getFriends();
    vector<shared_ptr<Human>> friends2 = human2->getFriends();

    // Compare the friends of both humans
    for (auto f1 : friends1) {
        for (auto f2 : friends2) {
            if (f1 == f2) {
                mutualFriends.push_back(f1);
            }
        }
    }
    return mutualFriends;
}

vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    // Returns a vector of groups where each group consists of humans connected either directly or indirectly.

    vector<vector<shared_ptr<Human>>> groups;  // To store the groups

    // Edge case: If the network is empty, return an empty vector
    if (NumberOfHumans == 0) {
        return groups;
    }

    // Create a graph of humans and their friendships
    Graph<shared_ptr<Human>> graph;

    // Add vertices (Humans) to the graph
    for (auto& human : Network.getAllVertices()) {
        graph.addVertex(human->getData());
    }

    // Add edges (friendships) to the graph
    for (auto& human : Network.getAllVertices()) {
        for (auto& friendPtr : human->getData()->getFriends()) {
            graph.addEdge(human->getData(), friendPtr, 1);  // Assuming weight 1 for friendships
        }
    }

    // Get the connected components (groups) from the graph
    vector<vector<shared_ptr<Vertex<shared_ptr<Human>>>>> connectedComponents = graph.connectedComponents();

    // For each connected component, create a group (vector of shared_ptr<Human>)
    for (auto& component : connectedComponents) {
        vector<shared_ptr<Human>> group;
        for (auto& vertex : component) {
            group.push_back(vertex->getData());  // Add each human to the group
        }
        groups.push_back(group);  // Add the group to the list of groups
    }

    return groups;  // Return the list of groups
}




bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Edge case: if either human is null, return false
    if (!human1 || !human2) {
        return false;
    }

    // Edge case: if both humans are the same, they are trivially connected
    if (human1 == human2) {
        return true;
    }

    // Get the vertices corresponding to human1 and human2
    auto vertex1 = Network.getVertex(human1);
    auto vertex2 = Network.getVertex(human2);

    // Edge case: if either human is not found in the network, return false
    if (!vertex1 || !vertex2) {
        return false;
    }

    // BFS to check connectivity between the two humans
    queue<shared_ptr<Vertex<shared_ptr<Human>>>> toVisit;
    unordered_set<shared_ptr<Vertex<shared_ptr<Human>>>> visited;

    // Start BFS from human1's vertex
    toVisit.push(vertex1);
    visited.insert(vertex1);

    while (!toVisit.empty()) {
        auto currentVertex = toVisit.front();
        toVisit.pop();

        // If we've reached human2, return true (path exists)
        if (currentVertex == vertex2) {
            return true;
        }

        // Visit all neighbors (friends) of the current human
        for (auto& edge : Network.getEdges(currentVertex)) {
            auto neighbor = edge->getDestination();
            if (visited.find(neighbor) == visited.end()) {  // If the neighbor hasn't been visited
                toVisit.push(neighbor);
                visited.insert(neighbor);  // Mark the neighbor as visited
            }
        }
    }

    // If we exit the loop without finding human2, return false (no path)
    return false;
}








vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Return the connection chain between these two humans if it exists, else return an empty vector.
    
    // Edge Case 1: If either human is null, return an empty vector
    if (!human1 || !human2) {
        return {};
    }

    // Edge Case 2: If both humans are the same, return a vector containing only human1
    if (human1 == human2) {
        return {human1};
    }

    // Retrieve the vertices for the humans from the network
    auto vertex1 = Network.getVertex(human1);
    auto vertex2 = Network.getVertex(human2);

    // Edge Case 3: If either human is not found in the network, return an empty vector
    if (!vertex1 || !vertex2) {
        return {};
    }

    // BFS to find the shortest path between human1 and human2
    vector<shared_ptr<Human>> connectionChain;  // To store the connection chain
    unordered_map<shared_ptr<Vertex<shared_ptr<Human>>>, shared_ptr<Vertex<shared_ptr<Human>>>> parent;

    // Initialize BFS with the first human
    queue<shared_ptr<Vertex<shared_ptr<Human>>>> toVisit;
    toVisit.push(vertex1);
    parent[vertex1] = nullptr;  // No parent for the starting human

    // Perform BFS to find the path from human1 to human2
    while (!toVisit.empty()) {
        auto currentVertex = toVisit.front();
        toVisit.pop();

        // If we reached human2, construct the connection chain
        if (currentVertex == vertex2) {
            // Backtrack from human2 to human1 to build the path
            shared_ptr<Vertex<shared_ptr<Human>>> vertex = vertex2;
            while (vertex != nullptr) {
                connectionChain.push_back(vertex->getData());  // Add human to the chain
                vertex = parent[vertex];  // Move to the parent (previous human in the path)
            }
            reverse(connectionChain.begin(), connectionChain.end());  // Reverse the chain to get the correct order
            return connectionChain;
        }

        // Visit all neighbors (friends) of the current human
        for (auto& edge : Network.getEdges(currentVertex)) {
            auto neighbor = edge->getDestination();
            if (parent.find(neighbor) == parent.end()) {  // If the neighbor has not been visited
                toVisit.push(neighbor);
                parent[neighbor] = currentVertex;  // Set the parent of the neighbor to the current human
            }
        }
    }

    // If no path exists, return an empty vector
    return {};
}
