cmake_minimum_required(VERSION 2.8.4)

# Declare our project.
project(Scheduling C)

# Create the executable project.
add_executable(Scheduling main.c ScheduleFile.h ScheduleFile.c Scanner.h Scanner.c Lexer.h)

# Link to the CLibExtensions library.
target_link_libraries(Scheduling PRIVATE CLibExtensions)