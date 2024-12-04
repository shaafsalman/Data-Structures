#include "SocialNetwork.h"

// -------------------- Human Class Methods --------------------

Human::Human() {
    this->name = "";
    this->age = 0;
    this->UniqueID = uniqueID_counter++;
    this->Friends = Graph<shared_ptr<Human>>();
}

Human::Human(string name, int age) {
    this->name = name;
    this->age = age;
    this->UniqueID = uniqueID_counter++;
    this->Friends = Graph<shared_ptr<Human>>();
}

Human::Human(const Human& human) {
    this->name = human.name;
    this->age = human.age;
    this->UniqueID = human.UniqueID;
    this->Friends = human.Friends;
}

int Human::getUniqueID() {
    return this->UniqueID;
}

string Human::getName() {
    return this->name;
}

int Human::getAge() {
    return this->age;
}

void Human::addFriend(shared_ptr<Human> Friend) {
    if (!this->Friends.getVertex(shared_from_this())) {
        this->Friends.addVertex(shared_from_this());
    }
    if (!this->Friends.getVertex(Friend)) {
        this->Friends.addVertex(Friend);
    }
    this->Friends.addEdge(shared_from_this(), Friend);
}

void Human::removeFriend(shared_ptr<Human> Friend) {
    // Ensure both humans are part of the graph
    auto thisVertex = this->Friends.getVertex(shared_from_this());
    auto friendVertex = this->Friends.getVertex(Friend);
    
    if (!thisVertex || !friendVertex) {
        cout << "Error: One or both humans not found in the graph!" << endl;
        return;  // If either human isn't found, exit early
    }

    // Debug: Check the list of friends before removal
    cout << "Before removal: " << this->getName() << "'s friends: ";
    auto friendsBeforeRemoval = this->getFriends();
    for (auto& f : friendsBeforeRemoval) {
        cout << f->getName() << " ";
    }
    cout << endl;

    // Remove the edge between this human and the friend
    this->Friends.removeEdge(shared_from_this(), Friend);
    this->Friends.removeEdge(Friend, shared_from_this()); // Ensure bidirectional removal

    // Debug: Verify if the edge was removed
    auto friendsAfterRemoval = this->getFriends();
    cout << "After removal: " << this->getName() << "'s friends: ";
    for (auto& f : friendsAfterRemoval) {
        cout << f->getName() << " ";
    }
    cout << endl;
}


vector<shared_ptr<Human>> Human::getFriends() {
    vector<shared_ptr<Human>> friendsList;
    auto thisVertex = this->Friends.getVertex(shared_from_this());
    
    if (!thisVertex) {
        cout << "Error: This human is not in the graph!" << endl;
        return friendsList; // Return empty list if the vertex is not found
    }

    // Ensure the adjacency list is up-to-date
    auto adjacentVertices = this->Friends.getAdjacentVertices(thisVertex);

    // Debug: Print the adjacency list to check for correctness
    cout << "Adjacency list for " << this->getName() << ": ";
    for (auto vertex : adjacentVertices) {
        cout << vertex->getData()->getName() << " ";
    }
    cout << endl;

    for (auto vertex : adjacentVertices) {
        friendsList.push_back(vertex->getData());
    }
    
    return friendsList;
}


// -------------------- SocialNetwork Class Methods --------------------

SocialNetwork::SocialNetwork() {
    this->NumberOfHumans = 0;
    this->Network = Graph<shared_ptr<Human>>(false, false);
}

void SocialNetwork::addHuman(shared_ptr<Human> human) {
    if (!this->Network.getVertex(human)) {
        this->Network.addVertex(human);
        this->NumberOfHumans++;
    }
}

void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    auto humanVertex = this->Network.getVertex(human);
    if (!humanVertex) return;

    for (auto friendVertex : this->Network.getAdjacentVertices(humanVertex)) {
        this->Network.removeEdge(human, friendVertex->getData());
    }
    this->Network.removeVertex(human);
    this->NumberOfHumans--;
}

void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    if (!this->Network.getVertex(human1)) {
        this->Network.addVertex(human1);
    }
    if (!this->Network.getVertex(human2)) {
        this->Network.addVertex(human2);
    }
    this->Network.addEdge(human1, human2);
    this->Network.addEdge(human2, human1); // Ensure bi-directionality
}

void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    this->Network.removeEdge(human1, human2);
    this->Network.removeEdge(human2, human1); // Remove in both directions
}

vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> mutualFriends;
    auto vertex1 = this->Network.getVertex(human1);
    auto vertex2 = this->Network.getVertex(human2);

    if (!vertex1 || !vertex2) return mutualFriends;

    auto friends1 = this->Network.getAdjacentVertices(vertex1);
    auto friends2 = this->Network.getAdjacentVertices(vertex2);

    unordered_set<shared_ptr<Human>> friendSet1;
    for (auto f1 : friends1) {
        friendSet1.insert(f1->getData());
    }

    for (auto f2 : friends2) {
        if (friendSet1.find(f2->getData()) != friendSet1.end()) {
            mutualFriends.push_back(f2->getData());
        }
    }
    return mutualFriends;
}

vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    vector<vector<shared_ptr<Human>>> groups;
    auto components = this->Network.connectedComponents();

    for (auto& component : components) {
        vector<shared_ptr<Human>> group;
        for (auto& vertex : component) {
            group.push_back(vertex->getData());
        }
        groups.push_back(group);
    }
    return groups;
}

bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    auto vertex1 = this->Network.getVertex(human1);
    auto vertex2 = this->Network.getVertex(human2);

    return vertex1 && vertex2 && !this->Network.shortestPath(vertex1, vertex2).empty();
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> connectionChain;
    auto vertex1 = this->Network.getVertex(human1);
    auto vertex2 = this->Network.getVertex(human2);

    if (!vertex1 || !vertex2) return connectionChain;

    auto path = this->Network.shortestPath(vertex1, vertex2);
    for (auto& vertex : path) {
        connectionChain.push_back(vertex->getData());
    }
    return connectionChain;
}
