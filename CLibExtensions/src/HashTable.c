#include "CLib/LinkedList.h"
#include "CLib/HashTable.h"
#include "CLib/Vector.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

// The number of buckets that the table starts with, by default.
#define DEFAULT_INITIAL_BUCKET_SIZE 13

// The max number of buckets the HashTable can ever have is 500000. Since each LinkedList creates
// some overhead per item, it's probably a good idea to not reach the absolute max number of buckets.
#define MAX_BUCKET_COUNT 500000

// The default rehashing to capacity factor.
#define DEFAULT_REHASH_FACTOR 0.75

typedef struct tagHashTableItem
{
	// The size of the key, in bytes.
	int keySize;
	// The actual value that the key associated with this item hashed to.
	int hashedKeyValue;
	// The key for this item.
	void* key;
	// The value associated with the key.
	void* value;
} HashTableItem;

typedef struct tagHashTable
{
	// Each "bucket" of the HashTable is a LinkedList.
	LinkedList** buckets;
	// The number of items that are currently stored in this HashTable.
	int numItems;
	// The size of the datatype that this HashTable stores.
	int dataTypeSize;
	// A pointer to the equals function to use for comparing two value objects.
	EqualsFunction* valueEqualsFunction;
	// A pointer to the equals function to use for comparing two key objects.
	EqualsFunction* keyEqualsFunction;
	// A pointer to the hash code function to use for locating the bucket of an item.
	HashCodeFunction* hashFunction;
	// The length of the buckets array.
	int bucketArrayLength;
	// The rehashing factor for the table. Given a capacity, if the number of items comes within
	// this percentage of the table, it will be rehashed to be bigger. Defaults to 75% since
	// Java does it that way.
	float rehashFactor;
} HashTable;

// Finds the next prime number after the given number.
int NextPrimeNumber(int startValue)
{
	for(int i = startValue; i < MAX_BUCKET_COUNT; i++)
	{
		bool isPrime = true;
		// For each value of i, check if it is prime.
		for(int j = 2; j < (int)sqrt(i); j++)
		{
			// If the current number being checked can be evenly divided by anything before the square root of that
			// number, it is not prime.
			if (i % j == 0)
			{
				isPrime = false;
				break;
			}		
		}

		// If the isPrime variable was left true, that means the value of i is a prime number.
		if (isPrime)
			return i;
	}

	// Got all the way to MAX_BUCKET_COUNT.
	return MAX_BUCKET_COUNT;
}

void HashTableDefaultInitData(HashTableInitData* data)
{
	data->dataTypeSize = 4;
	data->rehashFactor = DEFAULT_REHASH_FACTOR;
	data->initialBucketSize = DEFAULT_INITIAL_BUCKET_SIZE;
	data->hashCodeImpl = NULL;
	data->keyEqualsImpl = data->valueEqualsImpl = NULL;
}

bool HashTableCreate(HashTableInitData* initData, HashTable** hash)
{
	if (initData == NULL)
		return false;

	// The equals function for keys must be provided.
	if (initData->keyEqualsImpl == NULL)
		return false;
	// The HashCode function must be provided.
	if (initData->hashCodeImpl == NULL)
		return false;
	// The data size must be greater than 0.
	if (initData->dataTypeSize <= 0)
		return false;
	// The rehash factor, if not 0 or negative, must be a value between 0 and 1.
	if (initData->rehashFactor > 0 && initData->rehashFactor > 1)
		return false;

	(*hash) = calloc(1, sizeof(HashTable));

	HashTable* outerHash = (*hash);
	outerHash->dataTypeSize = initData->dataTypeSize;
	outerHash->keyEqualsFunction = initData->keyEqualsImpl;
	outerHash->valueEqualsFunction = initData->valueEqualsImpl;
	outerHash->hashFunction = initData->hashCodeImpl;
	
	// If no initial capacity is specified (0 or negative), start with the default capacity.
	if (initData->initialBucketSize <= 0)
		outerHash->bucketArrayLength = DEFAULT_INITIAL_BUCKET_SIZE;
	else
		outerHash->bucketArrayLength = initData->initialBucketSize;

	// If no rehash factor is specified, use the default rehash factor.
	if (initData->rehashFactor <= 0)
		outerHash->rehashFactor = DEFAULT_REHASH_FACTOR;
	else
		outerHash->rehashFactor = initData->rehashFactor;

	outerHash->buckets = calloc((*hash)->bucketArrayLength, sizeof(LinkedList*));
	for(int i = 0; i < (*hash)->bucketArrayLength; i++)
	{
		LinkedListCreate(sizeof(HashTableItem), &outerHash->buckets[i]);
	}

	return true;
}

