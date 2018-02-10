/**
 * @file RoundRobin.c
 */

#include "RoundRobin.h"
#include "ScheduleFile.h"
#include <CLib/Vector.h>
#include <stdlib.h>
#include <stdio.h>
bool RunRRProcess (ScheduleData* inputData){
	FILE *out = fopen("processes.out", "w");
	int numProc = VectorCount(inputData->processes);
	fprintf(out, "%d processes\nUsing Round Robin\n", numProc);

	/*
	* How-to:
	* Processes are completed in a Round Robin fashion.
	*/

	int Queue[numProc];
	InputProcess ListofProcesses[numProc];
	int Head, Tail, clock, quantum, currentQuantum;
	Head = Tail = clock = 0;
	currentQuantum = quantum = inputData->timeQuantum;
	bool running = false;

	fprintf(out, "Quantum %d\n\n", quantum);

	for(int i = 0; i < numProc; i++) {
		ListofProcesses[i] = (*(InputProcess*)VectorGet(i, inputData->processes));
	}

	for (; clock < inputData->runLength; ++clock) {
		// Check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			if (ListofProcesses[i].arrivalTime == clock) {
				Queue[Tail % numProc] = i;
				fprintf(out, "Time %d: %s arrived\n", clock, ListofProcesses[Queue[Tail % numProc]].processName);
				++Tail;
			}
		}

		// Check if we have at least one ready process
		if (Head < Tail) {

			// Check if the head has finished
			if (ListofProcesses[Queue[Head % numProc]].remainingTime == 0) {
				fprintf(out, "Time %d: %s finished\n", clock, ListofProcesses[Queue[Head % numProc]].processName);
				running = false;
				++Head;
				currentQuantum = quantum;
				// Check out now if we still have a ready process given that we just finished one
				if(Head == Tail) {
					fprintf(out, "Time %d: IDLE\n", clock);
					continue;
				}
			}

			// Check if we have a process running, or select a new process
			if (!running) {
				fprintf(
					out, "Time %d: %s selected (burst %d)\n",
					clock,
					ListofProcesses[Queue[Head % numProc]].processName,
					ListofProcesses[Queue[Head % numProc]].remainingTime
				);
				running = true;
			}

			// Update process counters
			ListofProcesses[Queue[Head % numProc]].remainingTime -= 1;
			ListofProcesses[Queue[Head % numProc]].turnaroundTime += 1;
			currentQuantum--;

			for (int i = Head+1; i < Tail; i++) {
				if(ListofProcesses[Queue[i % numProc]].remainingTime > 0) {
						ListofProcesses[Queue[i % numProc]].waitingTime += 1;
					ListofProcesses[Queue[i % numProc]].turnaroundTime += 1;
				}
			}
			
			// Check if we've reached exceeded the time quantum
			if(currentQuantum == 0) {
				running = false;
				currentQuantum = quantum;
				Queue[Tail % numProc] = Queue[Head % numProc];
				++Head;
				++Tail;
			}
		}
		// If Queue[Head] is null and we have clock time left, we are Idle
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
    return true;
}