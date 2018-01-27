/**
 * @file LinkedList.h
 * @author Nkosi Dean
 */

#pragma once

#include "CLibraryHeaders.h"

// Represents a Linked List of items. This can also act as a queue of objects or stack of objects.
typedef struct tagLinkedList LinkedList;

// Creates a new LinkedList object.
CLIBRARY_API bool LinkedListCreate(int dataTypeSize, LinkedList** list);

// Destroys a LinkedList of items.
CLIBRARY_API void LinkedListDestroy(LinkedList** list);

// Adds a new item to the end of a linked list.
CLIBRARY_API bool LinkedListPush(const void* value, LinkedList* list);

// Inserts a new item into the linked list.
CLIBRARY_API bool LinkedListInsert(const void* value, int index, LinkedList* list);

// Removes the item at the end of the linked list.
CLIBRARY_API bool LinkedListPop(LinkedList* list);

// Removes the item at the end of the linked list, without freeing the memory. Instead,
// the value is returned. NULL is returned on failure.
CLIBRARY_API void* LinkedListPopSafe(LinkedList* list);

// Removes the item at the specified index of a linked list.
CLIBRARY_API bool LinkedListRemove(int index, LinkedList* list);

// Removes the item at the specified index of a linked list, without freeing the memory. Instead,
// the value is returned.
CLIBRARY_API void* LinkedListRemoveSafe(int index, LinkedList* list);

// Removes all items from the LinkedList, leaving it in an empty state.
CLIBRARY_API void LinkedListClear(LinkedList* list);

// Gets the number of items contained in this linked list.
CLIBRARY_API int LinkedListCount(LinkedList* list);

// Returns true if the LinkedList has no values, otherwise returns false.
CLIBRARY_API bool LinkedListIsEmpty(LinkedList* list);

// Gets the value at the specfied index from the LinkedList.
CLIBRARY_API void* LinkedListGet(int index, LinkedList* list);

// Gets the data at the head of the list.
CLIBRARY_API void* LinkedListGetFront(LinkedList* list);

// Gets the data at the tail of the list.
CLIBRARY_API void* LinkedListGetBack(LinkedList* list);