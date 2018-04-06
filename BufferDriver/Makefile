# Add other object files to this list, separated by spaces.
ifdef compile_write_driver
	obj-m += BufferDriverWriter.o
	BufferDriverWriter-objs := CircularBuffer.o BufferDriverWriterMain.o
endif
ifdef compile_read_driver
	obj-m += BufferDriverReader.o
	BufferDriverReader-objs := CircularBuffer.o BufferDriverReaderMain.o
endif

# Compiler variables.
# CC = gcc
# CFLAGS = -std=c99 -g
# CFLAGS_LINK = -static

# Set CFlags such that they don't interfere with the internal Makefile declarations of the module stuff.
ccflags-y := -std=gnu99

# Command line variables.
ifdef CIRCULAR_BUFFER_CAPACITY
	CFLAGS += -DCIRCULAR_BUFFER_CAPACITY_BYTES=$(CIRCULAR_BUFFER_CAPACITY)
endif

# Other variables.
TEST_BINARY_NAME = CircularBufferTest
OBJ_PATH = obj/
BIN_PATH = bin/

all:
ifeq ($(compile_write_driver), TRUE)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
else ifeq ($(compile_read_driver), TRUE)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
else	
	make compile_write_driver=TRUE
	make compile_read_driver=TRUE
endif

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

circ_buffer_test:
	@mkdir -p $(OBJ_PATH) # Create the object directory if it doesn't currently exist.	
	$(CC) $(CFLAGS) -c CircularBuffer.c -o $(OBJ_PATH)CircularBuffer.o
	$(CC) $(CFLAGS) -c CircularBufferTest.c -o $(OBJ_PATH)CircularBufferTest.o
	@mkdir -p $(BIN_PATH) # Create the binary directory if it doesn't currently exist.
	$(CC) $(CFLAGS_LINK) $(OBJ_PATH)CircularBuffer.o $(OBJ_PATH)CircularBufferTest.o -o $(BIN_PATH)$(TEST_BINARY_NAME)