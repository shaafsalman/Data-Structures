#include "SocialNetwork.h"

// Static ID initializer
int uniqueID_counter = 0;

// ------------------- Human Class -------------------

Human::Human() {
    this->UniqueID = uniqueID_counter++;
    this->name = "Unnamed";
    this->age = 0;
    this->Friends = Graph<shared_ptr<Human>>(false, false); // Undirected, unweighted graph
}

Human::Human(string name, int age) {
    this->UniqueID = uniqueID_counter++;
    this->name = name;
    this->age = age;
    this->Friends = Graph<shared_ptr<Human>>(false, false); // Undirected, unweighted graph
}

Human::Human(const Human& human) {
    this->UniqueID = human.UniqueID; // Copy the same unique ID
    this->name = human.name;
    this->age = human.age;
    this->Friends = human.Friends; // Copy the friends graph
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
    return this->Friends.getAdjacentVertices(shared_from_this());
}

// ------------------- SocialNetwork Class -------------------

SocialNetwork::SocialNetwork() {
    this->NumberOfHumans = 0;
    this->Network = Graph<shared_ptr<Human>>(false, false); // Undirected, unweighted graph
}

void SocialNetwork::addHuman(shared_ptr<Human> human) {
    this->Network.addVertex(human);
    this->NumberOfHumans++;
}

void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    vector<shared_ptr<Human>> friends = this->getFriends(human);
    for (auto& friendHuman : friends) {
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
    vector<shared_ptr<Human>> friends1 = this->getFriends(human1);
    vector<shared_ptr<Human>> friends2 = this->getFriends(human2);
    vector<shared_ptr<Human>> mutualFriends;

    for (auto& friend1 : friends1) {
        if (find(friends2.begin(), friends2.end(), friend1) != friends2.end()) {
            mutualFriends.push_back(friend1);
        }
    }
    return mutualFriends;
}

vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    return this->Network.connectedComponents();
}

bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    return this->Network.canReach(human1, human2);
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // Breadth-First Search (BFS) for shortest path
    queue<vector<shared_ptr<Human>>> q;
    unordered_set<int> visited;
    q.push({human1});

    while (!q.empty()) {
        auto path = q.front();
        q.pop();
        shared_ptr<Human> last = path.back();

        if (last->getUniqueID() == human2->getUniqueID()) {
            return path;
        }

        if (visited.count(last->getUniqueID()) == 0) {
            visited.insert(last->getUniqueID());
            for (auto& neighbor : this->getFriends(last)) {
                if (visited.count(neighbor->getUniqueID()) == 0) {
                    vector<shared_ptr<Human>> newPath = path;
                    newPath.push_back(neighbor);
                    q.push(newPath);
                }
            }
        }
    }

    return {}; // Return empty if no connection
}
