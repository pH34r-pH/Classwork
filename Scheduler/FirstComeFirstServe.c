/**
 * @file FirstComeFirstServe.c
 */

#include "FirstComeFirstServe.h"
#include "ScheduleFile.h"
#include <stdlib.h>
#include <stdio.h>

bool RunProcess (ScheduleData* inputData){
	FILE *out = fopen("processes.out", "w");
	int numProc = VectorCount(inputData->processes);
	fprintf(out, "%d processes\nUsing First Come First Serve\n\n", numProc);

	/*
	* How-to:
	* Processes are completed in the order they are received.
	*/
	Vector* Queue;
	for (int clock = 0; clock < inputData->runLength; ++clock) 
	{
		// Iterate through processes to check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			if (inputData->processes[i]->arrivalTime == clock) {
				fprintf(out, "Time %d: %s arrived\n", clock, inputData->processes[i]->processName);
				VectorAdd(inputData->processes[i], Queue);
			}
		}
		// Continue working though whatever is at index 0 of the Queue, unless the Queue has nothing in it 
		// or we are out of time.

		// Update everything else to increment wait time

	}

	fclose(out);
    return true;
}