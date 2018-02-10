/**
 * @file FirstComeFirstServe.c
 */

#include "FirstComeFirstServe.h"
#include "ScheduleFile.h"
#include <stdlib.h>
#include <stdio.h>

void RunProcess (ScheduleData* inputData){
	FILE *out = fopen("processes.out", "w");
	int numProc = VectorCount(inputData.processes);
	fprintf(out, "%d processes\nUsing First Come First Serve\n\n", numProc);

	/*
	* How-to:
	* Processes are completed in the order they are received.
	*/

	InputProcess queue[numProc];
	int head, tail, clock;
	head = tail = clock = 0;
	int running = 0;

	for (; clock < inputData.runLength; ++clock) 
	{
		// Iterate through processes to check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			if (inputData.processes[i]->arrivalTime == clock) {
				fprintf(out, "Time %d: %s arrived\n", clock, inputData.processes[i].processName);
				VectorAdd(inputData.processes[i], queue);
			}
		}
		// Check if we have at least one ready process
		if(queue[head] != NULL){
			// Check if we have a process running, or select a new process
			if(running == 0){
				fprintf(out, "Time %d: %s selected (burst %d)\n", clock, queue[head].processName, queue[head].remainingTime);
				running = 1;
			}

			// Update process counters
			queue[head].remainingTime -= 1;
			queue[head].turnaroundTime += 1;

			for(int i = head+1; i < tail; ++i){
				queue[i].turnaroundTime += 1;
				queue[i].waitingTime += 1;
			}
			
			// Check if head has finished
			if(queue[head].remainingTime == 0){
				fprintf(out, "Time %d: %s finished\n", clock, queue[head].processName);
				running = 0;
				++head;
			}
		}
		// No ready processes means we're Idle
		else {
			fprintf(out, "Time %d: Idle", clock);
		}				

		// Show results
		fprintf(out, "Finished at time %d\n\n", clock);
		for(int i = 0; i < tail; ++i){
			fprintf(out, "%s wait %d turnaround %d\n", queue[i].processName, queue[i].waitingTime, queue[i].turnaroundTime);
		}

	}

	fclose(out);
    return true;
}
