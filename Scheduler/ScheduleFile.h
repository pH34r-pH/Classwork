/**
 * @file ScheduleFile.c
 */

#pragma once

#include <CLib/Vector.h>
#include <stdbool.h>

/**
 * Represents the different scheduler types.
 */
typedef enum tagSchedulerType
{
    /**
     * First-Come First-Served Scheduler
     */
    FCFS = 0,

    /**
     * Preemptive Shortest Job First Scheduler.
     */
    ShortestJob = 1,

    /**
     * Round-Robin Scheduler
     */
    RoundRobin = 2
} SchedulerType;

/**
 * Represents information on a process read from the "processes.in" file.
 */
typedef struct tagInputProcess
{
    /**
     * The name of the process, stored as a standard C String.
     */
    char processName[128];

    /**
     * The total length of time that the process needs to run.
     */
    unsigned int burstLength;

    /**
     * The time at which this process arrived.
     */
    unsigned int arrivalTime;
} InputProcess;

/**
 * Contains all the data read in from the "processes.in" file.
 */
typedef struct tagScheduleData
{
    /**
     * The type of scheduler specified in the input file.
     */
    SchedulerType schedulerType;

    /**
     * The time quantum value for the Round-Robin algorithm. Set to 0 if the
     * algorithm is not Round-Robin.
     */
    unsigned int timeQuantum;

    /**
     * The number of time units that the scheduler should run for.
     */
    unsigned int runLength;

    /**
     * The list of InputProcess objects that were retrieved from the file.
     */
    Vector* processes;
} ScheduleData;

/**
 * Reads the "processes.in" file, parses it, and updates the ScheduleData object with the data parsed from the
 * processes file. A call to DestroyScheduleFile is necessary after use to ensure dynamically allocated process
 * memory is freed.
 *
 * @param file The ScheduleData object to update with the data obtained from the "processes.in" file.
 * @return True if the operation succeeds, otherwise false.
 */
bool ReadScheduleFile(ScheduleData* file);