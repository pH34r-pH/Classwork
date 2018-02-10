# Makefile for Scheduler

# Include and library paths. Scheduler is dependent on CLibExtensions.
INC = -I../CLibExtensions/include
LIB = -L../CLibExtensions/lib

# Compiler variables.
CC = gcc
CFLAGS = -std=c99 $(INC)
CFLAGS_LINK = -static

# Other variables.
BINARY_NAME = Scheduler
LIBRARIES = CExtensions
OBJ_PATH = obj/
BIN_PATH = bin/
OBJECTS = $(OBJ_PATH)Lexer.o $(OBJ_PATH)Scanner.o $(OBJ_PATH)ScheduleFile.o $(OBJ_PATH)main.o $(OBJ_PATH)FirstComeFirstServe.o

# Command line variables.
ifdef PRINT_PROCESSES_DATA
	CFLAGS += -DPRINT_PROCESSES_DATA
endif

# Compile the object files into an executable file.
$(BIN_PATH)$(BINARY_NAME) : $(OBJECTS)
	@mkdir -p $(BIN_PATH) # Create the binary directory if it doesn't currently exist.
	$(CC) $(CFLAGS_LINK) $(LIB) $(OBJECTS) -l$(LIBRARIES) -lm -o $(BIN_PATH)$(BINARY_NAME)

# Compile the source files into object files.
$(OBJ_PATH)%.o : %.c
	@mkdir -p $(OBJ_PATH) # Create the object directory if it doesn't currently exist.	
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH) $(BIN_PATH)
