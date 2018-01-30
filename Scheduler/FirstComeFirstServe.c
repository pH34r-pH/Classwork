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
	for (int clock = 0; clock < inputData->runLength; ++clock) 
	{
		
	}

	fclose(out);
    return true;
}