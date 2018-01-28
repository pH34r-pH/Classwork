/**
 * @file ScheduleFile.c
 */

#include "ScheduleFile.h"
#include "Scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool ReadScheduleFile(ScheduleData* fileData)
{
    // Create a scanner for parsing the file.
    Scanner* scan;
    if (!ScannerCreate(&scan))
    {
        fprintf(stderr, "Failed to create a scanner.\n");
        return false;
    }

    // Read the file into the scanner.
    if (!ScannerReadFile("processes.in", scan))
    {
        fprintf(stderr, "The scanner failed to read the 'processes.in' file. Are you sure it exists in the same folder "
                "as the executable?\n");
        ScannerDestroy(&scan);
        return false;
    }

    while (!ScannerIsAtEnd(scan))
    {
        // Read a string from input.
        char* parsedString;
        unsigned int currentWordLength;
        ScannerReadNextString(&parsedString, &currentWordLength, scan);

        // In order to use the word the scanner found, it has to be copied out and turned into a C String.
        char* currentWord = calloc(currentWordLength + 1, sizeof(char));
        memcpy(currentWord, parsedString, currentWordLength);
        currentWord[currentWordLength] = '\0';

        // Do something depending on what the word is.
        if (strcmp(currentWord, "processcount"))
        {
            // The next value in the file should be a number representing the number of processes that
            // are in the file.
            fileData->numProcesses = (unsigned int)ScannerReadNextInteger(scan);

            // Initialize the processes array with the expected number of processes.
            fileData->processes = calloc(fileData->numProcesses, sizeof(InputProcess));
        }
        else if (strcmp(currentWord, "runfor"))
        {
            // The next value in the file should be a number representing the run time for the scheduler.
            fileData->runLength = (unsigned int)ScannerReadNextInteger(scan);
        }
        else if (strcmp(currentWord, "use"))
        {
            // The next value in the file should be a string representing the scheduler type we want to use.

        }
        else if (strcmp(currentWord, "quantum"))
        {

        }
        else if (strcmp(currentWord, "process"))
        {

        }
    }

    // Destroy the scanner.
    ScannerDestroy(&scan);

    return true;
}