cmake_minimum_required(VERSION 2.8.4)

# Declare our project.
project(Scheduling C)

# Create the executable project.
add_executable(Scheduling main.c ScheduleFile.h ScheduleFile.c Scanner.h Scanner.c Lexer.h Lexer.c FirstComeFirstServe.c FirstComeFirstServe.h)

# If a variable called PRINT_PROCESSES_DATA is set, the main method will first print out the processes data that
# was read in before doing anything else. This is a helpful debugging utility.
if (${PRINT_PROCESSES_DATA})
    target_compile_definitions(Scheduling PRIVATE -DPRINT_PROCESSES_DATA)
endif()

# Link to the CLibExtensions library.
target_link_libraries(Scheduling PRIVATE CLibExtensions)