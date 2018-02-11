/*
 * Frank Nkosi Dean
 * Vector.h
 */

#pragma once

#include "CLibraryHeaders.h"
#include "CLibraryExtensions.h"

// Represents an array that is resized automatically as items are added to it.
typedef struct tagVector Vector;

// Creates a new vector object, allocating an initial size.
CLIBRARY_API bool VectorCreate(unsigned int dataTypeSize, unsigned int initialCapacity, unsigned int maxCapacity,
							   EqualsFunction* equalsImpl, CompareToFunction* compareImpl, Vector** vect);

// Destroys a vector object.
CLIBRARY_API void VectorDestroy(Vector** vect);

// Adds an item to the end of the vector.
CLIBRARY_API bool VectorAdd(const void* data, Vector* vect);

// Inserts an item into the vector at the specified index.
CLIBRARY_API bool VectorInsert(const void* data, int index, Vector* vect);

// Replaces the data at the specified index with the new data.
CLIBRARY_API bool VectorReplace(const void* data, int index, Vector* vect);

// Replaces the data at the specified index with the new data, but copies and returns the data that will
// be overwritten. Returns null if the operation fails.
CLIBRARY_API void* VectorReplaceSafe(const void* data, int index, Vector* vect);

// Removes the item at the specified index from the vector.
CLIBRARY_API bool VectorRemove(int index, Vector* vect);

// Remvoes the item at the specified index from the vector, but copies and returns that data first. Returns
// null if the operation fails.
CLIBRARY_API void* VectorRemoveSafe(int index, Vector* vect);

// Gets the first index that a particular element is at. If the element is not found, returns -1. This requires
// a pointer to an appropriate "Equals" method implementation to have been set.
CLIBRARY_API int VectorIndexOf(const void* data, Vector* vect);

// Checks if the vector contains a certain value. This requires a pointer to an appropriate "Equals" method
// implementation to have been set.
CLIBRARY_API bool VectorContains(const void* data, Vector* vect);

// Gets the number of items that are currently stored in a vector.
CLIBRARY_API unsigned int VectorCount(Vector* vect);

// Returns true if the vector is empty, otherwise returns false.
CLIBRARY_API bool VectorIsEmpty(Vector* vect);

// Gets the item at the specified index in the given vector.
CLIBRARY_API void* VectorGet(int index, Vector* vect);

// Removes all items from the vector.
CLIBRARY_API void VectorClear(Vector* vect);

// Copies all the contents of the vector "vect" to the end of the vector "vectOther".
// Note this copies only the used part of the data, not any unused parts of data.
CLIBRARY_API bool VectorCopy(Vector* vectOther, Vector* vect);

// Sorts the items in a vector. This requires a pointer to an appropriate "CompareTo" method implementation
// to have been set.
CLIBRARY_API void VectorSort(Vector* vect);