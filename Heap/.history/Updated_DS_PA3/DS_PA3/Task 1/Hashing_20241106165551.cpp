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
    int index = hashFunction1(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            return probingTable[index].value;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using linear probing
template <typename T>
void HashTable<T>::removeLinearProbing(int key)
{
    int index = hashFunction1(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            probingTable[index].isEmpty = true;
            probingTable[index].key = 0;
            probingTable[index].value = T();
            cout << "Key " << key << " removed" << endl;
            return;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    cout << "Key " << key << " not found for removal" << endl;
}

// ============================ Quadratic Probing ============================ //

// Insert using quadratic probing
template <typename T>
void HashTable<T>::insertQuadraticProbing(int key, T value)
{
    int index = hashFunction1(key);
    int i = 0;
    int startIndex = index;

    while (!probingTable[index].isEmpty && probingTable[index].key != key)
    {
        i++;
        index = (startIndex + i * i) % TABLE_SIZE;
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

// Search using quadratic probing
template <typename T>
T HashTable<T>::searchQuadraticProbing(int key)
{
    int index = hashFunction1(key);
    int i = 0;
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            return probingTable[index].value;
        }
        i++;
        index = (startIndex + i * i) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using quadratic probing
template <typename T>
void HashTable<T>::removeQuadraticProbing(int key)
{
    int index = hashFunction1(key);
    int i = 0;
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            probingTable[index].isEmpty = true;
            probingTable[index].key = 0;
            probingTable[index].value = T();
            cout << "Key " << key << " removed" << endl;
            return;
        }
        i++;
        index = (startIndex + i * i) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    cout << "Key " << key << " not found for removal" << endl;
}

// ============================ Double Hashing ============================ //

// Insert using double hashing
template <typename T>
void HashTable<T>::insertDoubleHashing(int key, T value)
{
    int index = hashFunction1(key);
    int step = hashFunction2(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty && probingTable[index].key != key)
    {
        index = (index + step) % TABLE_SIZE;
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

// Search using double hashing
template <typename T>
T HashTable<T>::searchDoubleHashing(int key)
{
    int index = hashFunction1(key);
    int step = hashFunction2(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            return probingTable[index].value;
        }
        index = (index + step) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using double hashing
template <typename T>
void HashTable<T>::removeDoubleHashing(int key)
{
    int index = hashFunction1(key);
    int step = hashFunction2(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            probingTable[index].isEmpty = true;
            probingTable[index].key = 0;
            probingTable[index].value = T();
            cout << "Key " << key << " removed" << endl;
            return;
        }
        index = (index + step) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    cout << "Key " << key << " not found for removal" << endl;
}

// ============================ Separate Chaining ============================ //

// Insert using separate chaining (vector of vectors)
template <typename T>
void HashTable<T>::insertSeparateChaining(int key, T value)
{
    int index = hashFunction1(key);
    chainingTable[index].push_back(KeyValue(key, value, false));
}

// Search using separate chaining (vector of vectors)
template <typename T>
T HashTable<T>::searchSeparateChaining(int key)
{
    int index = hashFunction1(key);
    for (const auto &pair : chainingTable[index])
    {
        if (pair.key == key)
        {
            return pair.value;
        }
    }
    cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using separate chaining (vector of vectors)
template <typename T>
void HashTable<T>::removeSeparateChaining(int key)
{
    int index = hashFunction1(key);
    for (auto it = chainingTable[index].begin(); it != chainingTable[index].end(); ++it)
    {
        if (it->key == key)
        {
            chainingTable[index].erase(it);
            cout << "Key " << key << " removed" << endl;
            return;
        }
    }
    cout << "Key " << key << " not found for removal" << endl;
}

// ============================ General Interface ============================ //

// General insert function (chooses method based on collision strategy)
template <typename T>
void HashTable<T>::insert(int key, T value)
{
    switch (collisionStrategy)
    {
        case LINEAR_PROBING:
            insertLinearProbing(key, value);
            break;
        case QUADRATIC_PROBING:
            insertQuadraticProbing(key, value);
            break;
        case DOUBLE_HASHING:
            insertDoubleHashing(key, value);
            break;
        case SEPARATE_CHAINING:
            insertSeparateChaining(key, value);
            break;
    }
}

// General search function (chooses method based on collision strategy)
template <typename T>
T HashTable<T>::search(int key)
{
    switch (collisionStrategy)
    {
        case LINEAR_PROBING:
            return searchLinearProbing(key);
        case QUADRATIC_PROBING:
            return searchQuadraticProbing(key);
        case DOUBLE_HASHING:
            return searchDoubleHashing(key);
        case SEPARATE_CHAINING:
            return searchSeparateChaining(key);
    }
    return T();
}

// General remove function (chooses method based on collision strategy)
template <typename T>
void HashTable<T>::remove(int key)
{
    switch (collisionStrategy)
    {
        case LINEAR_PROBING:
            removeLinearProbing(key);
            break;
        case QUADRATIC_PROBING:
            removeQuadraticProbing(key);
            break;
        case DOUBLE_HASHING:
            removeDoubleHashing(key);
            break;
        case SEPARATE_CHAINING:
            removeSeparateChaining(key);
            break;
    }
}

// Print the contents of the hash table (for debugging purposes)
template <typename T>
void HashTable<T>::printTable()
{
    if (collisionStrategy == SEPARATE_CHAINING)
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            cout << "Bucket " << i << ": ";
            for (const auto &pair : chainingTable[i])
            {
                cout << "[" << pair.key << ": " << pair.value << "] ";
            }
            cout << endl;
        }
    }
    else
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            if (!probingTable[i].isEmpty)
            {
                cout << "[" << probingTable[i].key << ": " << probingTable[i].value << "] ";
            }
            else
            {
                cout << "[Empty] ";
            }
        }
        cout << endl;
    }
}

#endif
