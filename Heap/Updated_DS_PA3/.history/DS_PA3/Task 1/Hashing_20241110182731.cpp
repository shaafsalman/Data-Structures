#include "Hashing.h"
using namespace std;

// ============================ Constructor & Destructor ============================ //

// Constructor to initialize the hash table
template <typename T>
HashTable<T>::HashTable(CollisionHandling strategy) : collisionStrategy(strategy)
{
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
    return 1 + (key % (TABLE_SIZE - 1));
}

// ============================ Linear Probing ============================ //

// Insert using linear probing
template <typename T>
void HashTable<T>::insertLinearProbing(int key, T value)
{
    // TODO: Implement insert using linear probing

    int index = hashFunction1(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty && probingTable[index].key != key)
    {
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex)
        {
            // cout << "Hash table is full! cant insert  " << key << endl;
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
    // cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using linear probing
template <typename T>
void HashTable<T>::removeLinearProbing(int key)
{
    // TODO: Implement remove using linear probing

    int index = hashFunction1(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty)
    {
        if (probingTable[index].key == key)
        {
            probingTable[index].isEmpty = true;
            probingTable[index].key = 0;
            probingTable[index].value = T();
            // cout << "Key " << key << " removed" << endl;
            return;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    // cout << "Key " << key << " not found 404" << endl;
}

// ============================ Quadratic Probing ============================ //

// Insert using quadratic probing
template <typename T>
void HashTable<T>::insertQuadraticProbing(int key, T value)
{
    // TODO: Implement insert using quadratic probing

    int index = hashFunction1(key);
    int i = 0;
    int startIndex = index;

    while (!probingTable[index].isEmpty && probingTable[index].key != key)
    {
        i++;
        index = (startIndex + i * i) % TABLE_SIZE;
        if (index == startIndex)
        {
            // cout << "Hash table is full! cant insert " << key << endl;
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
    // TODO: Implement search using quadratic probing

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
    // TODO: Implement remove using quadratic probing
    
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
            // cout << "Key " << key << " removed" << endl;
            return;
        }
        i++;
        index = (startIndex + i * i) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    // cout << "Key " << key << " not found for removal" << endl;
}

// ============================ Double Hashing ============================ //

// Insert using double hashing
template <typename T>
void HashTable<T>::insertDoubleHashing(int key, T value)
{
        // TODO: Implement insert using double hashing

    int index = hashFunction1(key);
    int step = hashFunction2(key);
    int startIndex = index;

    while (!probingTable[index].isEmpty && probingTable[index].key != key)
    {
        index = (index + step) % TABLE_SIZE;
        if (index == startIndex)
        {
            cout << "Hash table is full! cant insert" << key << endl;
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
    // TODO: Implement search using double hashing

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
    // cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using double hashing
template <typename T>
void HashTable<T>::removeDoubleHashing(int key)
{
    // TODO: Implement remove using double hashing
        
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
            // cout << "Key " << key << " removed" << endl;
            return;
        }
        index = (index + step) % TABLE_SIZE;
        if (index == startIndex)
        {
            break;
        }
    }
    // cout << "Key " << key << " not found 404" << endl;
}

// ============================ Separate Chaining ============================ //

// Insert using separate chaining (vector of vectors)
template <typename T>
void HashTable<T>::insertSeparateChaining(int key, T value)
{
    // TODO: Implement insert using separate chaining
    
    int index = hashFunction1(key);
    chainingTable[index].push_back(KeyValue(key, value, false));
}

// Search using separate chaining (vector of vectors)
template <typename T>
T HashTable<T>::searchSeparateChaining(int key)
{
    // TODO: Implement search using separate chaining

    int index = hashFunction1(key);
    for (int i = 0; i < chainingTable[index].size(); i++)
    {
        if (chainingTable[index][i].key == key)
        {
            return chainingTable[index][i].value;
        }
    }
    cout << "Key " << key << " not found" << endl;
    return T();
}

// Remove using separate chaining (vector of vectors)
template <typename T>
void HashTable<T>::removeSeparateChaining(int key)
{
    // TODO: Implement remove using separate chaining

    int index = hashFunction1(key);
    for (int i = 0; i < chainingTable[index].size(); i++)
    {
        if (chainingTable[index][i].key == key)
        {
            chainingTable[index].erase(chainingTable[index].begin() + i);
            // cout << "Key " << key << " removed" << endl;
            return;
        }
    }
    // cout << "Key " << key << " not found for removal" << endl;
}

// ============================ General Interface ============================ //

// General insert function (chooses method based on collision strategy)
template <typename T>
void HashTable<T>::insert(int key, T value)
{
    // TODO: Implement general insert function


    if (collisionStrategy == LINEAR_PROBING)
        insertLinearProbing(key, value);
    else if (collisionStrategy == QUADRATIC_PROBING)
        insertQuadraticProbing(key, value);
    else if (collisionStrategy == DOUBLE_HASHING)
        insertDoubleHashing(key, value);
    else if (collisionStrategy == SEPARATE_CHAINING)
        insertSeparateChaining(key, value);
}

// General search function (chooses method based on collision strategy)
template <typename T>
T HashTable<T>::search(int key)
{
        // TODO: Implement general search function


    if (collisionStrategy == LINEAR_PROBING)
        return searchLinearProbing(key);
    else if (collisionStrategy == QUADRATIC_PROBING)
        return searchQuadraticProbing(key);
    else if (collisionStrategy == DOUBLE_HASHING)
        return searchDoubleHashing(key);
    else if (collisionStrategy == SEPARATE_CHAINING)
        return searchSeparateChaining(key);
    
    return T();
}

// General remove function (chooses method based on collision strategy)
template <typename T>
void HashTable<T>::remove(int key)
{
    // TODO: Implement general remove function
   
    if (collisionStrategy == LINEAR_PROBING)
        removeLinearProbing(key);
    else if (collisionStrategy == QUADRATIC_PROBING)
        removeQuadraticProbing(key);
    else if (collisionStrategy == DOUBLE_HASHING)
        removeDoubleHashing(key);
    else if (collisionStrategy == SEPARATE_CHAINING)
        removeSeparateChaining(key);
}

// Print the contents of the hash table (for debugging purposes)
template <typename T>
void HashTable<T>::printTable()
{
    // TODO: Implement print table function
   
    if (collisionStrategy == SEPARATE_CHAINING)
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            cout << "Bucket " << i << ": ";
            for (int j = 0; j < chainingTable[i].size(); j++)
            {
                cout << "[" << chainingTable[i][j].key << ": " << chainingTable[i][j].value << "] ";
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
