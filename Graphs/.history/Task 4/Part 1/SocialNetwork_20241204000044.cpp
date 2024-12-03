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
    this->Friends.addVertex(Friend);
    this->Friends.addEdge(shared_from_this(), Friend);
}

void Human::removeFriend(shared_ptr<Human> Friend) {
    this->Friends.removeEdge(shared_from_this(), Friend);
    this->Friends.removeVertex(Friend);
}

vector<shared_ptr<Human>> Human::getFriends() {
    return this->Friends.getAllVertices();
}

// -------------------- SocialNetwork Class Methods --------------------

SocialNetwork::SocialNetwork() {
    this->NumberOfHumans = 0;
    this->Network = Graph<shared_ptr<Human>>(false, false);
}

void SocialNetwork::addHuman(shared_ptr<Human> human) {
    this->Network.addVertex(human);
    this->NumberOfHumans++;
}

void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    for (auto friendHuman : this->Network.getAdjacentVertices(human)) {
        this->Network.removeEdge(human, friendHuman);
    }
    this->Network.removeVertex(human);
    this->NumberOfHumans--;
}

void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    this->Network.addEdge(human1, human2);
}

void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    this->Network.removeEdge(human1, human2);
}

vector<shared_ptr<Human>> SocialNetwork::getFriends(shared_ptr<Human> human) {
    return this->Network.getAdjacentVertices(human);
}

vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> mutualFriends;
    auto friends1 = this->Network.getAdjacentVertices(human1);
    auto friends2 = this->Network.getAdjacentVertices(human2);

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
    return !this->Network.shortestPath(human1, human2).empty();
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    vector<shared_ptr<Human>> connectionChain;
    auto path = this->Network.shortestPath(human1, human2);

    for (auto& vertex : path) {
        connectionChain.push_back(vertex->getData());
    }
    return connectionChain;
}
