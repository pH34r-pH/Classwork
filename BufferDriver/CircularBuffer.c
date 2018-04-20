/**
 * @file CircularBuffer.c
 */

#include "CircularBuffer.h"
#include <linux/kernel.h>
#include <linux/bug.h>
#include <linux/string.h>

#define UCF_MESSAGE_LENGTH 38
static const unsigned char* ucfReplaceMessage = "Undefeated 2018 National Champions UCF";

static short MathMod(short x, short m)
{
	short r = x % m;
	return r < 0 ? r + m : r;
}

void CircularBufferClear(CircularBuffer* circBuffer)
{
	BUG_ON(circBuffer == NULL);

	// Initialize the mutex. Note that this should be called ONLY by whichever driver "owns" the
	// CircularBuffer.
	mutex_init(&circBuffer->bufferMutex);

	circBuffer->head = circBuffer->tail = 0;
	circBuffer->isEmpty = true;
    memset(circBuffer->buffer, 0, CIRCULAR_BUFFER_CAPACITY_BYTES);
}

static void CircularBufferPerformAddByte(unsigned char byte, CircularBuffer* circBuffer)
{
	BUG_ON(circBuffer == NULL);

	// Add in the new byte.
	circBuffer->buffer[circBuffer->head] = byte;

	// Move the head forward, wrapping to the front if necessary.
	circBuffer->head = (circBuffer->head + 1) % CIRCULAR_BUFFER_CAPACITY_BYTES;

    // By adding a byte, we're definitely no longer empty.
    circBuffer->isEmpty = false;
}

void CircularBufferAddByte(unsigned char byte, CircularBuffer* circBuffer)
{
	static const unsigned char ucfStr[3] = "FCU";
	bool ucfDetected = true;

	BUG_ON(circBuffer == NULL);

	// Lock the mutex before modifying buffer variables.
	mutex_lock(&circBuffer->bufferMutex);

	// If the buffer is full, do nothing.
	if (CircularBufferIsFull(circBuffer))
	{
		return;
	}

	// Add in the current byte.
	CircularBufferPerformAddByte(byte, circBuffer);

	// Check if the byte we just added formed the string "UCF". We do this check using the following logic;
	// we test moving head backwards (including wrapping) and make sure it lands on the expected letter
	// of UCF. If we ever land on the same position as tail, that means we would pass tail (which is the
	// boundary of where the circular array ends). This does not apply to the last character that we're
	// checking (the U).
	for (int i = 1; i <= 3; i++)
	{
		unsigned short pos = MathMod(((short)circBuffer->head) - i, CIRCULAR_BUFFER_CAPACITY_BYTES);
		if ((pos == circBuffer->tail && i != 3) || circBuffer->buffer[pos] != ucfStr[i - 1])
		{
			ucfDetected = false;
			break;
		}
	}

	if (ucfDetected)
	{
		// We've formed the string "UCF" and we need to replace it with the replacement string.
		// printk("Detected the string 'UCF'. Replacing with replacement string.\n");

		// First, move head back three positions so it is on top of the U in "UCF".
		circBuffer->head = MathMod(((short)circBuffer->head) - 3, CIRCULAR_BUFFER_CAPACITY_BYTES);

		// Add bytes in the replacement string until we've consumed the replacement string or we
		// run out of room in the buffer.
		for (unsigned int i = 0; i < UCF_MESSAGE_LENGTH; i++)
		{
			// We ignore CircularBufferIsFull when i = 0 because head may be on top of tail to get
			// ready to start replacing everything. isEmpty was already set to false, so that creates
			// the conditions of being full, even though we aren't. We must write at least one byte
			// before we care about whether or not CircularBufferIsFull returns ture.
			if (i != 0 && CircularBufferIsFull(circBuffer))
			{
				break;
			}

			// printk(KERN_INFO "Adding %c of replacement string into buffer.\n", ucfReplaceMessage[i]);
			CircularBufferPerformAddByte(ucfReplaceMessage[i], circBuffer);
		}
	}

	// Unlock the mutex since we're done modifying buffer variables.
	mutex_unlock(&circBuffer->bufferMutex);
}

unsigned char CircularBufferGetByte(CircularBuffer* circBuffer)
{
	unsigned char byte;

	BUG_ON(circBuffer == NULL);

	// Must not attempt to get a byte when the buffer is empty.
	BUG_ON(CircularBufferIsEmpty(circBuffer));

	// Lock the mutex before modifying buffer variables.
	mutex_lock(&circBuffer->bufferMutex);

	// Get the byte to return.
	byte = circBuffer->buffer[circBuffer->tail];

	// Move the tail forward, wrapping around if necessary.
	circBuffer->tail = (circBuffer->tail + 1) % CIRCULAR_BUFFER_CAPACITY_BYTES;

	// The buffer is empty if head equals tail and isEmpty is false.
    if (circBuffer->tail == circBuffer->head && !circBuffer->isEmpty)
    {
        circBuffer->isEmpty = true;
    }

	// Unlock the mutex since we're done modifying buffer variables.
	mutex_unlock(&circBuffer->bufferMutex);

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

	// Lock the mutex before modifying buffer variables.
	mutex_lock(&circBuffer->bufferMutex);

	// In order to get the amount of data requested, will wrapping be necessary?
	if (circBuffer->tail + size > CIRCULAR_BUFFER_CAPACITY_BYTES)
	{
		// We will need to perform a wrap. The first pointer will represent all data to the end of the array.
		pointers[0].bufferPointer = circBuffer->buffer + circBuffer->tail;
		pointers[0].length = CIRCULAR_BUFFER_CAPACITY_BYTES - circBuffer->tail;

		// The second pointer is the remainder of the "contiguous" data, starting from the beginning of the buffer.
		pointers[1].length = ((circBuffer->tail + size) % CIRCULAR_BUFFER_CAPACITY_BYTES);
		pointers[1].bufferPointer = circBuffer->buffer;
		
		// Move the tail forward by the correct amount from the start of the array (since it wrapped around).
		circBuffer->tail = pointers[1].length;
	}
	else
	{
		// A wrap around is not necessary. The second pointer should have its size set to 0.
		pointers[1].bufferPointer = NULL;
		pointers[1].length = 0;

		// The first pointer should have the appropriate information set.
		pointers[0].bufferPointer = circBuffer->buffer + circBuffer->tail;
		pointers[0].length = size;

		// Move the tail forward.
		circBuffer->tail += size;
	}

	// The buffer is empty if head equals tail and isEmpty is false.
    if (circBuffer->tail == circBuffer->head && !circBuffer->isEmpty)
    {
        circBuffer->isEmpty = true;
    }

	// Unlock the mutex since we're done modifying buffer variables.
	mutex_unlock(&circBuffer->bufferMutex);
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
