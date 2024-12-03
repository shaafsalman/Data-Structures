#include "SocialNetwork.h"

// Constructor to initialize the SocialNetwork
SocialNetwork::SocialNetwork() {
    // Initialize the social network graph for all humans
    NumberOfHumans = 0;
}

// Add a human to the network
void SocialNetwork::addHuman(shared_ptr<Human> human) {
    // Add the human to the network graph
    Network.addVertex(human->getUniqueID());  // Add the human's unique ID to the network graph
    NumberOfHumans++;  // Increment the number of humans
}

// Remove a human from the network
void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    // Remove the human and all their connections from the network
    Network.removeVertex(human->getUniqueID());  // Remove the human's vertex from the graph

    // Remove all friendships associated with this human
    for (auto friendHuman : human->getFriends()) {
        Network.removeEdge(human->getUniqueID(), friendHuman->getUniqueID());  // Remove edges (friendships)
    }

    NumberOfHumans--;  // Decrement the number of humans
}

// Add a friendship between two humans
void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Add a mutual friendship by adding edges between both humans
    Network.addEdge(human1->getUniqueID(), human2->getUniqueID(), 1);  // Add edge from human1 to human2
    Network.addEdge(human2->getUniqueID(), human1->getUniqueID(), 1);  // Add edge from human2 to human1 (mutual friendship)
}

// Remove a friendship between two humans
void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Remove the friendship between two humans by removing the edge in both directions
    Network.removeEdge(human1->getUniqueID(), human2->getUniqueID());  // Remove edge from human1 to human2
    Network.removeEdge(human2->getUniqueID(), human1->getUniqueID());  // Remove edge from human2 to human1
}

// Get a list of friends of a human
vector<shared_ptr<Human>> SocialNetwork::getFriends(shared_ptr<Human> human) {
    vector<shared_ptr<Human>> friends;
    // Retrieve the friends of the human from the graph
    auto friendIDs = Network.getAdjacentVertices(human->getUniqueID());  // Get adjacent vertices (friends)
    for (auto id : friendIDs) {
        friends.push_back(make_shared<Human>(Network.getVertex(id)));  // Convert IDs to Human objects and add them to the list
    }
    return friends;
}

// Get mutual friends between two humans
vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> mutualFriends;
    auto friends1 = getFriends(human1);
    auto friends2 = getFriends(human2);

    // Find the intersection (mutual friends)
    for (auto f1 : friends1) {
        for (auto f2 : friends2) {
            if (*f1 == *f2) {  // Check if the friends are the same
                mutualFriends.push_back(f1);
            }
        }
    }
    return mutualFriends;
}

// Get all groups of connected humans (connected components)
vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    // Use the graph's connected components method to get all groups
    return Network.connectedComponents();  // Assume Graph has a connectedComponents method
}

// Check if two humans can be connected (either directly or indirectly)
bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    auto path = Network.shortestPath(human1->getUniqueID(), human2->getUniqueID());  // Get the shortest path between the two humans
    return !path.empty();  // If path exists, they are connected
}

// Get the chain of connections (order of connections) between two humans
vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> connectionChain;
    auto path = Network.shortestPath(human1->getUniqueID(), human2->getUniqueID());  // Get shortest path

    if (path.empty()) {
        return connectionChain;  // Return empty if no path exists
    }

    // Convert vertex IDs to Human objects and build the connection chain
    for (auto id : path) {
        connectionChain.push_back(make_shared<Human>(Network.getVertex(id)));
    }
    return connectionChain;
}
