#include <iostream>
#include <vector>
#include <future>
#include <chrono>
#include <memory>
#include <thread>
#include "../Task 1/Hashing.cpp" // Include your hash table implementation

using namespace std;

// Global variables for timeout handling
bool timeOut;
promise<bool> done;

// Enum for collision handling types

// Timer function to handle timeout
void timer(future<bool> done_future) {
    chrono::seconds span(3);
    if (done_future.wait_for(span) == future_status::timeout) {
        timeOut = true;
    }
}

// Helper function to convert CollisionHandling to string
string typeToString(CollisionHandling type) {
    switch (type) {
        case LINEAR_PROBING: return "Linear Probing";
        case QUADRATIC_PROBING: return "Quadratic Probing";
        case DOUBLE_HASHING: return "Double Hashing";
        case SEPARATE_CHAINING: return "Separate Chaining";
        default: return "Unknown";
    }
}

// Template function to test hash table with both int and string types
template<typename T>
double runTest(vector<int> keys, vector<T> values, CollisionHandling type, promise<bool> done_future) {
    using namespace chrono;

    HashTable<T> table(type); // Using different collision strategies
    double score = 0;
    auto startTime = high_resolution_clock::now();

    cout << "Testing Insertions (" << typeToString(type) << "): ";

    // Insert values into the hash table
    for (size_t i = 0; i < keys.size(); ++i) {
        table.insert(keys[i], values[i]);
        if (timeOut) {
            cout << "Test timed out!" << endl;
            return score;
        }
    }
    score += 3; // Insertion test contributes 3 points
    cout << "Passed!" << endl;

    cout << "Testing Search (" << typeToString(type) << "):    ";

    // Search for values in the hash table
    for (size_t i = 0; i < keys.size(); ++i) {
        T result = table.search(keys[i]);
        if (result != values[i]) {
            cout << "Search failed!" << endl;
            return score;
        }
        if (timeOut) {
            cout << "Test timed out!" << endl;
            return score;
        }
    }
    score += 3; // Search test contributes 3 points
    cout << "Passed!" << endl;

    cout << "Testing Removal (" << typeToString(type) << "):   ";

    // Remove values from the hash table
    for (size_t i = 0; i < keys.size(); ++i) {
        table.remove(keys[i]);
        if (timeOut) {
            cout << "Test timed out!" << endl;
            return score;
        }
    }

    // Ensure keys have been removed
    for (size_t i = 0; i < keys.size(); ++i) {
        if (table.search(keys[i]) != T()) {
            cout << "Removal failed!" << endl;
            return score;
        }
        if (timeOut) {
            cout << "Test timed out!" << endl;
            return score;
        }
    }
    score += 1.5; // Removal test contributes 1.5 points
    cout << "Passed!" << endl;

    done_future.set_value(true);
    auto endTime = high_resolution_clock::now();
    duration<double> totalTime = duration_cast<duration<double>>(endTime - startTime);

    cout << "Test completed in: " << totalTime.count() << " seconds." << endl;
    return score;
}

int main() {
    // Integer keys and values
    vector<int> intKeys = {10, 21, 32, 43, 54, 65, 76, 87, 98, 109};
    vector<int> intValues = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

    // String keys and values
    vector<int> strKeys = {10, 21, 32, 43, 54, 65, 76, 87, 98, 109};
    vector<string> strValues = {"Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Hannah", "Ivy", "John"};

    // Define the different types of collision handling strategies
    CollisionHandling collisionTypes[] = {LINEAR_PROBING, QUADRATIC_PROBING, DOUBLE_HASHING, SEPARATE_CHAINING};

    double totalScore = 0;

    for (auto type : collisionTypes) {
        // Initialize timeout control for integer test
        timeOut = false;
        promise<bool> doneIntPromise;
        future<bool> doneIntFuture = doneIntPromise.get_future();
        thread intTimer(timer, move(doneIntFuture));

        // Run integer test
        cout << "Testing " << typeToString(type) << " with integer values..." << endl;
        double intScore = runTest(intKeys, intValues, type, move(doneIntPromise));
        intTimer.join();  // Wait for the timer thread to finish

        // Initialize timeout control for string test
        timeOut = false;
        promise<bool> doneStrPromise;
        future<bool> doneStrFuture = doneStrPromise.get_future();
        thread strTimer(timer, move(doneStrFuture));

        // Run string test
        cout << "----------------------------------------------" << endl;
        cout << "Testing " << typeToString(type) << " with string values..." << endl;
        double strScore = runTest(strKeys, strValues, type, move(doneStrPromise));
        strTimer.join();  // Wait for the timer thread to finish

        double collisionScore = (intScore + strScore) / 2; // Each method contributes up to 7.5 points
        totalScore += collisionScore;
        cout << "----------------------------------------------" << endl;
        cout << "Test results for " << typeToString(type) << ":" << endl;
        cout << "Combined Test Score: " << collisionScore << "/7.5" << endl;
        cout << "----------------------------------------------" << endl;
    }

    cout << "Total Combined Score for All Hashing Methods: " << totalScore << "/30" << endl;

    return 0;
}