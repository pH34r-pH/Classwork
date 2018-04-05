/**
 * @file CircularBufferTest.c
 */

#include "CircularBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void CircularBufferShouldHaveNoDataAfterBeingCleared()
{
    CircularBuffer buffer;
    CircularBufferClear(&buffer);
    
    assert(CircularBufferCount(&buffer) == 0);
}

void CircularBufferShouldReportEmptyWhenZeroBytesInBuffer()
{
    CircularBuffer buffer;
    CircularBufferClear(&buffer);

    for (unsigned int i = 0; i < 10; i++)
    {
        CircularBufferAddByte('A', &buffer);
    }
    for (unsigned int i = 0; i < 10; i++)
    {
        CircularBufferGetByte(&buffer);
    }

    assert(CircularBufferIsEmpty(&buffer));
}

void CircularBufferSizeShouldEqualNumberOfBytesInBuffer()
{
    CircularBuffer buffer;
    CircularBufferClear(&buffer);

    for (unsigned int i = 0; i < 10; i++)
    {
        CircularBufferAddByte('B', &buffer);
    }

    assert(CircularBufferCount(&buffer) == 10);
}

void CircularBufferShouldAddAndRemoveInQueueOrder()
{
    static unsigned char bufferContents[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    CircularBuffer buffer;
    CircularBufferClear(&buffer);

    for (unsigned int i = 0; i < 7; i++)
    {
        CircularBufferAddByte(bufferContents[i], &buffer);
    }

    for (unsigned int i = 0; i < 7; i++)
    {
        unsigned char letter = CircularBufferGetByte(&buffer);
        assert(letter == bufferContents[i]);
    }
}

void CircularBufferShouldOverwriteOldestValuesWhenAddingToFullBuffer()
{
    CircularBuffer buffer;
    CircularBufferClear(&buffer);

    for (unsigned int i = 0; i < CIRCULAR_BUFFER_CAPACITY_BYTES; i++)
    {
        CircularBufferAddByte('A', &buffer);
    }

    CircularBufferAddByte('B', &buffer);

    assert(CircularBufferGetByte(&buffer) == 'B');
}

int main()
{
    printf("Running Circular Buffer tests using buffer of size %i bytes.\n", CIRCULAR_BUFFER_CAPACITY_BYTES);
    
    // Execute all tests.
    CircularBufferShouldHaveNoDataAfterBeingCleared();
    CircularBufferShouldReportEmptyWhenZeroBytesInBuffer();
    CircularBufferSizeShouldEqualNumberOfBytesInBuffer();
    CircularBufferShouldAddAndRemoveInQueueOrder();
    CircularBufferShouldOverwriteOldestValuesWhenAddingToFullBuffer();

    printf("All Circular Buffer tests passed.\n");
    return 0;
}