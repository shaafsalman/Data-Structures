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
    vector<vector<shared_ptr<Human>>> groups;

    if (NumberOfHumans == 0) {
        return groups;
    }

    Graph<shared_ptr<Human>> graph;

    for (auto& human : Network.getAllVertices()) {
        graph.addVertex(human->getData());
    }

    for (auto& human : Network.getAllVertices()) {
        for (auto& friendPtr : human->getData()->getFriends()) {
            graph.addEdge(human->getData(), friendPtr, 1);
        }
    }

    vector<vector<shared_ptr<Vertex<shared_ptr<Human>>>>> connectedComponents = graph.connectedComponents();

    for (auto& component : connectedComponents) {
        vector<shared_ptr<Human>> group;
        for (auto& vertex : component) {
            group.push_back(vertex->getData());
        }
        groups.push_back(group);
    }

    return groups;
}




bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    if (!human1 || !human2) {
        return false;
    }

    if (human1 == human2) {
        return true;
    }

    auto vertex1 = Network.getVertex(human1);
    auto vertex2 = Network.getVertex(human2);

    if (!vertex1 || !vertex2) {
        return false;
    }

    queue<shared_ptr<Vertex<shared_ptr<Human>>>> toVisit;
    vector<bool> visited(Network.getAllVertices().size(), false);

    toVisit.push(vertex1);

    vector<shared_ptr<Vertex<shared_ptr<Human>>>> allVertices = Network.getAllVertices();
    for (size_t i = 0; i < allVertices.size(); ++i) {
        if (allVertices[i] == vertex1) {
            visited[i] = true;
            break;
        }
    }

    while (!toVisit.empty()) {
        auto currentVertex = toVisit.front();
        toVisit.pop();

        if (currentVertex == vertex2) {
            return true;
        }

        for (auto& edge : Network.getEdges(currentVertex)) {
            auto neighbor = edge->getDestination();

            // Find the index of the neighbor in the list of vertices
            for (size_t i = 0; i < allVertices.size(); ++i) {
                if (allVertices[i] == neighbor && !visited[i]) {
                    toVisit.push(neighbor);
                    visited[i] = true;
                    break;
                }
            }
        }
    }

    return false;
}
vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    if (!human1 || !human2) {
        return {};
    }

    if (human1 == human2) {
        return {human1};
    }

    auto vertex1 = Network.getVertex(human1);
    auto vertex2 = Network.getVertex(human2);

    if (!vertex1 || !vertex2) {
        return {};
    }

    vector<shared_ptr<Vertex<shared_ptr<Human>>>> visitedVertices = Network.BFSTraversal(vertex1);
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> vertices;
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> parentVertices;

    queue<shared_ptr<Vertex<shared_ptr<Human>>>> toVisit;
    toVisit.push(vertex1);
    vertices.push_back(vertex1);
    parentVertices.push_back(nullptr);

    while (!toVisit.empty()) {
        auto currentVertex = toVisit.front();
        toVisit.pop();

        if (currentVertex == vertex2) {
            shared_ptr<Vertex<shared_ptr<Human>>> vertex = vertex2;
            vector<shared_ptr<Human>> connectionChain;

            while (vertex != nullptr) {
                connectionChain.push_back(vertex->getData());

                shared_ptr<Vertex<shared_ptr<Human>>> parent = nullptr;
                for (size_t i = 0; i < vertices.size(); ++i) {
                    if (vertices[i] == vertex) {
                        parent = parentVertices[i];
                        break;
                    }
                }

                vertex = parent;
            }

            // Reverse the chain to get the correct order
            vector<shared_ptr<Human>> reversedConnectionChain;
            for (int i = connectionChain.size() - 1; i >= 0; --i) {
                reversedConnectionChain.push_back(connectionChain[i]);
            }

            return reversedConnectionChain;
        }

        vector<shared_ptr<Vertex<shared_ptr<Human>>>> adjacentVertices = Network.getAdjacentVertices(currentVertex);
        for (size_t i = 0; i < adjacentVertices.size(); ++i) {
            shared_ptr<Vertex<shared_ptr<Human>>> neighbor = adjacentVertices[i];
            bool alreadyVisited = false;

            for (size_t j = 0; j < vertices.size(); ++j) {
                if (vertices[j] == neighbor) {
                    alreadyVisited = true;
                    break;
                }
            }

            if (!alreadyVisited) {
                toVisit.push(neighbor);
                vertices.push_back(neighbor);
                parentVertices.push_back(currentVertex);
            }
        }
    }

    return {};
}
