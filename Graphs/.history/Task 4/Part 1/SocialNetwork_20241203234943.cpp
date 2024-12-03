#include "SocialNetwork.h"

Human::Human() {
    // Constructor for the Human class
    // Set the name and age of the human
    // name
    // age
    // Friends: create the default graph to store the friends of the human

    // Solution:
    this->UniqueID = uniqueID_counter++; // Assign a unique ID
    this->Friends = Graph<shared_ptr<Human>>(); // Create a default empty graph for friends
}

Human::Human(string name, int age) {
    // Constructor for the Human class
    // Set the name and age of the human
    // name: name of the human
    // age: age of the human
    // Friends: create the default graph to store the friends of the human
    // UniqueID: assign a unique ID to the human

    // Solution:
    this->name = name;
    this->age = age;
    this->UniqueID = uniqueID_counter++; // Assign a unique ID
    this->Friends = Graph<shared_ptr<Human>>(); // Create a default empty graph for friends
}

Human::Human(const Human& human) {
    // Copy Constructor for the Human class
    // Copy the name, age, Friends and UniqueID of the human passed as argument

    // Solution:
    this->name = human.name;
    this->age = human.age;
    this->UniqueID = human.UniqueID;
    this->Friends = human.Friends; // Assuming Graph supports deep copy or shared ownership
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
    this->Friends.addEdge(shared_from_this(), Friend); // Assuming Graph supports addEdge
}

void Human::removeFriend(shared_ptr<Human> Friend) {
    // search and remove the friend from the Graph

    // Solution:
    this->Friends.removeEdge(shared_from_this(), Friend); // Assuming Graph supports removeEdge
}

vector<shared_ptr<Human>> Human::getFriends() {
    // return the friends of the human

    // Solution:
    return this->Friends.getFriends(shared_from_this()); // Assuming Graph supports getFriends or getNeighbors
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

SocialNetwork::SocialNetwork() {
    // Construct the Social Network
    // Initialize the Graph and the number of humans

    // Solution:
    this->NumberOfHumans = 0;
    this->Network = Graph<shared_ptr<Human>>(); // Initialize the network as an empty graph
}

void SocialNetwork::addHuman(shared_ptr<Human> human) {
    // Add the humans and their networks

    // Solution:
    this->Network.addVertex(human); // Assuming Graph supports addVertex
    this->NumberOfHumans++;
}

void SocialNetwork::removeHuman(shared_ptr<Human> human) {
    // Remove any and all connections to this human. Make sure not to affect other people's networks

    // Solution:
    this->Network.removeVertex(human); // Assuming Graph supports removeVertex
    this->NumberOfHumans--;
}

void SocialNetwork::addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // create a friendship between these two humans and add it to the graph.

    // Solution:
    this->Network.addEdge(human1, human2); // Assuming Graph supports addEdge
}

void SocialNetwork::removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // remove that single friendship but do not do anything else

    // Solution:
    this->Network.removeEdge(human1, human2); // Assuming Graph supports removeEdge
}

vector<shared_ptr<Human>> SocialNetwork::getFriends(shared_ptr<Human> human) {
    // return the friends of the human passed as argument

    // Solution:
    return this->Network.getFriends(human); // Assuming Graph supports getFriends or getNeighbors
}

vector<shared_ptr<Human>> SocialNetwork::getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // return the mutual friends (a central connection between Human 1 and Human 2) of the people passed (Not directly connected to each other)

    // Solution:
    vector<shared_ptr<Human>> friends1 = this->Network.getFriends(human1);
    vector<shared_ptr<Human>> friends2 = this->Network.getFriends(human2);
    vector<shared_ptr<Human>> mutualFriends;

    for (auto& friend1 : friends1) {
        for (auto& friend2 : friends2) {
            if (*friend1 == *friend2) {
                mutualFriends.push_back(friend1);
            }
        }
    }

    return mutualFriends;
}

vector<vector<shared_ptr<Human>>> SocialNetwork::getGroups() {
    // return a vector of groups of humans (all humans are friends with each other)

    // Solution:
    return this->Network.connectedComponents(); // Assuming Graph supports connectedComponents
}

bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // check if there is a connection between these two humans or not return true or false

    // Solution:
    return this->Network.isConnected(human1, human2); // Assuming Graph supports isConnected
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // return the connection chain between these two humans if it exists else return empty vector
    // Can not use map or set or pair
    // Solution:
    return this->Network.shortestPath(human1, human2); // Assuming Graph supports shortestPath
}
