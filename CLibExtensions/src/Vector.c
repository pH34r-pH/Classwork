#include "CLib/Vector.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// The implementation of the vector object.
typedef struct tagVector
{
	// The size of the data type stored in this vector.
	int dataTypeSize;
	// The current capacity of the vector.
	int currentCapacity;
	// The maximum capacity of the vector. If a maximum capacity is not used, this is -1.
	int maxCapacity;
	// The data that this vector stores.
	void* data;
	// The number of items stored in the vector.
	int numItems;
	// An equals implementation to compare two objects.
	EqualsFunction* equalsFunction;
} Vector;

bool VectorCreate(int dataTypeSize, int initialCapacity, int maxCapacity, EqualsFunction* equalImpl, Vector** vect)
{
	if (dataTypeSize <= 0)
		return false;
	if (initialCapacity < 0)
		return false;

	// Create a new vector object.
	(*vect) = calloc(1, sizeof(Vector));
	(*vect)->dataTypeSize = dataTypeSize;
	(*vect)->maxCapacity = maxCapacity;
	(*vect)->numItems = 0;
	(*vect)->currentCapacity = initialCapacity;
	(*vect)->equalsFunction = equalImpl;
	(*vect)->data = calloc(initialCapacity, dataTypeSize);

	return true;
}

void VectorDestroy(Vector** vect)
{
	// Destroy the actual data array.
	free((*vect)->data);
	// Destroy the vector object.
	free((*vect));
	(*vect) = NULL;
}

bool VectorAdd(const void* data, Vector* vect)
{
	// If a max capacity was set and adding an item would exceed that max capacity, return false.
	if (vect->maxCapacity > 0 && vect->maxCapacity < vect->numItems + 1)
		return false;

	if (vect->currentCapacity < vect->numItems + 1)
	{
		// Need to increase the capacity of the vector. Since initial capacity can be a zero, take that into account.
		vect->currentCapacity += (vect->currentCapacity > 0 ? vect->currentCapacity : 1) * 2;
		vect->data = realloc(vect->data, vect->currentCapacity * vect->dataTypeSize);
	}

	// Add this item to the vector, copying the data since the item is likely stack allocated.
	memcpy((char*)vect->data + (vect->dataTypeSize * vect->numItems), data, vect->dataTypeSize);

	// The size of the vector increased.
	vect->numItems++;

	return true;
}

bool VectorInsert(const void* data, int index, Vector* vect)
{
	// If a max capacity was set and adding an item would exceed that max capacity, return false.
	if (vect->maxCapacity > 0 && vect->maxCapacity < vect->numItems + 1)
		return false;

	// If the insert index is not valid, return false. If it is at the end of the array, just call VectorPush.
	// Otherwise, continue this method.
	if (index < 0 || index > vect->numItems)
		return false;
	if (index == vect->numItems)
		return VectorAdd(data, vect);

	if (vect->currentCapacity < vect->numItems + 1)
	{
		// Need to increase the capacity of the vector. Since initial capacity can be a zero, take that into account.
		vect->currentCapacity += (vect->currentCapacity > 0 ? vect->currentCapacity : 1) * 2;
		vect->data = realloc(vect->data, vect->currentCapacity * vect->dataTypeSize);
	}

	// Shift the proper subset of elements (elements to the right of the insertion index) 1 to the right.
	memmove(
		(char*)vect->data + (vect->dataTypeSize * (index + 1)),
		(char*)vect->data + (vect->dataTypeSize * index),
		vect->dataTypeSize * (vect->numItems - index));

	// Perform the memcpy into the data array.
	memcpy((char*)vect->data + (vect->dataTypeSize * index), data, vect->dataTypeSize);

	// The number of items goes up by 1.
	vect->numItems++;

	return true;
}

bool VectorReplaceInternal(const void* data, int index, void** overwrittenData, Vector* vect)
{
	// Replacing a value neither adds or removes any data. The data at the specified index is just overwritten.

	// The index must be valid.
	if (index < 0 || index > vect->numItems - 1)
		return false;
	
	// Overwrite the section of the data array with the new memory value. If overwrittenData is not NULL, copy the
	// section of memory that will be overwritten to return to the caller first.
	if (overwrittenData != NULL)
		memcpy(*overwrittenData, (char*)vect->data + (vect->dataTypeSize * index), vect->dataTypeSize);
	// Overwrite the section of data with the new data.
	memcpy((char*)vect->data + (vect->dataTypeSize * index), data, vect->dataTypeSize);

	return true;
}

