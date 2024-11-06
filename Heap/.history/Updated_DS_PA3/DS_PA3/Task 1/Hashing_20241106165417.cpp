#include "Hashing.h"
using namespace std;

// ============================ Constructor & Destructor ============================ //

// Constructor to initialize the hash table
template <typename T>
HashTable<T>::HashTable(CollisionHandling strategy) : collisionStrategy(strategy)
{
    // Initialize the hash table based on the selected collision strategy
    if (strategy == SEPARATE_CHAINING)
        chainingTable.resize(TABLE_SIZE); // For separate chaining (vector of vectors)
    else
        probingTable.resize(TABLE_SIZE);  // For linear, quadratic, or double hashing
}

// Destructor
template <typename T>
HashTable<T>::~HashTable() {}

// ============================ Hash Functions ============================ //

// Primary hash function (common to all methods)
template <typename T>
int HashTable<T>::hashFunction1(int key)
{
    return key % TABLE_SIZE;
}

// Secondary hash function (used only for double hashing)
template <typename T>
int HashTable<T>::hashFunction2(int key)
{
    return 1 + (key % (TABLE_SIZE - 1)); // Ensures step size is non-zero
}

// ============================ Linear Probing ============================ //

// Insert using linear probing
template <typename T>
void HashTable<T>::insertLinearProbing(int key, T value)
{
    int index = hashFunction1(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty && probingTable[index].key != key)
    {
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex)
        {
            cout << "Hash table is full, cannot insert key " << key << endl;
            return;
        }
    }

    probingTable[index].key = key;
    probingTable[index].value = value;
    probingTable[index].isEmpty = false;
}

// Search using linear probing
template <typename T>
T HashTable<T>::searchLinearProbing(int key)
{
    // TODO: Implement search using linear probing
    return T(); // Placeholder return
}

// Remove using linear probing
template <typename T>
void HashTable<T>::removeLinearProbing(int key)
{
    // TODO: Implement remove using linear probing
}

// ============================ Quadratic Probing ============================ //

// Insert using quadratic probing
template <typename T>
void HashTable<T>::insertQuadraticProbing(int key, T value)
{
    // TODO: Implement insert using quadratic probing
}

// Search using quadratic probing
template <typename T>
T HashTable<T>::searchQuadraticProbing(int key)
{
    // TODO: Implement search using quadratic probing
    return T(); // Placeholder return
}

// Remove using quadratic probing
template <typename T>
void HashTable<T>::removeQuadraticProbing(int key)
{
    // TODO: Implement remove using quadratic probing
}

// ============================ Double Hashing ============================ //

// Insert using double hashing
template <typename T>
void HashTable<T>::insertDoubleHashing(int key, T value)
{
    // TODO: Implement insert using double hashing
}

// Search using double hashing
template <typename T>
T HashTable<T>::searchDoubleHashing(int key)
{
    // TODO: Implement search using double hashing
    return T(); // Placeholder return
}

// Remove using double hashing
template <typename T>
void HashTable<T>::removeDoubleHashing(int key)
{
    // TODO: Implement remove using double hashing
}

// ============================ Separate Chaining ============================ //

// Insert using separate chaining (vector of vectors)
template <typename T>
void HashTable<T>::insertSeparateChaining(int key, T value)
{
    // TODO: Implement insert using separate chaining
}

// Search using separate chaining (vector of vectors)
template <typename T>
T HashTable<T>::searchSeparateChaining(int key)
{
    // TODO: Implement search using separate chaining
    return T(); // Placeholder return
}

// Remove using separate chaining (vector of vectors)
template <typename T>
void HashTable<T>::removeSeparateChaining(int key)
{
    // TODO: Implement remove using separate chaining
}

// ============================ General Interface ============================ //

// General insert function (chooses method based on collision strategy)
template <typename T>
void HashTable<T>::insert(int key, T value)
{
    // TODO: Implement general insert function
}

// General search function (chooses method based on collision strategy)
template <typename T>
T HashTable<T>::search(int key)
{
    // TODO: Implement general search function
    return T(); // Placeholder return
}

// General remove function (chooses method based on collision strategy)
template <typename T>
void HashTable<T>::remove(int key)
{
    // TODO: Implement general remove function
}

// Print the contents of the hash table (for debugging purposes)
template <typename T>
void HashTable<T>::printTable()
{
    // TODO: Implement print table function
}