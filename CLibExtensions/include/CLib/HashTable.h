/**
 * @file HashTable.h
 * @author Nkosi Dean
 */

#pragma once

#include "CLibraryExtensions.h"

// Represents a Data Structure that allows for fast, O(1) search time to get items.
typedef struct tagHashTable HashTable;

// Represents data used to create a new HashTable.
typedef struct tagHashTableInitData
{
	// The rehashing factor (Java's HashTable and HashMap load factor). When the percentage of items in the table to
	// size of the table passes this, the table will rehash. Value defaults to 75%. Specify 0 or a negative number
	// to use the default value.
	float rehashFactor;
	// The size of each item that is to be stored in the HashTable.
	int dataTypeSize;
	// The number of buckets (or capacity) that the table starts with. Should be a prime number to help reduce
	// collisions. Specify 0 or a negative number to use the default value of 13.
	int initialBucketSize;
	// An equals function implementation to check if two keys are equal. This is required.
	EqualsFunction* keyEqualsImpl;
	// An equals function implementation to check if two values are equal. Not required unless you plan on using
	// the HashTableContainsValue function.
	EqualsFunction* valueEqualsImpl;
	// A HashCode function that returns a number uniquely identifying an instance of something. Used to pick
	// the bucket that a value goes into based on the key. This is required.
	HashCodeFunction* hashCodeImpl;

} HashTableInitData;

// Fills a HashTableInitData object with default values, though the required equals function and hash function
// must be set.
CLIBRARY_API void HashTableDefaultInitData(HashTableInitData* data);

// Creates a new HashTable object. The valueEqualsImpl method is optional and used for HashTableContainsValue.
CLIBRARY_API bool HashTableCreate(HashTableInitData* initData, HashTable** hashTable);

// Destroys a HashTable object.
CLIBRARY_API void HashTableDestroy(HashTable** hashTable);

// Checks if a particular key is stored in the HashTable.
CLIBRARY_API bool HashTableContainsKey(const void* key, HashTable* hash);

// Checks if a particular value is stored at least once in the HashTable. This iterates all buckets and returns true
// if the provided equals method ever returns true when comparing the provided value to all other stored values.
CLIBRARY_API bool HashTableContainsValue(const void* value, HashTable* hash);

// Adds a value in the HashTable, given the value and the key representing the item. If the operation succeeds, returns
// true. Returns false if the value already exists in the HashTable.
CLIBRARY_API bool HashTableAdd(const void* key, int keyDataSize, const void* value, HashTable* hash);

// Gets a value in the HashTable, given the key that the value maps to. Returns null if the value that the key maps
// to is not found.
CLIBRARY_API void* HashTableGet(const void* key, HashTable* hash);

// Removes the item from the table with the given key. Returns true if removal was successful, otherwise false, as
// the key did not map to anything in the table.
CLIBRARY_API bool HashTableRemove(const void* key, HashTable* hash);

// Deletes everything from the HashTable.
CLIBRARY_API void HashTableClear(HashTable* hash);

// Gets the number of items that exist in the table.
CLIBRARY_API int HashTableCount(HashTable* hash);

// Returns true if the HashTable has no values in it, otherwise false.
CLIBRARY_API bool HashTableIsEmpty(HashTable* hash);

// Gets the number of buckets that exist in the table.
CLIBRARY_API int HashTableCountBuckets(HashTable* hash);

// Causes the HashTable to Rehash itself. This means it will double the number of buckets there are, and re-evaluate
// all the current values and rehash the keys and put them into different buckets.
CLIBRARY_API void HashTableRehash(HashTable* hash);

// Copies the contents of the HashTable "hash" into the HashTable "other".
// Duplicate values will not be copied over. It is assumed that the other HashTable
// uses the same 
CLIBRARY_API bool HashTableCopy(HashTable* other, HashTable* hash);