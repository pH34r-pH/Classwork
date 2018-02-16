/**
 * @file SJF.c
 */

#include "SJF.h"
#include "ScheduleFile.h"
#include <CLib/Vector.h>
#include <stdlib.h>
#include <stdio.h>
void RunPreemptiveSJFProcess (ScheduleData* inputData){
	FILE *out = fopen("processes.out", "w");
	int numProc = VectorCount(inputData->processes);
	fprintf(out, "%d processes\nUsing Shortest Job First (Pre)\n\n", numProc);

	/*
	* How-to:
	* Processes are completed using Shortest Job First.
	*/

	LinkedList* list;
	InputProcess listofProcesses[numProc];
	int clock, quantum, currentQuantum;
	clock = 0;
	currentQuantum = quantum = inputData->timeQuantum;
	bool running = false;

	for(int i = 0; i < numProc; i++) {
		listofProcesses[i] = (*(InputProcess*)VectorGet(i, inputData->processes));
	}

	LinkedListCreate(sizeof(int), &list);

	for (; clock < inputData->runLength+1; ++clock) {

		if(running) {
			// Check if the current process just finished
			if (listofProcesses[*(int *)LinkedListGet(0, list)].remainingTime == 0) {
				fprintf(out, "Time %d: %s finished\n", clock, listofProcesses[*(int *)LinkedListGet(0, list)].processName);
				LinkedListRemove(0, list);
				running = false;
				currentQuantum = quantum;
			}
		}

		// Check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			if (listofProcesses[i].arrivalTime == clock) {
				int j = 0;
				// Check to see if the new arrival should be inserted inside the linked list
				for (; j < LinkedListCount(list); j++)
				{
					if(listofProcesses[i].remainingTime < listofProcesses[*(int*)LinkedListGet(j, list)].remainingTime) {
						LinkedListInsert(&i, j, list);
						// no longer running the current process because a new one has higher priority
						if(j == 0) {
							running = false;
						}
						break;
					}
				}
				// Check to see if the new arrival should be inserted at the tail
				if(j == LinkedListCount(list)) {
					LinkedListPush(&i, list);
				}
				fprintf(out, "Time %d: %s arrived\n", clock, listofProcesses[i].processName);
			}
		}

		// Did we run out of time?
		if(clock == inputData->runLength) {
			break;
		}

		// Check if we have at least one ready process
		if (LinkedListCount(list) > 0) {
			// Check if we have a process running, or select a new process
			if (!running) {
				fprintf(
					out, "Time %d: %s selected (burst %d)\n",
					clock,
					listofProcesses[*(int *)LinkedListGet(0, list)].processName,
					listofProcesses[*(int *)LinkedListGet(0, list)].remainingTime
				);
				running = true;
			}

			// Update process counters
			listofProcesses[*(int *)LinkedListGet(0, list)].remainingTime -= 1;
			listofProcesses[*(int *)LinkedListGet(0, list)].turnaroundTime += 1;
			currentQuantum--;

			for (int i = 0; i < LinkedListCount(list); i++) {
				if(listofProcesses[*(int *)LinkedListGet(i, list)].remainingTime > 0 &&
					i != 0) {
						listofProcesses[*(int *)LinkedListGet(i, list)].waitingTime += 1;
					listofProcesses[*(int *)LinkedListGet(i, list)].turnaroundTime += 1;
				}
			}
		}
		// If there is no available process and we have clock time left, we are Idle
		else {
			fprintf(out, "Time %d: Idle\n", clock);
		}
	}

	// Show results
	fprintf(out, "Finished at time %d\n\n", clock);
	for (int i = 0; i < numProc; ++i) {
		fprintf(
			out,
			"%s wait %d turnaround %d\n",
			listofProcesses[i].processName,
			listofProcesses[i].waitingTime,
			listofProcesses[i].turnaroundTime
		);
	}

	fclose(out);
  return;
}