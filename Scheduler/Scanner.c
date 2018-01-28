/**
 * @file Scanner.c
 */

#include "Scanner.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>

/**
 * Internal representation of a Scanner.
 */
typedef struct tagScanner
{
    /**
     * Represents the raw contents of the file.
     */
    char* fileContents;
    /**
     * The length of the file, in bytes.
     */
    unsigned int fileLength;
    /**
     * The current index position in the file that the scanner is on.
     */
    unsigned int currentPos;
} Scanner;

bool ScannerCreate(Scanner** scan)
{
    (*scan) = calloc(1, sizeof(Scanner));
    if (*scan == NULL)
        return false;

    return true;
}

void ScannerDestroy(Scanner** scan)
{
    free((*scan)->fileContents);
    free(*scan);
    *scan = NULL;
}

bool ScannerReadFile(const char* fileName, Scanner* scan)
{
    // Get file statistics.
    struct stat fileStats;
    if (stat(fileName, &fileStats) == -1)
    {
        fprintf(stderr, "Failed to get file statistics.\n");
        return false;
    }

    // Get the length of the file.
    scan->fileLength = (unsigned int)fileStats.st_size;

    // Open the processes.in file.
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open the 'processes.in' file. Are you sure it's in the same directory "
                "as the executable?\n");
        return false;
    }

    // Read the contents of the file.
    scan->fileContents = calloc(scan->fileLength + 1, sizeof(char));
    unsigned int numBytesRead = fread(scan->fileContents, sizeof(char), scan->fileLength, file);

    fclose(file);

    if (numBytesRead < scan->fileLength)
    {
        fprintf(stderr, "Failed to read the entire contents of the file.\n");
        free(scan->fileContents);
        scan->fileContents = NULL;
        return false;
    }

    // Ensure fileContents is a C String.
    scan->fileContents[scan->fileLength] = '\0';

    return true;
}

void ScannerReadNextString(char** strStart, unsigned int* strLength, Scanner* scan)
{
    // Set teh start position of the string.
    unsigned int length = 0;
    *strStart = scan->fileContents + scan->currentPos;

    // Move the scanner forward until a whitespace character is found, or the end of the file is reached.
    while (!ScannerIsAtEnd(scan) && !isblank(scan->fileContents[scan->currentPos]))
    {
        length++;
        ScannerMoveForward(scan);
    }

    // Set the end position to where the scanner is now.
    *strLength = length;
}

int ScannerReadNextInteger(Scanner* scan)
{
    int numberStartPos = scan->currentPos;

    // Convert the portion of teh file data from start position to end position into a number.
    // The nice thing about strtol is it will continue parsing digits until a non digit is found.
    // It then outputs a pointer to the section of the string where the number ends.
    char* numberEndPos;
    int result = (int)strtol(scan->fileContents + numberStartPos, &numberEndPos, 10);

    // Update the position of the scanner. Make it point to the character (or end of file) directly after the last
    // character of the number.
    scan->currentPos = (unsigned int)(numberEndPos - scan->fileContents);

    return result;
}

void ScannerMoveForward(Scanner* scan)
{
    if (ScannerIsAtEnd(scan))
        return;

    scan->currentPos++;
}

bool ScannerIsAtEnd(Scanner* scan)
{
    return scan->currentPos == scan->fileLength;
}