/**
 * @file FirstComeFirstServe.c
 */

#include "FirstComeFirstServe.h"
#include "ScheduleFile.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void RunFCFSProcess (ScheduleData* inputData){
	bool debug = false;		
	
	if(debug) printf("Starting FCFS\n");

	FILE *out = fopen("processes.out", "w");
	if(out == NULL) {
		printf("Error opening output file\n");
		return;
	}
	if(debug) printf("Opened processes.out\n");

	int numProc = VectorCount(inputData->processes);

	if(debug) printf("%d processes\nUsing First-Come First-Serve\n\n", numProc);
	fprintf(out, "%d processes\nUsing First-Come First-Serve\n\n", numProc);

	/*
	* How-to:
	* Processes are completed in the order they are received.
	*/

	Vector* queue;
	VectorCreate(sizeof(InputProcess), numProc, 0, NULL, &queue);
	int head, tail, clock;
	head = tail = clock = 0;
	bool running = false;
	
	if(debug) printf("Beginning processing:\n");
	for (; clock <= inputData->runLength; ++clock) 
	{
		// Iterate through processes to check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			InputProcess* process = VectorGet(i, inputData->processes);
			if (process->arrivalTime == clock) {
				if(debug) printf("Time %d: %s arrived\n", clock, process->processName);
				fprintf(out, "Time %d: %s arrived\n", clock, process->processName);
				VectorAdd(process, queue);
				++tail;
			}
		}
		// Check if we have at least one ready process
		if(head != tail){

			// Check if head has finished
			InputProcess* headProcess = VectorGet(head, queue);
			headProcess->remainingTime -= 1;
			headProcess->turnaroundTime += 1;

			// Update everything else
			for(int i = head+1; i < tail; ++i){
				InputProcess* thisProcess = VectorGet(i, queue);
				thisProcess->turnaroundTime += 1;
				thisProcess->waitingTime += 1;
			}

			if(headProcess->remainingTime == 0){
				if(debug) printf("Time %d: %s finished\n", clock, headProcess->processName);
				fprintf(out, "Time %d: %s finished\n", clock, headProcess->processName);
				running = false;
				++head;
			}

			// If head has finished or we are idle, and there is still at least one ready process, select a new process
			if(!running && head != tail){
				InputProcess* newProcess = VectorGet(head, queue);
				if(debug) printf("Time %d: %s selected (burst %d)\n", clock, newProcess->processName, newProcess->remainingTime);
				fprintf(out, "Time %d: %s selected (burst %d)\n", clock, newProcess->processName, newProcess->remainingTime);
				running = true;
			}
			
		}
		// No ready processes means we're Idle
		else {
			if(debug) printf("Time %d: Idle\n", clock);
			fprintf(out, "Time %d: Idle\n", clock);
		}		
	}		
	// Show results
	if(debug) printf("Finished at time %d\n\n", clock-1);
	fprintf(out, "Finished at time %d\n\n", clock-1);
	for(int i = 0; i < tail; ++i){
		InputProcess* thisProcess = VectorGet(i, queue);
		if(debug) printf("%s wait %d turnaround %d\n", thisProcess->processName, thisProcess->waitingTime, thisProcess->turnaroundTime);
		fprintf(out, "%s wait %d turnaround %d\n", thisProcess->processName, thisProcess->waitingTime, thisProcess->turnaroundTime);
	}

	fclose(out);
}
