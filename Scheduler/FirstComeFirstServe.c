/**
 * @file FirstComeFirstServe.c
 */

#include "FirstComeFirstServe.h"
#include "ScheduleFile.h"
#include <CLib/Vector.h>
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
<<<<<<< HEAD

	InputProcess Queue[numProc];
	int Head, Tail, clock;
	Head = Tail = clock = 0;
	bool running = false;

	for (; clock < inputData->runLength; ++clock) 
	{
		// Check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			if (inputData->processes[i]->arrivalTime == clock) {
				Queue[Tail] = inputData->processes[i];
				fprintf(out, "Time %d: %s arrived\n", clock, Queue[Tail]->processName);
				++Tail;
			}
		}

		// Check if we have at least one ready process
		if (Queue[Head] != NULL) {
			// Check if we have a process running, or select a new process
			if (!running) {
				fprintf(out, "Time %d: %s selected (burst %d)\n", clock, Queue[Head]->processName, Queue[Head]->remainingTime);
				running = true;
			}

			// Update process counters
			Queue[Head]->remainingTime -= 1;
			Queue[Head]->turnaroundTime += 1;

			for (int i = Head + 1; i < Tail; ++i) {
				Queue[i]->turnaroundTime += 1;
				Queue[i]->waitingTime += 1;
			}

			// Check if the head has finished
			if (Queue[Head]->remainingTime == 0) {
				fprintf(out, "Time %d: %s finished\n", clock, Queue[Head]->processName);
				running = false;
				++Head;
			}
		}
		// If Queue[Head] is null and we have clock time left, we are Idle
		else {
			fprintf(out, "Time %d: Idle\n", clock);
		}
	}

	// Show results
	fprintf(out, "Finished at time %d\n\n", clock);
	for (int i = 0; i < Tail; ++i) {
		fprintf(out, "%s wait %d turnaround %d\n", Queue[i]->processName, Queue[i]->waitingTime, Queue[i]->turnaroundTime);
=======
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

>>>>>>> d53c830cc17d7a24ca7dc4e54637a6db3142bfa6
	}

	fclose(out);
    return true;
}