bool VectorReplace(const void* data, int index, Vector* vect)
{
	return VectorReplaceInternal(data, index, NULL, vect);
}

void* VectorReplaceSafe(const void* data, int index, Vector* vect)
{
	void* savedData = calloc(1, vect->dataTypeSize);
	if(!VectorReplaceInternal(data, index, &savedData, vect))
	{
		free(savedData);
		return NULL;
	}
	return savedData;
}

bool VectorRemoveInternal(int index, void** removedData, Vector* vect)
{
	// Can't remove from an empty array.
	if (vect->numItems == 0)
		return false;

	// The index to remove needs to be valid.
	if (index < 0 || index > vect->numItems - 1)
		return false;

	// If removedData is not NULL, first copy out the section of data that will be overwritten when memmove is used.
	if (removedData != NULL)
		memcpy(*removedData, (char*)vect->data + (index * vect->dataTypeSize), vect->dataTypeSize);

	// Because of how the data array is used, removing an item means shifting all items down 1 to overwrite the
	// location. However, no memory is freed.
	memmove(
		(char*)vect->data + (index * vect->dataTypeSize),
		(char*)vect->data + ((index + 1) * vect->dataTypeSize),
		vect->dataTypeSize * (vect->numItems - index - 1));

	// The numbe rof items goes down by 1.
	vect->numItems--;

	return true;
}

bool VectorRemove(int index, Vector* vect)
{
	return VectorRemoveInternal(index, NULL, vect);
}

void* VectorRemoveSafe(int index, Vector* vect)
{
	void* savedData = calloc(1, vect->dataTypeSize);
	if(!VectorRemoveInternal(index, &savedData, vect))
	{
		free(savedData);
		return NULL;
	}
	return savedData;
}

int VectorIndexOf(const void* data, Vector* vect)
{
	// The equals function must be set to use the IndexOf method.
	assert(vect->equalsFunction != NULL);

	// Loop through the vector and try to find the first instance of the given object.
	for (int i = 0; i < vect->numItems; i++)
		if (vect->equalsFunction(data, VectorGet(i, vect)))
			return i;
	// The item was not found.
	return -1;
}

bool VectorContains(const void* data, Vector* vect)
{
	// The equals function must be set to use the Contains method.
	assert(vect->equalsFunction != NULL);

	return VectorIndexOf(data, vect) != -1;
}

int VectorCount(Vector* vect)
{
	return vect->numItems;
}

bool VectorIsEmpty(Vector* vect)
{
	return (vect->numItems == 0);
}

void* VectorGet(int index, Vector* vect)
{
	// If index is out of bounds, return null.
	if (index < 0 || index > vect->numItems - 1)
		return NULL;

	// Get the item at the specified index.
	return (char*)vect->data + (vect->dataTypeSize * index);
}

void VectorClear(Vector* vect)
{
	// Because of how the array is implemented, clearing a vector just means setting the number of items to zero.
	vect->numItems = 0;
}

bool VectorCopy(Vector* vectOther, Vector* vect)
{
	// The size of the datatype needs to be the same in both vectors.
	if (vectOther->dataTypeSize != vect->dataTypeSize)
		return false;

	// The first thing to do is ensure vectOther has enough space to take in the contents of this
	// vector.
	int resultSize = vectOther->numItems + vect->numItems;
	if (vectOther->currentCapacity < resultSize)
	{
		// vectOther needs to have its space expanded. Is it allowed to do this?
		if (vectOther->maxCapacity > 0 && vectOther->maxCapacity < resultSize)
			return false;

		// vectOther can have its space expanded. Expand it to double however many items are to be
		// added to it, plus its previous capacity.
		vectOther->currentCapacity += resultSize * 2;
		vectOther->data = realloc(vectOther->data, vectOther->dataTypeSize * resultSize);
	}

	// Copy the entirety of the vector into the proper position of vectOther.
	memcpy((char*)vectOther->data + (vectOther->dataTypeSize * vectOther->numItems), vect->data, vect->numItems * vect->dataTypeSize);

	// The number of items in the other vector has gone up.
	vectOther->numItems = resultSize;

	return true;
}