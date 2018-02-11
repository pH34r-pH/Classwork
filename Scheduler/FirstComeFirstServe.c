/**
 * @file FirstComeFirstServe.c
 */

#include "FirstComeFirstServe.h"
#include "ScheduleFile.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void RunProcess (ScheduleData* inputData){
	bool debug = true;		
	
	if(debug) printf("Starting FCFS\n");

	FILE *out = fopen("processes.out", "w");
	if(out == NULL) {
		printf("Error opening output file\n");
		return;
	}
	if(debug) printf("Opened processes.out\n");

	int numProc = VectorCount(inputData->processes);

	if(debug) printf("%d processes\nUsing First Come First Serve\n\n", numProc);
	fprintf(out, "%d processes\nUsing First Come First Serve\n\n", numProc);

	/*
	* How-to:
	* Processes are completed in the order they are received.
	*/

	Vector* queue;
	VectorCreate(sizeof(InputProcess), numProc, 0, NULL, NULL, &queue);
	int head, tail, clock;
	head = tail = clock = 0;
	bool running = false;
	
	if(debug) printf("Beginning processing:\n");
	for (; clock < inputData->runLength; ++clock) 
	{
		if(debug) printf("heartbeat 1");
		// Iterate through processes to check for new arrivals
		for (int i = 0; i < numProc; ++i) {
			InputProcess* process = VectorGet(i, inputData->processes);
			if (process->arrivalTime == clock) {
				if(debug) printf("Time %d: %s arrived\n", clock, process->processName);
				fprintf(out, "Time %d: %s arrived\n", clock, process->processName);
				if(debug) printf("heartbeat pre-add");
				VectorAdd(process, queue);
				if(debug) printf("heartbeat post-add");
				++tail;
			}
		}
		if(debug) printf("heartbeat 2");
		// Check if we have at least one ready process
		if(head != tail){
			// Check if we have a process running, or select a new process
			if(!running){
				InputProcess* newProcess = VectorGet(head, queue);
				if(debug) printf("Time %d: %s selected (burst %d)\n", clock, newProcess->processName, newProcess->remainingTime);
				fprintf(out, "Time %d: %s selected (burst %d)\n", clock, newProcess->processName, newProcess->remainingTime);
				running = true;
			}
			if(debug) printf("heartbeat 3");
			// Update process counters
			InputProcess* headProcess = VectorGet(head, queue);
			headProcess->remainingTime -= 1;
			headProcess->turnaroundTime += 1;

			for(int i = head+1; i < tail; ++i){
				InputProcess* thisProcess = VectorGet(i, queue);
				thisProcess->turnaroundTime += 1;
				thisProcess->waitingTime += 1;
			}
			if(debug) printf("heartbeat 4");
			// Check if head has finished
			if(headProcess->remainingTime == 0){
				if(debug) printf("Time %d: %s finished\n", clock, headProcess->processName);
				fprintf(out, "Time %d: %s finished\n", clock, headProcess->processName);
				running = false;
				++head;
			}
		}
		// No ready processes means we're Idle
		else {
			if(debug) printf("Time %d: Idle", clock);
			fprintf(out, "Time %d: Idle", clock);
		}				
		if(debug) printf("heartbeat 5");
		// Show results
		if(debug) printf("Finished at time %d\n\n", clock);
		fprintf(out, "Finished at time %d\n\n", clock);
		for(int i = 0; i < tail; ++i){
			InputProcess* thisProcess = VectorGet(i, queue);
			if(debug) printf("%s wait %d turnaround %d\n", thisProcess->processName, thisProcess->waitingTime, thisProcess->turnaroundTime);
			fprintf(out, "%s wait %d turnaround %d\n", thisProcess->processName, thisProcess->waitingTime, thisProcess->turnaroundTime);
		}

	}

	fclose(out);
}