void HashTableDestroy(HashTable** hash)
{
	HashTable* outerHash = (*hash);

	// Destroy the data in each HashTableItem in each LinkedList.
	for(int i = 0; i < outerHash->bucketArrayLength; i++)
	{
		LinkedList* currentList = outerHash->buckets[i];
		for(int j = 0; j < LinkedListCount(currentList); j++)
		{
			HashTableItem* item = LinkedListGet(j, currentList);
			free(item->key);
			free(item->value);
		}

		// Destroy will destroy the LinkedList and all the HashTableItem objects in the LinkedList.
		LinkedListDestroy(&currentList);
	}

	free(outerHash->buckets);
	free(outerHash);
	(*hash) = NULL;
}

bool HashTableContainsKey(const void* key, HashTable* hash)
{
	// Get the hash code that the key creates, as well as the bucket that the key represents.
	int hashCode = hash->hashFunction(key);
	LinkedList* list = hash->buckets[hashCode % hash->bucketArrayLength];

	// Go through the HashTableItems of the linked list. If any of them contain the hash code generated by this
	// key, the key is considered in the HashTable.
	for(int i = 0; i < LinkedListCount(list); i++)
	{
		HashTableItem* item = LinkedListGet(i, list);
		// The key exists in this HashTable.
		if (item->hashedKeyValue == hashCode && hash->keyEqualsFunction(key, item->key))
			return true;
	}

	// The key was not found.
	return false;
}

bool HashTableContainsValue(const void* value, HashTable* hash)
{
	// Assert if a value equals function was never provided.
	assert(hash->valueEqualsFunction != NULL);
		
	// Loop through all buckets.
	for(int i = 0; i < hash->bucketArrayLength; i++)
	{
		LinkedList* list = hash->buckets[i];

		// Loop through the LinkedList.
		for(int j = 0; j < LinkedListCount(list); j++)
		{
			// Get the HashTableItem of the current LinkedList, and check if its value variable is equal to the
			// provided one.
			HashTableItem* item = LinkedListGet(j, list);
			if (hash->valueEqualsFunction(item->value, value))
				return true;
		}
	}

	return false;
}

bool HashTableAdd(const void* key, int keyDataSize, const void* value, HashTable* hash)
{
	// Which bucket does the value go into, given its key?
	int hashedValue = hash->hashFunction(key);

	// Grab the proper linked list.
	LinkedList* list = hash->buckets[hashedValue % hash->bucketArrayLength];

	// Walk the linked list, and see if it already contains the given value. If it doesn't, add it to the list.
	for(int i = 0; i < LinkedListCount(list); i++)
	{
		HashTableItem* hashItem = LinkedListGet(i, list);
		
		// Is the hashed key value and the keys equal?
		if(hashItem->hashedKeyValue == hashedValue && hash->keyEqualsFunction(key, hashItem->key))
		{
			// The element could not be added because it already exists in the table.
			return false;
		}
	}

	// Before adding this item, is a Rehash necessary?
	if(hash->numItems + 1 > (hash->rehashFactor * hash->bucketArrayLength))
	{
		// Perform a Rehash.
		HashTableRehash(hash);

		// Set the "list" variable to the new correct linked list for the item. After a Rehash, the number
		// of items has likely changed, and therefore the bucket this new item goes into is different.
		list = hash->buckets[hashedValue % hash->bucketArrayLength];
	}

	// The key did not map to anything, which means it and the value can be safely added.
	HashTableItem item;
	item.hashedKeyValue = hashedValue;
	item.value = calloc(1, hash->dataTypeSize);
	item.key = calloc(1, keyDataSize);
	item.keySize = keyDataSize;
	memcpy(item.value, value, hash->dataTypeSize);
	memcpy(item.key, key, keyDataSize);

	// Push this item to the end of the given linked list.
	LinkedListPush(&item, list);

	// The number of items  in this HashTable has gone up by 1.
	hash->numItems++;

	// The Key/Value pair was added to the HashTable successfully.
	return true;
}

void* HashTableGet(const void* key, HashTable* hash)
{
	// Get the hash that the key represents, and get the bucket that the key maps to.
	int hashCode = hash->hashFunction(key);
	LinkedList* list = hash->buckets[hashCode % hash->bucketArrayLength];

	for(int i = 0; i < LinkedListCount(list); i++)
	{
		HashTableItem* item = LinkedListGet(i, list);
		if(item->hashedKeyValue == hashCode && hash->keyEqualsFunction(key, item->key))
		{
			// Found the item.
			return item->value;
		}
	}

	// The given key does not map to any value in the table.
	return NULL;
}

