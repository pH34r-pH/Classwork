cmake_minimum_required(VERSION 2.8.4)

# This project contains various types such as stacks and vectors.
project(CLibExtensions)

# Create a list of source files.
set(src_files
        src/HashTable.c
        src/LinkedList.c
        src/Stack.c
        src/Vector.c
        src/UtilityFunctions.c)

# Create a list of header files.
set(hdr_files
        include/CLib/CLibraryExtensions.h
        include/CLib/CLibraryHeaders.h
        include/CLib/HashTable.h
        include/CLib/LinkedList.h
        include/CLib/Stack.h
        include/CLib/Vector.h)

# Include source and header files into a library. Force static linking since that will
# keep things simple.
add_library(CLibExtensions STATIC ${src_files} ${hdr_files})

# Add the include/CLib directory as a public include path associated with the CLibExtensions target.
target_include_directories(CLibExtensions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)