cmake_minimum_required(VERSION 2.8.4)

# Declare out project.
project(Scheduling C)

# Set the C Standard to 11 to avoid any annoying "old" C related pitfalls (such as having to declare for loop variables
# before the loop.)
set(CMAKE_C_STANDARD 11)

# Create the executable project.
add_executable(Scheduling main.c)