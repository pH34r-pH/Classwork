# Makefile for CLibraryExtensions

# Include paths.
INC = -Iinclude

# Compiler variables.
CC = gcc
CFLAGS = -std=c99 $(INC)

# The main folders.
SRC_PATH = src/
OBJ_PATH = obj/
LIB_PATH = lib/

# Variables
OBJECTS := $(OBJ_PATH)Vector.o $(OBJ_PATH)Stack.o $(OBJ_PATH)LinkedList.o $(OBJ_PATH)HashTable.o $(OBJ_PATH)UtilityFunctions.o

# Compile the object files into a static library.

$(LIB_PATH)libCExtensions.a: $(OBJECTS)
	@mkdir -p $(LIB_PATH) # Create the library directory if it doesn't currently exist.
	ar rcs $(LIB_PATH)libCExtensions.a $(OBJECTS)

# Compile the source files into object files.

$(OBJ_PATH)%.o : $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH) # Create the object directory if it doesn't currently exist. Note that the "-lm" includes the math library.
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH) $(LIB_PATH)