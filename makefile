# Makefile for our COP4600 assignments.

# Compile variables.
CC = gcc
CFLAGS = -std=c99

# The paths to the various parts of the overall "solution"
CLIBEXT_PATH = CLibExtensions/
SCHEDULER_PATH = Scheduler/
OS_BIN_PATH = Output/

# Move the executables into the COP4600 root directory.
OS: SCHEDULER CLIB
	mkdir -p $(OS_BIN_PATH)
	echo "Copying Scheduler into output folder."
	cp -r $(SCHEDULER_PATH)bin/* $(OS_BIN_PATH)

# Compile the Scheduler project.
SCHEDULER: CLIB
	echo "Compiling the Scheduler project."
	$(MAKE) -C $(SCHEDULER_PATH)

# Compile the CLibraryExtensions library. It is needed by other executables.
CLIB:
	echo "Compiling the C Library Extensions library."
	$(MAKE) -C $(CLIBEXT_PATH)

clean:
	$(MAKE) -C $(CLIBEXT_PATH) clean
	$(MAKE) -C $(SCHEDULER_PATH) clean