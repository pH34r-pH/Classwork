/**
 * @file CircularBufferTest.c
 */

#define CIRCULAR_BUFFER_CAPACITY_BYTES 2056

#include "CircularBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void CircularBufferSizeShouldEqualNumberOfBytesInBuffer()
{
    CircularBuffer buffer;
    CircularBufferCreate(&buffer);

    assert(!CircularBufferIsFull(&buffer));
    assert(CircularBufferCount(&buffer) == 0);

    for (unsigned int i = 0; i < 10; i++)
    {
        CircularBufferAddByte('B', &buffer);
    }

    assert(CircularBufferCount(&buffer) == 10);
}

void CircularBufferShouldReportFullWhenFull()
{
    CircularBuffer buffer;
    CircularBufferCreate(&buffer);

    for (unsigned int i = 0; i < CIRCULAR_BUFFER_CAPACITY_BYTES; i++)
    {
        CircularBufferAddByte('A', &buffer);
    }

    assert(CircularBufferIsFull(&buffer));
}

int main()
{
    // Execute all tests.
    CircularBufferSizeShouldEqualNumberOfBytesInBuffer();
    CircularBufferShouldReportFullWhenFull();

    printf("All Circular Buffer tests passed.\n");
    return 0;
}