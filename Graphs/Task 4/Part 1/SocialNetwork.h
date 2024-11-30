#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H
#include <iostream>
#include <vector>
#include <memory>

#include "../../Task 3/Graph_Task_3.cpp"

using namespace std;

int uniqueID_counter = 0;
class Human : public enable_shared_from_this<Human> {
    int UniqueID;
    string name;
    int age;
    Graph<shared_ptr<Human>> Friends;

public:
    Human();
    Human(string name, int age);
    Human(const Human& human);
    int getUniqueID();
    string getName();
    int getAge();
    void addFriend(shared_ptr<Human> Friend);
    void removeFriend(shared_ptr<Human> Friend);
    vector<shared_ptr<Human>> getFriends(); 
    // equality operator
    bool operator==(const Human& human) const {
        return this->UniqueID == human.UniqueID;
    }
    friend ostream& operator<<(ostream& os, const Human& human) {
        os << human.name;
        return os;
    }
};

// SocialNetwork class to store the network of humans
class SocialNetwork {
    int NumberOfHumans;

public:
    Graph<shared_ptr<Human>> Network;
    SocialNetwork();
    void addHuman(shared_ptr<Human> human);
    void removeHuman(shared_ptr<Human> human);
    void addFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2);
    void removeFriendship(shared_ptr<Human> human1, shared_ptr<Human> human2);
    vector<shared_ptr<Human>> getFriends(shared_ptr<Human> human);
    vector<shared_ptr<Human>> getMutualFriends(shared_ptr<Human> human1, shared_ptr<Human> human2);
    vector<vector<shared_ptr<Human>>> getGroups();
    bool canBeConnected(shared_ptr<Human> human1, shared_ptr<Human> human2);
    vector<shared_ptr<Human>> connectionOrder(shared_ptr<Human> human1, shared_ptr<Human> human2);
};
#endif