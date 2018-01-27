cmake_minimum_required(VERSION 2.8.4)

# Declare out project.
project(Scheduling C)

# Create the executable project.
add_executable(Scheduling main.c ScheduleFile.h ScheduleFile.c)

# Uncomment the following line if the Scheduling program needs something from CLibExtensions.
# target_link_libraries(Scheduling PRIVATE CLibExtensions)