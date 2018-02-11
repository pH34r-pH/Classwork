/**
 * @file RoundRobin.c
 */

#include "RoundRobin.h"
#include "ScheduleFile.h"
#include <CLib/Vector.h>
#include <stdlib.h>
#include <stdio.h>
void RunRRProcess (ScheduleData* inputData){
	FILE *out = fopen("processes.out", "w");
	int numProc = VectorCount(inputData->processes);
	fprintf(out, "%d processes\nUsing Round Robin\n", numProc);

	/*
	* How-to:
	* Processes are completed in a Round Robin fashion.
	*/

	LinkedList* list;
	InputProcess ListofProcesses[numProc];
	int Head, clock, quantum, currentQuantum;
	Head = clock = 0;
	currentQuantum = quantum = inputData->timeQuantum;
	bool running = false;

	fprintf(out, "Quantum %d\n\n", quantum);

	for(int i = 0; i < numProc; i++) {
		ListofProcesses[i] = (*(InputProcess*)VectorGet(i, inputData->processes));
	}

	LinkedListCreate(sizeof(int), &list);

	for (; clock < inputData->runLength+1; ++clock) {
		// Check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			if (ListofProcesses[i].arrivalTime == clock) {
				int j = 0;
				// Check to see if the new arrival should be inserted inside the linked list
				for (; j < LinkedListCount(list); j++)
				{
					if(i < *(int*)LinkedListGet(j, list)) {
						LinkedListInsert(&i, j, list);
						break;
					}
				}
				// Check to see if the new arrival should be inserted at the tail
				if(j == LinkedListCount(list)) {
					LinkedListPush(&i, list);
				}
				// Check to see if Head should be incremented to point to the same node in the list
				if(Head >= j && LinkedListCount(list) > 1) {
					++Head;
				}
				fprintf(out, "Time %d: %s arrived\n", clock, ListofProcesses[i].processName);
			}
		}
		if(running) {
			// Check if the current process just finished
			if (ListofProcesses[*(int *)LinkedListGet(Head, list)].remainingTime == 0) {
				fprintf(out, "Time %d: %s finished\n", clock, ListofProcesses[*(int *)LinkedListGet(Head, list)].processName);
				LinkedListRemove(Head, list);
				running = false;
				currentQuantum = quantum;
			}
			// Check if we've reached exceeded the time quantum
			else if(currentQuantum == 0) {
				running = false;
				currentQuantum = quantum;
				++Head;
			}
		}

		// Did we run out of time?
		if(clock == inputData->runLength) {
			break;
		}

		// Circle back around to the beginning of the linked list
		if(Head >= LinkedListCount(list)) {
			Head = 0;
		}

		// Check if we have at least one ready process
		if (Head < LinkedListCount(list)) {
			// Check if we have a process running, or select a new process
			if (!running) {
				fprintf(
					out, "Time %d: %s selected (burst %d)\n",
					clock,
					ListofProcesses[*(int *)LinkedListGet(Head, list)].processName,
					ListofProcesses[*(int *)LinkedListGet(Head, list)].remainingTime
				);
				running = true;
			}

			// Update process counters
			ListofProcesses[*(int *)LinkedListGet(Head, list)].remainingTime -= 1;
			ListofProcesses[*(int *)LinkedListGet(Head, list)].turnaroundTime += 1;
			currentQuantum--;

			for (int i = 0; i < LinkedListCount(list); i++) {
				if(ListofProcesses[*(int *)LinkedListGet(i, list)].remainingTime > 0 &&
					i != Head) {
						ListofProcesses[*(int *)LinkedListGet(i, list)].waitingTime += 1;
					ListofProcesses[*(int *)LinkedListGet(i, list)].turnaroundTime += 1;
				}
			}
		}
		// If there is no available process and we have clock time left, we are Idle
		else {
			fprintf(out, "Time %d: IDLE\n", clock);
		}
	}

	// Show results
	fprintf(out, "Finished at time %d\n\n", clock);
	for (int i = 0; i < numProc; ++i) {
		fprintf(
			out,
			"%s wait %d turnaround %d\n",
			ListofProcesses[i].processName,
			ListofProcesses[i].waitingTime,
			ListofProcesses[i].turnaroundTime
		);
	}

	fclose(out);
  return;
}