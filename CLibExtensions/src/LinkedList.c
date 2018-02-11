#include "CLib/LinkedList.h"
#include <stdlib.h>
#include <string.h>

// Forward declare the LLNode, so the typedef can be used within the structure.
typedef struct tagLLNode LLNode;

typedef struct tagLLNode
{
	// The data that this node holds.
	void* data;
	// The next node that this node points to.
	LLNode* next;
} LLNode;

typedef struct tagLinkedList
{
	// Always points to the first node in the list.
	LLNode* head;
	// Always points to the last node in the list.
	LLNode* tail;
	// The number of items contained in the Linked List.
	int numItems;
	// The size of the datatype stored in this LinkedList.
	int dataTypeSize;
} LinkedList;

bool LinkedListCreate(int dataTypeSize, LinkedList** list)
{
	(*list) = calloc(1, sizeof(LinkedList));
	if ((*list) == NULL)
		return false;
	(*list)->dataTypeSize = dataTypeSize;
	return true;
}

void LinkedListDestroy(LinkedList** list)
{
	// Clear the Linked List.
	LinkedListClear((*list));

	// Destroy the list object.
	free((*list));
}

bool LinkedListPush(const void* value, LinkedList* list)
{
	// Add a new node at the end of the list.
	LLNode* node = calloc(1, sizeof(LLNode));
	if (node == NULL)
		return false;
	node->data = calloc(1, list->dataTypeSize);
	if (node->data == NULL)
		return false;
	memcpy(node->data, value, list->dataTypeSize);

	if (list->head == NULL)
	{
		list->head = list->tail = node;
	}
	else
	{
		// Before this node becomes the last item, make what will become the second to last item point to this node,
		// making it the last item.
		list->tail->next = node;
		// Set tail to this new node.
		list->tail = node;
	}
	
	// The number of items in the linked list goes up.
	list->numItems++;

	return true;
}
#include <stdio.h>
bool LinkedListInsert(const void* value, int index, LinkedList* list)
{
	// Index must be valid.
	if (index < 0 || index > list->numItems - 1)
		return false;

	// Just call LinkedListPush if the index is the end of the list.
	if (index == list->numItems)
		return LinkedListPush(value, list);

	// Create the new node.
	LLNode* newNode = calloc(1, sizeof(LLNode));
	newNode->data = calloc(1, list->dataTypeSize);
	memcpy(newNode->data, value, list->dataTypeSize);

	if(index == 0)
	{
		// The newNode is being inserted as the head node (being inserted as the tail is handled
		// by LinkedListPush call). Make this new node the head and make it point to the old head
		newNode->next = list->head;
		list->head = newNode;
	}
	else
	{
		// Find the place to insert the node.
		LLNode* iterNode = list->head;
		int currentIndex = 0;
		while(currentIndex != index - 1)
		{
			iterNode = iterNode->next;
			currentIndex++;
		}
		// Make the new node point to what the current node points at, and make the current node
		// point at the new node.
		newNode->next = iterNode->next;
		iterNode->next = newNode;
	}

	// The number of items has gone up by 1.
	list->numItems++;

	return true;
}

bool LinkedListPopInternal(bool eraseData, void** unfreedData, LinkedList* list)
{
	// Do nothing if there are no items to pop.
	if (list->numItems == 0)
		return false;

	// The item at the end of the linked list needs to be removed.
	if (list->numItems == 1)
	{
		// Destroy the node's data, if eraseData is true. Otherwise, output the unfreed data.
		if (eraseData)
			free(list->head->data);
		else
			(*unfreedData) = list->head->data;
		free(list->head);
		list->head = list->tail = NULL;
	}
	else
	{
		LLNode* doomedNode = list->tail;

		// In order to restore the tail pointer, have the walk the list to find the current second to last node.
		LLNode* iterNode = list->head;
		while (iterNode->next != doomedNode)
			iterNode = iterNode->next;

		// iterNode is now the second to last node, and becomes the new tail pointer.
		list->tail = iterNode;

		// Destroy the doomed node's data, if eraseData is true. If not erasing the data, output the unfreed data.
		if (eraseData)
			free(doomedNode->data);
		else
			(*unfreedData) = doomedNode->data;
		// Destroy the doomed node.
		free(doomedNode);
	}

	// The number of items in the list goes down by 1.
	list->numItems--;

	return true;
}

