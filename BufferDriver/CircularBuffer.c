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
	BUG_ON(circBuffer == NULL);

	// Must not attempt to get a byte when the buffer is empty.
	BUG_ON(CircularBufferIsEmpty(circBuffer));

	// Get the byte to return.
	unsigned char byte = circBuffer->buffer[circBuffer->tail];

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
    if (!circBuffer->isEmpty && circBuffer->head >= circBuffer->tail)
    {
        return true;
    }

    return false;
}
