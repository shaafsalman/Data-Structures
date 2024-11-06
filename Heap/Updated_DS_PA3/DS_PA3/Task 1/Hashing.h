#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>

using namespace std;
// Enumeration to choose collision handling method
enum CollisionHandling
{
    LINEAR_PROBING,      // Linear probing
    QUADRATIC_PROBING,   // Quadratic probing
    DOUBLE_HASHING,      // Double hashing
    SEPARATE_CHAINING    // Separate chaining using a vector of vectors
};

template <typename T>
class HashTable
{
private:
    static const int TABLE_SIZE = 20; // The size of the hash table

    // Structure to hold key-value pairs for both probing and chaining methods
    struct KeyValue
    {
        int key;
        T value;
        bool isEmpty; // Flag to indicate if a slot is empty

        KeyValue() : key(0), value(T()), isEmpty(true) {}
        KeyValue(int k, T v, bool empty) : key(k), value(v), isEmpty(empty) {}

        // Overload equality operator
        bool operator==(const KeyValue &other) const
        {
            return key == other.key && value == other.value && isEmpty == other.isEmpty;
        }
    };

    // Hash table for probing methods
    vector<KeyValue> probingTable;

    // Hash table for separate chaining (vector of vectors)
    vector<vector<KeyValue>> chainingTable;

    // The collision resolution strategy being used
    CollisionHandling collisionStrategy;

    // Primary hash function (used for all methods)
    int hashFunction1(int key);

    // Secondary hash function (used for double hashing)
    int hashFunction2(int key);

    // Linear probing functions
    void insertLinearProbing(int key, T value);
    T searchLinearProbing(int key);
    void removeLinearProbing(int key);

    // Quadratic probing functions
    void insertQuadraticProbing(int key, T value);
    T searchQuadraticProbing(int key);
    void removeQuadraticProbing(int key);

    // Double hashing functions
    void insertDoubleHashing(int key, T value);
    T searchDoubleHashing(int key);
    void removeDoubleHashing(int key);

    // Separate chaining functions
    void insertSeparateChaining(int key, T value);
    T searchSeparateChaining(int key);
    void removeSeparateChaining(int key);

public:
    // Constructor to initialize the hash table and choose collision strategy
    HashTable(CollisionHandling strategy = LINEAR_PROBING);
    ~HashTable();

    // Insert a key-value pair based on the chosen collision strategy
    void insert(int key, T value);

    // Search for a key and return its associated value based on the chosen strategy
    T search(int key);

    // Remove a key-value pair based on the chosen collision strategy
    void remove(int key);

    // Print the contents of the hash table (for debugging purposes)
    void printTable();
};

#endif