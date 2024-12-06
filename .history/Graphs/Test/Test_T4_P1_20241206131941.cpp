#include "../Task 4/Part 1/SocialNetwork.cpp"
#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

// ANSI codes for colored output
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

float marks_total = 0;
float part_marks = 0;

// Helper function to print test results
void printResult(const string& testName, bool success, float marks) {
    if (success) {
        cout << GREEN << "[PASS] " << testName << " (Marks: " << marks << ")" << RESET << endl;
        marks_total += marks;
        part_marks += marks;
    } else {
        cout << RED << "[FAIL] " << testName << " (Marks: " << marks << ")" << RESET << endl;
    }
}

void testHumanClass() {
    cout << YELLOW << "[TEST] Human class" << endl;

    // Test: Create Humans and check attributes (1 marks)
    auto h1 = make_shared<Human>("Alice", 25);
    auto h2 = make_shared<Human>("Bob", 30);

    bool nameTest = h1->getName() == "Alice" && h2->getName() == "Bob";
    bool ageTest = h1->getAge() == 25 && h2->getAge() == 30;

    printResult("Human attribute test (name and age)", nameTest && ageTest, 1);

    // Test: Add and retrieve friends (3 marks)
    h1->addFriend(h2);
    auto friends = h1->getFriends();
    bool addFriendTest = (friends.size() == 1 && friends[0]->getName() == "Bob");

    printResult("Add and retrieve friends", addFriendTest, 3);

    // Test: Remove friends (1.5 mark)
    h1->removeFriend(h2);
    friends = h1->getFriends();
    bool removeFriendTest = friends.empty();

    printResult("Remove friends", removeFriendTest, 1.5);

    cout << YELLOW << "[TEST] Human class completed -- [MARKS] = " << part_marks << endl;
    part_marks = 0;
}

void testSocialNetwork() {
    cout << YELLOW << "[TEST] SocialNetwork class" << endl;

    // Test: Create SocialNetwork and add Humans (2 marks)
    SocialNetwork sn;
    auto h1 = make_shared<Human>("Alice", 25);
    auto h2 = make_shared<Human>("Bob", 30);
    auto h3 = make_shared<Human>("Charlie", 35);

    sn.addHuman(h1);
    sn.addHuman(h2);

    bool addHumanTest = sn.Network.getAllVertices().size() == 2;
    printResult("Add humans to network", addHumanTest, 2);

    // Test: Add Friendship and validate graph edges (2 marks)
    sn.addFriendship(h1, h2);
    auto h1Friends = h1->getFriends();
    bool addFriendshipTest = h1Friends.size() == 1 && h1Friends[0]->getName() == "Bob";

    printResult("Add friendship between two humans", addFriendshipTest, 2);

    // Test: Mutual Friends (2 marks)
    h1->addFriend(h3);
    h2->addFriend(h3);
    auto mutualFriends = sn.getMutualFriends(h1, h2);
    bool mutualFriendsTest = mutualFriends.size() == 1 && mutualFriends[0]->getName() == "Charlie";

    printResult("Get mutual friends", mutualFriendsTest, 2);

    // Test: Remove Human and validate network integrity (2.5 marks)
    sn.removeHuman(h2);
    bool removeHumanTest = sn.Network.getAllVertices().size() == 1; // h2 removed

    printResult("Remove human from network", removeHumanTest, 2.5);

    cout << YELLOW << "[TEST] SocialNetwork class completed -- [MARKS] = " << part_marks << endl;
    part_marks = 0;
}

void testGraphConnectivity() {
    cout << YELLOW << "[TEST] Graph Connectivity" << endl;

    SocialNetwork sn;
    auto h1 = make_shared<Human>("Alice", 25);
    auto h2 = make_shared<Human>("Bob", 30);
    auto h3 = make_shared<Human>("Charlie", 35);
    auto h4 = make_shared<Human>("David", 40);

    sn.addHuman(h1);
    sn.addHuman(h2);
    sn.addHuman(h3);
    sn.addHuman(h4);

    sn.addFriendship(h1, h2);
    sn.addFriendship(h2, h3);

    // Test: Connectivity (2 marks)
    bool connectedTest = sn.canBeConnected(h1, h3); // Path: h1 -> h2 -> h3
    printResult("Check connectivity between humans", connectedTest, 2);

    // Test: Disconnected Humans (1.5 mark)
    bool notConnectedTest = !sn.canBeConnected(h1, h4); // No path to h4
    printResult("Check disconnected humans", notConnectedTest, 2);

    // Test: Connection Order (2 marks)
    auto path = sn.connectionOrder(h1, h3);
    bool connectionOrderTest = (path.size() == 3 && path[0] == h1 && path[1] == h2 && path[2] == h3);

    printResult("Get connection chain between humans", connectionOrderTest, 2);

    cout << YELLOW << "[TEST] Graph Connectivity completed -- [MARKS] = " << part_marks << endl;
    part_marks = 0;
}

int main() {
    cout << "Starting tests..." << endl;

    testHumanClass();
    testSocialNetwork();
    testGraphConnectivity();
    cout << GREEN << "[TOTAL MARKS] = " << marks_total << RESET << endl;
    cout << "All tests completed." << endl;
    return 0;
}