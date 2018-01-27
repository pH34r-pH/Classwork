#include "ScheduleFile.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Read in the input scheduling data.
    ScheduleData inputData;
    bool result = ReadScheduleFile(&inputData);
    if (!result)
    {
        // Failed to read in the input schedule file.
        printf("Failed to read the input schedule file.\n");
        return -1;
    }

    switch (inputData.schedulerType)
    {
        case SchedulerType::FIFO:
            // Call FIFO code.
            break;
        case SchedulerType::ShortestJob:
            // Call Shortest Job code.
            break;
        case SchedulerType::RoundRobin:
            // Call Round Robin code.
            break;
    }

    // Destroy the dynamically allocated list of processes in the ScheduleData.
    free(inputData.processes);

    return 0;
}