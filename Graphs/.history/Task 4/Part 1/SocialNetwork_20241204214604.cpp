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
    // return a vector of groups of humans (all humans are friends with each other)

    // Solution:
    
}

bool SocialNetwork::canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // check if there is a connection between these two humans or not return true or false

    // Solution:
}

vector<shared_ptr<Human>> SocialNetwork::connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2) {
    // return the connection chain between these two humans if it exists else return empty vector
    // Can not use map or set or pair
    // Solution:
}
