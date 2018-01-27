/**
 * @file Stack.h
 * @author Nkosi Dean
 */

#pragma once

#include "CLibraryHeaders.h"

// Represents a LIFO Stack object.
typedef struct tagStack Stack;

// Creates a new stack object.
CLIBRARY_API bool StackCreate(int dataTypeSize, int initialCapacity, int maxStackSize, Stack** stackObj);

// Destroys a stack object.
CLIBRARY_API void StackDestroy(Stack** stackObj);

// Counts the number of items currently in the stack.
CLIBRARY_API int StackCountItems(Stack* stack);

// Returns a value indicating whether or not the stack is empty.
CLIBRARY_API bool StackIsEmpty(Stack* stack);

// Adds a new item to the top of the stack.
CLIBRARY_API bool StackPush(const void* item, Stack* stack);

// Pops off the item on the top of the stack. This will free the memory of the item as well.
CLIBRARY_API bool StackPop(Stack* stack);

// Replaces the value at the specified index of the stack with the given new item.
CLIBRARY_API bool StackReplace(const void* item, int index, Stack* stack);

// Gets the item at the top of the stack.
CLIBRARY_API void* StackPeek(Stack* stack);

// Gets the item at the specified index of the stack.
CLIBRARY_API void* StackGet(int index, Stack* stack);

// Clears out all the items of a stack.
CLIBRARY_API void StackClear(Stack* stack);