bool LinkedListPop(LinkedList* list)
{
	return LinkedListPopInternal(true, NULL, list);
}

void* LinkedListPopSafe(LinkedList* list)
{
	void* unfreedData;
	if (LinkedListPopInternal(false, &unfreedData, list))
		return unfreedData;
	return NULL;
}

bool LinkedListRemoveInternal(int index, bool eraseData, void** unfreedData, LinkedList* list)
{
	// Can't remove from an empty list.
	if (list->numItems == 0)
		return false;
	// The index must be valid.
	if (index < 0 || index > list->numItems - 1)
		return false;

	// If the index is the last item in the list, just call the LinkedListPop method.
	if (index == list->numItems - 1)
		return LinkedListPop(list);
	// If the index is the first item in the list, just update the head pointer.
	if (index == 0)
	{
		LLNode* newHead = list->head->next;
		// Free the data of the node is eraseData is true, otherwise output the data.
		if (eraseData)
			free(list->head->data);
		else
			(*unfreedData) = list->head->data;
		free(list->head);
		list->head = newHead;
	}
	else
	{
		// Need to navigate to the doomed node.
		LLNode* doomedNode = list->head;
		LLNode* doomedNodeLeft = NULL;
		int currentIndex = 0;
		while (currentIndex != index)
		{
			doomedNodeLeft = doomedNode;
			doomedNode = doomedNode->next;
			currentIndex++;
		}

		// Connect doomedNodeLeft to the next pointer of the current doomed node.
		doomedNodeLeft->next = doomedNode->next;

		// Free the data of the node if eraseData is true, otherwise, output the data.
		if (eraseData)
			free(doomedNode->data);
		else
			(*unfreedData) = doomedNode->data;
		// Destroy the node.
		free(doomedNode);
	}

	// The number of nodes has gone down.
	list->numItems--;
	return true;
}

bool LinkedListRemove(int index, LinkedList* list)
{
	return LinkedListRemoveInternal(index, true, NULL, list);
}

void* LinkedListRemoveSafe(int index, LinkedList* list)
{
	void* unfreedData;
	if (LinkedListRemoveInternal(index, false, &unfreedData, list))
		return unfreedData;
	return NULL;
}

void LinkedListClear(LinkedList* list)
{
	// Walk through the list, freeing the data of each node.
	if (list->head != NULL)
	{
		LLNode* currentNode = list->head;
		while (currentNode != NULL)
		{
			free(currentNode->data);
			// Store this node to be deleted after saving the next node.
			LLNode* doomedNode = currentNode;
			currentNode = currentNode->next;
			free(doomedNode);
		}
	}

	// There are now 0 items in the list.
	list->numItems = 0;
}

int LinkedListCount(LinkedList* list)
{
	return list->numItems;
}

bool LinkedListIsEmpty(LinkedList* list)
{
	return (list->numItems == 0);
}

void* LinkedListGet(int index, LinkedList* list)
{
	// The index must be valid.
	if (index < 0 || index > list->numItems - 1)
		return NULL;

	// Iterate the list until the given index is reached.
	int currentIndex = 0;
	LLNode* iterNode = list->head;
	while(currentIndex != index)
	{
		iterNode = iterNode->next;
		currentIndex++;
	}

	return iterNode->data;
}

void* LinkedListGetFront(LinkedList* list)
{
	if (list->numItems == 0)
		return NULL;

	return list->head->data;
}

void* LinkedListGetBack(LinkedList* list)
{
	if (list->numItems == 0)
		return NULL;

	return list->tail->data;
}