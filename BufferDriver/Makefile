# Change the name of this if the main object file isn't called "BufferDriver.o".
obj-m += BufferDriver.o
# Add other object files to this list, separated by spaces.
BufferDriver-objs := CircularBuffer.o

# Compiler variables.
CC = gcc
CFLAGS = -std=c99
CFLAGS_LINK = -static

# Other variables.
TEST_BINARY_NAME = CircularBufferTest
OBJ_PATH = obj/
BIN_PATH = bin/

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shall uname -r)/build M=$(PWD) clean

circ_buffer_test:
	@mkdir -p $(OBJ_PATH) # Create the object directory if it doesn't currently exist.	
	$(CC) $(CFLAGS) -c CircularBuffer.c -o $(OBJ_PATH)CircularBuffer.o
	$(CC) $(CFLAGS) -c CircularBufferTest.c -o $(OBJ_PATH)CircularBufferTest.o
	@mkdir -p $(BIN_PATH) # Create the binary directory if it doesn't currently exist.
	$(CC) $(CFLAGS_LINK) $(OBJ_PATH)CircularBuffer.o $(OBJ_PATH)CircularBufferTest.o -o $(BIN_PATH)$(TEST_BINARY_NAME)