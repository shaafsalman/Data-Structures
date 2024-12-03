#include "SocialNetwork.h"

SocialNetwork::SocialNetwork() {
    // Initialize the social network
    humans = make_shared<Graph<Human>>();  // Create a graph for all humans
}

// Add a human to the network
void SocialNetwork::addHuman(shared_ptr<Human> human) {
    // Add the human to the graph
    humans->addVertex(human->getUniqueID());
}

// Remove a human from the network
void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    // Remove the human and all their connections from the network
    humans->removeVertex(human->getUniqueID());

    // Remove all friendships associated with this human
    for (auto friendVertex : human->getFriends()) {
        humans->removeEdge(human->getUniqueID(), friendVertex->getUniqueID());
    }
}

// Add a friendship between two humans
void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Add mutual friendship by adding edges between both humans in their respective graphs
    human1->addFriend(human2);
    human2->addFriend(human1);
}

// Remove a friendship between two humans
void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Remove the friendship by removing edges in both directions
    human1->removeFriend(human2);
    human2->removeFriend(human1);
}

// Get a list of friends of a human
vector<shared_ptr<Human>> SocialNetwork::getFriends(shared_ptr<Human> human) {
    return human->getFriends();
}

// Get mutual friends between two humans
vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> mutualFriends;
    auto friends1 = human1->getFriends();
    auto friends2 = human2->getFriends();

    // Find intersection of the two friend lists
    for (auto f1 : friends1) {
        for (auto f2 : friends2) {
            if (f1->getUniqueID() == f2->getUniqueID()) {
                mutualFriends.push_back(f1);
            }
        }
    }
    return mutualFriends;
}

// Get groups of connected humans (friend circles)
vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    // Find all connected components in the network
    return humans->connectedComponents();
}

// Check if two humans can be connected (direct or indirect)
bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    auto path = humans->shortestPath(human1, human2);
    return !path.empty();  // If path exists, they are connected
}

// Get the chain of connections between two humans (if exists)
vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> connectionChain;
    
    auto path = humans->shortestPath(human1, human2);
    
    if (path.empty()) {
        return connectionChain;  // If no connection exists, return empty vector
    }
    
    // Extract the chain from the path
    for (auto vertex : path) {
        connectionChain.push_back(vertex);
    }
    
    return connectionChain;
}
