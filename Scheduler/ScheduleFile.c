/**
 * @file ScheduleFile.c
 */

#include "ScheduleFile.h"
#include <stdio.h>

bool ReadScheduleFile(ScheduleData* fileData)
{
    // Open the processes.in file.
    FILE* file = fopen("processes.in", "r");
    if (file == NULL)
    {
        printf("Failed to open the 'processes.in' file. Are you sure it's in the same directory as the executable?\n");
        return false;
    }

    // Read the input line by line.

    return true;
}