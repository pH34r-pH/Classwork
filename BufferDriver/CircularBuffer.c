/**
 * @file CircularBuffer.c
 */

#include "CircularBuffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool CircularBufferCreate(CircularBuffer* circBuffer)
{
	assert(circBuffer != NULL);

	// First, reset the CircularBuffer object so all values are zeroed.
	memset(circBuffer, 0, sizeof(CircularBuffer));

	return true;
}

void CircularBufferClear(CircularBuffer* circBuffer)
{
	circBuffer->head = circBuffer->tail = 0;
	circBuffer->isFull = false;
}

void CircularBufferAddByte(unsigned char byte, CircularBuffer* circBuffer)
{
	assert(circBuffer != NULL);

	// Add in the new byte.
	circBuffer->buffer[circBuffer->head] = byte;

	// Move the head forward, wrapping to the front if necessary.
	circBuffer->head = (circBuffer->head + 1) % CIRCULAR_BUFFER_CAPACITY_BYTES;

	// If the head coincides with the tail, the buffer has become full.
	if (circBuffer->head == circBuffer->tail)
	{
		circBuffer->isFull = true;
	}
}

unsigned char CircularBufferGetByte(CircularBuffer* circBuffer)
{
	assert(circBuffer != NULL);

	// Must not attempt to get a byte when the buffer is empty.
	assert(!CircularBufferIsEmpty(circBuffer));

	// Get the byte to return.
	unsigned char byte = circBuffer->buffer[circBuffer->tail];

	// Move the tail forward, wrapping around if necessary.
	circBuffer->tail = (circBuffer->tail + 1) % CIRCULAR_BUFFER_CAPACITY_BYTES;

	// If the buffer was full, it isn't anymore.
	circBuffer->isFull = false;

	// Return the retrieved byte.
	return byte;
}

unsigned short CircularBufferCount(CircularBuffer* circBuffer)
{
	assert(circBuffer != NULL);

	// Return the amount of data that is available.
	if (circBuffer->head > circBuffer->tail)
	{
		// If head > tail, we return head - tail so we get the amount of data between the two variables.
		return circBuffer->head - circBuffer->tail;
	}
	else if (circBuffer->tail > circBuffer->head)
	{
		// If tail > head, the data has wrapped around the array. We want the data after tail to the end of the array, and
		// then the data from the beginning of the array to head.
		return CIRCULAR_BUFFER_CAPACITY_BYTES - circBuffer->tail + circBuffer->head;
	}
	else
	{
		// The buffer is either full or empty.
		return circBuffer->isFull ? CIRCULAR_BUFFER_CAPACITY_BYTES : 0;
	}

	// Unnecessary return to remove GCC warning about control reaching end of non-void function. It's not actually
	// possible to reac this part of the function.
	return 0;
}

bool CircularBufferIsEmpty(CircularBuffer* circBuffer)
{
	assert(circBuffer != NULL);

	if (circBuffer->head == circBuffer->tail && circBuffer->isFull == false)
	{
		return true;
	}

	return false;
}

bool CircularBufferIsFull(CircularBuffer* circBuffer)
{
	assert(circBuffer != NULL);

	return circBuffer->isFull;
}