bool HashTableRemove(const void* key, HashTable* hash)
{
	int hashCode = hash->hashFunction(key);
	LinkedList* list = hash->buckets[hashCode % hash->bucketArrayLength];

	// Go through the current bucket, and try to find the HashTableItem.
	for(int i = 0; i < LinkedListCount(list); i++)
	{
		HashTableItem* item = LinkedListGet(i, list);

		// Now, is this item's hashed value equal to the hash value created from the key?
		if (item->hashedKeyValue == hashCode && hash->keyEqualsFunction(item->key, key))
		{
			// Found the item. It needs to be removed.
			free(item->value);
			free(item->key);
			LinkedListRemove(i, list);
			// The number of items decreased by 1.
			hash->numItems--;
			return true;
		}
	}

	// Could not find the item to remove.
	return false;
}

void HashTableClear(HashTable* hash)
{
	// To clear the HashTable, loop through and clear all the linked list buckets.
	for (int i = 0; i < hash->bucketArrayLength; i++)
	{
		LinkedList* list = hash->buckets[i];

		// Before clearing the LinkedList, have to destroy the dynamically allocated memory that exists in each
		// HashTableItem.
		for (int j = 0; j < LinkedListCount(list); j++)
		{
			HashTableItem* item = LinkedListGet(j, list);
			free(item->key);
			free(item->value);
		}
		
		// It is now safe to clear the LinkedList. This will destroy all the HashTableItems in the list.
		LinkedListClear(list);
	}
		
	// There are now 0 items in the HashTable.
	hash->numItems = 0;
}

int HashTableCount(HashTable* hash)
{
	return hash->numItems;
}

bool HashTableIsEmpty(HashTable* hash)
{
	return (hash->numItems == 0);
}

int HashTableCountBuckets(HashTable* hash)
{
	return hash->bucketArrayLength;
}

void HashTableRehash(HashTable* hash)
{
	// To Rehash the table, all the HashTableItem values need to be pulled out, the LinkedList array
	// needs to be reallocated to be bigger, and then all items need to be placed into the new,
	// appropriate buckets.

	// The first step is to reallocate the linked list array to be double its current size.
	int oldArrayLength = hash->bucketArrayLength;
	// Double the size of the bucket array, and then round up to the nearest prime number.
	hash->bucketArrayLength = NextPrimeNumber(hash->bucketArrayLength * 2);
	hash->buckets = realloc(hash->buckets, sizeof(LinkedList*) * hash->bucketArrayLength);

	// Instantiate a LinkedList object at the new locations.
	for (int i = oldArrayLength; i < hash->bucketArrayLength; i++)
		LinkedListCreate(sizeof(HashTableItem), &hash->buckets[i]);

	// Iterate through all the LinkedLists before the new section of them, pull out the HashTableItem, and
	// then insert them into a vector for temporary storage. This vector will just store the pointers (so essentially
	// numbers) for each HashTableItem, instead of the item itself.
	Vector* hashTableItems;
	VectorCreate(sizeof(uintptr_t), hash->numItems, hash->numItems, NULL, &hashTableItems);

	for(int i = 0; i < oldArrayLength; i++)
	{
		if (LinkedListCount(hash->buckets[i]) == 0)
			continue;
		LinkedList* list = hash->buckets[i];

		// Go through each item of the LinkedList. To speed up the process, just keep popping items instead of
		// using the remove method.
		while(!LinkedListIsEmpty(list))
		{
			// Pull out (remove without deletion) the HashTableItem.
			HashTableItem* item = LinkedListPopSafe(list);
			// Store the memory address of item in an unsigned integer, and then store that value into the Vector.
			uintptr_t pointerLocation = (uintptr_t)item;
			VectorAdd(&pointerLocation, hashTableItems);
		}
	}

	// Finally, go back through all the items, and redistribute them into the correct buckets.
	for(int i = 0; i < VectorCount(hashTableItems); i++)
	{
		// Add the items back to the appropriate LinkedLists. The address of the current item is stored in the 
		// Vector; convert that back into a HashTableItem pointer.
		uintptr_t pointerLocation = *(uintptr_t*)VectorGet(i, hashTableItems);
		HashTableItem* item = (HashTableItem*)pointerLocation;
		LinkedListPush(item, hash->buckets[item->hashedKeyValue % hash->bucketArrayLength]);
	}

	// Destroy the vector. This is safe because it will only be deleting void*s, not the items themselves.
	VectorDestroy(&hashTableItems);
}

bool HashTableCopy(HashTable* other, HashTable* hash)
{
	// The other HashTable can't be null.
	if (other == NULL)
		return false;

	// Loop through each entry of the given hash table.
	for(int i = 0; i < hash->bucketArrayLength; i++)
	{
		LinkedList* currentList = hash->buckets[i];

		// Loop through the entries of the LinkedList.
		for(int j = 0; j < LinkedListCount(currentList); j++)
		{
			HashTableItem* item = LinkedListGet(j, currentList);
			
			// Attempt to add this entry into the other HashTable. Whether it succeeds or not
			// is ignored here. TODO: Probably should not be ignored. Implement later.
			HashTableAdd(item->key, item->keySize, item->value, other);
		}
	}

	return true;
}