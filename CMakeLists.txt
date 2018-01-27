cmake_minimum_required(VERSION 2.8.4)

# Declare our project.
project(COP4600 C)

# Set the C Standard to 11 to avoid any annoying "old" C related pitfalls
# (such as having to declare for loop variables before the loop.)
set(CMAKE_C_STANDARD 11)

# Add the CLibExtensions project.
add_subdirectory(CLibExtensions)
# Add the scheduling project.
add_subdirectory(Scheduler)