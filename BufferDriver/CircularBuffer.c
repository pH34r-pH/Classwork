/**
 * @file CircularBuffer.c
 */

#include "CircularBuffer.h"
#include <linux/bug.h>
#include <linux/string.h>

void CircularBufferClear(CircularBuffer* circBuffer)
{
	circBuffer->head = circBuffer->tail = 0;
	circBuffer->isEmpty = true;
    memset(circBuffer->buffer, 0, CIRCULAR_BUFFER_CAPACITY_BYTES);
}

void CircularBufferAddByte(unsigned char byte, CircularBuffer* circBuffer)
{
	BUG_ON(circBuffer == NULL);

	// Add in the new byte.
	circBuffer->buffer[circBuffer->head] = byte;

	// Move the head forward, wrapping to the front if necessary.
	circBuffer->head = (circBuffer->head + 1) % CIRCULAR_BUFFER_CAPACITY_BYTES;

    // By adding a byte, we're definitely no longer empty.
    circBuffer->isEmpty = false;
}

unsigned char CircularBufferGetByte(CircularBuffer* circBuffer)
{
	unsigned char byte;

	BUG_ON(circBuffer == NULL);

	// Must not attempt to get a byte when the buffer is empty.
	BUG_ON(CircularBufferIsEmpty(circBuffer));

	// Get the byte to return.
	byte = circBuffer->buffer[circBuffer->tail];

	// Move the tail forward, wrapping around if necessary.
	circBuffer->tail = (circBuffer->tail + 1) % CIRCULAR_BUFFER_CAPACITY_BYTES;

	// The buffer is empty if head equals tail and isEmpty is false.
    if (circBuffer->tail == circBuffer->head && !circBuffer->isEmpty)
    {
        circBuffer->isEmpty = true;
    }

	// Return the retrieved byte.
	return byte;
}

void CircularBufferGetBytes(unsigned short size, CircularBufferPointer pointers[2], CircularBuffer* circBuffer)
{
	BUG_ON(circBuffer == NULL);

	// If the specified size is 0, set both pointer reference objects to 0.
	if (size == 0)
	{
		memset(pointers, 0, sizeof(CircularBufferPointer) * 2);
		return;
	}	

	// The amount of data requested must be less than or equal to the amount of data available.
	BUG_ON(size > CircularBufferCount(circBuffer));

	// In order to get the amount of data requested, will wrapping be necessary?
	if (circBuffer->tail + size > CIRCULAR_BUFFER_CAPACITY_BYTES)
	{
		// We will need to perform a wrap. The first pointer will represent all data to the end of the array.
		pointers[0].bufferPointer = circBuffer->buffer + circBuffer->tail;
		pointers[0].length = circBuffer->tail + size - CIRCULAR_BUFFER_CAPACITY_BYTES;

		// The second pointer is the remainder of the "contiguous" data, starting from the beginning of the buffer.
		pointers[1].length = ((circBuffer->tail + size) % CIRCULAR_BUFFER_CAPACITY_BYTES) - 1;
		pointers[1].bufferPointer = circBuffer->buffer;
		
		circBuffer->tail = pointers[1].length;
	}
	else
	{
		// A wrap around is not necessary. The second pointer should have its size set to 0.
		pointers[1].bufferPointer = NULL;
		pointers[1].length = 0;

		// The first pointer should have the appropriate information set.
		pointers[0].bufferPointer = circBuffer->buffer + circBuffer->tail + size;
		pointers[0].length = size;

		// Move the tail forward.
		circBuffer->tail += size;
	}

	// The buffer is empty if head equals tail and isEmpty is false.
    if (circBuffer->tail == circBuffer->head && !circBuffer->isEmpty)
    {
        circBuffer->isEmpty = true;
    }
}

unsigned short CircularBufferCount(CircularBuffer* circBuffer)
{
	BUG_ON(circBuffer == NULL);

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
		return circBuffer->isEmpty ? 0 : CIRCULAR_BUFFER_CAPACITY_BYTES;
	}

	// Unnecessary return to remove GCC warning about control reaching end of non-void function. It's not actually
	// possible to reach this part of the function.
	return 0;
}

int CircularBufferIsEmpty(CircularBuffer* circBuffer)
{
	BUG_ON(circBuffer == NULL);

	return circBuffer->isEmpty;
}

int CircularBufferIsFull(CircularBuffer* circBuffer)
{
    BUG_ON(circBuffer == NULL);

    // The buffer is full if isEmpty is false and the head is greater than (overflowed) or equal to (full) the tail.
    if (!circBuffer->isEmpty && circBuffer->head == circBuffer->tail)
    {
        return true;
    }

    return false;
}
