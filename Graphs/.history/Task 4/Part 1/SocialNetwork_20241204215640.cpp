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
     Network.addEdge(human1, human2, 1);  // Assuming weight 1 for friendship
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
    // Check if there is a connection between these two humans (direct or indirect).
    
    // Edge case: If either human is null, return false
    if (!human1 || !human2) {
        return false;
    }

    // Retrieve the vertices for the humans from the network
    auto vertex1 = Network.getVertex(human1);
    auto vertex2 = Network.getVertex(human2);
    
    // If any human is not found in the network, return false
    if (!vertex1 || !vertex2) {
        return false;
    }

    // Perform a BFS or DFS to check if a path exists between the two humans
    unordered_set<shared_ptr<Vertex<shared_ptr<Human>>>> visited;
    queue<shared_ptr<Vertex<shared_ptr<Human>>>> toVisit;
    
    toVisit.push(vertex1);
    visited.insert(vertex1);

    while (!toVisit.empty()) {
        auto currentVertex = toVisit.front();
        toVisit.pop();
        
        // If we reach human2, return true
        if (currentVertex == vertex2) {
            return true;
        }

        // Visit all neighbors (friends) of the current human
        for (auto& edge : Network.getEdges(currentVertex)) {
            auto neighbor = edge->getDestination();
            if (visited.find(neighbor) == visited.end()) {
                toVisit.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }

    // If no path is found, return false
    return false;
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Return the connection chain between human1 and human2 if it exists, else return an empty vector.
    // Cannot use map, set, or pair.

    // Edge case: If either human is null, return an empty vector
    if (!human1 || !human2) {
        return {};
    }

    // Retrieve the vertices for the humans from the network
    auto vertex1 = Network.getVertex(human1);
    auto vertex2 = Network.getVertex(human2);

    // If either human is not found in the network, return an empty vector
    if (!vertex1 || !vertex2) {
        return {};
    }

    // Initialize a queue for BFS and a vector to store the path
    queue<shared_ptr<Vertex<shared_ptr<Human>>>> toVisit;
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> parent(vertex1, nullptr);
    
    toVisit.push(vertex1);

    while (!toVisit.empty()) {
        auto currentVertex = toVisit.front();
        toVisit.pop();
        
        // If we find human2, reconstruct the path from human1 to human2
        if (currentVertex == vertex2) {
            vector<shared_ptr<Human>> path;
            for (auto v = currentVertex; v != nullptr; v = parent[v]) {
                path.push_back(v->getData());
            }
            reverse(path.begin(), path.end());  // Reverse the path to get the correct order
            return path;
        }

        // Visit all neighbors (friends) of the current human
        for (auto& edge : Network.getEdges(currentVertex)) {
            auto neighbor = edge->getDestination();
            if (parent[neighbor] == nullptr) {  // If this neighbor has not been visited yet
                toVisit.push(neighbor);
                parent[neighbor] = currentVertex;
            }
        }
    }

    // If no path is found, return an empty vector
    return {};
}
