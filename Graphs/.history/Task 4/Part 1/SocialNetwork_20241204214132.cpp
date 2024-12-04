#include "SocialNetwork.h"

// Human Class Implementation
Human::Human() {
    // Default constructor
    this->UniqueID = uniqueID_counter++;
    this->name = "Unknown";
    this->age = 0;
    this->Friends = Graph<shared_ptr<Human>>();
}

Human::Human(string name, int age) {
    // Parameterized constructor
    this->UniqueID = uniqueID_counter++;
    this->name = name;
    this->age = age;
    this->Friends = Graph<shared_ptr<Human>>();
}

Human::Human(const Human& human) {
    // Copy constructor
    this->UniqueID = human.UniqueID;
    this->name = human.name;
    this->age = human.age;
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
    this->Friends.addEdge(shared_from_this(), Friend);
}

void Human::removeFriend(shared_ptr<Human> Friend) {
    this->Friends.removeEdge(shared_from_this(), Friend);
}

vector<shared_ptr<Human>> Human::getFriends() {
    // Convert adjacent vertices to shared_ptr<Human>
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> vertices = this->Friends.getAdjacentVertices(shared_from_this());
    vector<shared_ptr<Human>> friends;
    for (const auto& vertex : vertices) {
        friends.push_back(vertex->getData());
    }
    return friends;
}

// SocialNetwork Class Implementation
SocialNetwork::SocialNetwork() {
    this->NumberOfHumans = 0;
    this->Network = Graph<shared_ptr<Human>>();
}

void SocialNetwork::addHuman(shared_ptr<Human> human) {
    this->Network.addVertex(human);
    ++this->NumberOfHumans;
}

void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    // Remove all edges associated with this human
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> friends = this->Network.getAdjacentVertices(human);
    for (const auto& friendVertex : friends) {
        this->Network.removeEdge(human, friendVertex->getData());
    }
    this->Network.removeVertex(human);
    --this->NumberOfHumans;
}

void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    this->Network.addEdge(human1, human2);
}

void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    this->Network.removeEdge(human1, human2);
}

vector<shared_ptr<Human>> SocialNetwork::getFriends(shared_ptr<Human> human) {
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> vertices = this->Network.getAdjacentVertices(human);
    vector<shared_ptr<Human>> friends;
    for (const auto& vertex : vertices) {
        friends.push_back(vertex->getData());
    }
    return friends;
}

vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> friends1 = getFriends(human1);
    vector<shared_ptr<Human>> friends2 = getFriends(human2);
    vector<shared_ptr<Human>> mutualFriends;

    for (const auto& friend1 : friends1) {
        if (find(friends2.begin(), friends2.end(), friend1) != friends2.end()) {
            mutualFriends.push_back(friend1);
        }
    }
    return mutualFriends;
}

vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    vector<vector<shared_ptr<Vertex<shared_ptr<Human>>>>> components = this->Network.connectedComponents();
    vector<vector<shared_ptr<Human>>> groups;

    for (const auto& component : components) {
        vector<shared_ptr<Human>> group;
        for (const auto& vertex : component) {
            group.push_back(vertex->getData());
        }
        groups.push_back(group);
    }
    return groups;
}

bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    return this->Network.canReach(human1, human2);
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Vertex<shared_ptr<Human>>>> path = this->Network.shortestPath(human1, human2);
    vector<shared_ptr<Human>> connectionChain;

    for (const auto& vertex : path) {
        connectionChain.push_back(vertex->getData());
    }
    return connectionChain;
}
