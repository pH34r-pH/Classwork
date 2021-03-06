#include "ScheduleFile.h"
#include "FirstComeFirstServe.h"
#include "RoundRobin.h"
#include "SJF.h"
#include <stdio.h>

void PrintScheduleData(ScheduleData* inputData)
{
    printf("Process file contents.\n\n");
    printf("Process Count: %i\n", VectorCount(inputData->processes));
    printf("Run Time: %i\n", inputData->runLength);
    printf("Time Quantum: %i\n", inputData->timeQuantum);

    switch (inputData->schedulerType)
    {
        case FCFS:
            printf("Scheduler Type: First-Come First-Served\n");
            break;
        case ShortestJob:
            printf("Scheduler Type: Shortest Job First\n");
            break;
        case RoundRobin:
            printf("Scheduler Type: Round-Robin\n");
            break;
    }

    for(int i = 0; i < VectorCount(inputData->processes); i++)
    {
        InputProcess* process = VectorGet(i, inputData->processes);
        printf("Process %i: Name: %s, Arrival Time: %i, Burst Length: %i\n", i + 1, process->processName,
               process->arrivalTime, process->burstLength);
    }
}

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

    // Set this to 0 to disable this code. This is just nice to have to verify the ScheduleData reader is working
    // properly.
#ifdef PRINT_PROCESSES_DATA
    PrintScheduleData(&inputData);
#endif

    switch (inputData.schedulerType)
    {
        case FCFS:
            // Call First-Come First-Served code.
            RunFCFSProcess(&inputData);
            break;
        case ShortestJob:
            // Call Shortest Job First code.
            RunPreemptiveSJFProcess(&inputData);
            break;
        case RoundRobin:
            // Call Round-Robin code.
            RunRRProcess(&inputData);
            break;
    }

    // Destroy the vector of processes.
    VectorDestroy(&inputData.processes);

    return 0;
}
