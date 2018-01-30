#include "CLib/Stack.h"
#include "CLib/Vector.h"
#include <stdlib.h>

#define DEFAULT_STACK_INITIAL_CAPACITY 100

// The implementation for a Stack. The nice thing is, a Stack is just a Vector with some extra methods.
typedef struct tagStack
{
	// The vector that stores all the data.
	Vector* vect;
} Stack;

bool StackCreate(int dataTypeSize, int initialCapacity, int maxStackSize, Stack** stackObj)
{
	// A valid data type size must be given.
	if (dataTypeSize <= 0)
		return false;

	*stackObj = calloc(1, sizeof(Stack));
	if (*stackObj == NULL)
		return false;

	// Instantiate the underlying vector.
	initialCapacity = initialCapacity <= 0 ? DEFAULT_STACK_INITIAL_CAPACITY : initialCapacity;
	if (!VectorCreate(dataTypeSize, initialCapacity, maxStackSize, NULL, &(*stackObj)->vect))
	{
		free(*stackObj);
		*stackObj = NULL;
		return false;
	}

	return true;
}

void StackDestroy(Stack** stackObj)
{
	// Destroy the underlying vector.
	VectorDestroy(&(*stackObj)->vect);

	// Destroy the stack object.
	free(*stackObj);
}

int StackCountItems(Stack* stack)
{
	return VectorCount(stack->vect);
}

bool StackIsEmpty(Stack* stack)
{
	return VectorIsEmpty(stack->vect);
}

bool StackPush(const void* item, Stack* stack)
{
	return VectorAdd(item, stack->vect);
}

bool StackPop(Stack* stack)
{
	// Remove the item at the end of the vector.
	return VectorRemove(VectorCount(stack->vect) - 1, stack->vect);
}

bool StackReplace(const void* item, int index, Stack* stack)
{
	return VectorReplace(item, index, stack->vect);
}

void* StackPeek(Stack* stack)
{
	// Return the item at the end of the vector.
	return VectorGet(VectorCount(stack->vect) - 1, stack->vect);
}

void* StackGet(int index, Stack* stack)
{
	return VectorGet(index, stack->vect);
}

void StackClear(Stack* stack)
{
	VectorClear(stack->vect);
}