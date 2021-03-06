/**
 * @file CircularBuffer.h
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef CIRCULAR_BUFFER_CAPACITY_BYTES
#define CIRCULAR_BUFFER_CAPACITY_BYTES 1024
#else
#if CIRCULAR_BUFFER_CAPACITY_BYTES < 1024
#error When 'CIRCULAR_BUFFER_CAPACITY_BYTES' is specified, it must be a value greater\
 than or equal to 1024.
#endif
#endif

/**
 * @brief Represents a Circular Buffer of a fixed size.
 *
 * A circular buffer acts the way the name describes; it's a buffer of fixed size that allows data
 * to be added and read from it arbitrarily. The data can wrap around the bounds of the buffer.
 * Normally we would hide the implementation details (the actual struct) in the C file, but to avoid
 * extra mallocs, we make the internals of the struct available to clients.
 */
typedef struct
{
	/**
	 * The head of the circular buffer.
	 */
	unsigned short head;
	/**
	 * The tail of the circular buffer.
	 */
	unsigned short tail;
	/**
	 * The circular buffer itself.
	 */
	unsigned char buffer[CIRCULAR_BUFFER_CAPACITY_BYTES];
	/**
	 * Whether or not the circular buffer is currently empty.
	 */
	bool isEmpty;
} CircularBuffer;

/**
 * Clears the circular buffer, emptying it of contents. This doubles as initialization.
 * @param buffer The CircularBuffer object to initialize.
 */
void CircularBufferClear(CircularBuffer* buffer);

/**
 * Adds a single byte to the specified circular buffer. Note that if the buffer is full when this
 * is done, the oldest data will be overwritten, and the buffer will continue being considered full
 * until the CircularBufferGetByte function is called.
 *
 * @param byte The byte to add to the circular buffer.
 * @param buffer The CircularBuffer object to use.
 */
void CircularBufferAddByte(unsigned char byte, CircularBuffer* buffer);

/**
 * Gets the next byte from the specified circular buffer if it isn't empty. This
 * will remove the byte from the buffer.
 * @param buffer The CircularBuffer object to use.
 * @return The next available byte in the circular buffer.
 */
unsigned char CircularBufferGetByte(CircularBuffer* buffer);

/**
 * Gets the amount of data that is currently in the buffer.
 * @param buffer The CircularBuffer object to use.
 * @return The number of bytes that are currently in the buffer.
 */
unsigned short CircularBufferCount(CircularBuffer* buffer);

/**
 * Gets whether or not the circular buffer is empty.
 * @param buffer The CircularBuffer object to use.
 * @return True if the buffer is empty, otherwise false.
 */
bool CircularBufferIsEmpty(CircularBuffer* buffer);

/**
 * Gets whether or not the circular buffer is full.
 * @param buffer The CircularBuffer object to use.
 * @return True if the buffer is full, otherwise false.
 */
bool CircularBufferIsFull(CircularBuffer* buffer);