/**
 * @file ScheduleFile.c
 */

#include "ScheduleFile.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Once a process line is identified, this will handle the tokens that make
// up a process line, returning false if any of the tokens are invalid. The main
// code should have the currentTokenIndex positioned on "process".
bool ParseProcess(Vector* tokens, unsigned int* currentTokenIndex, InputProcess* process)
{
    LexerToken* currentToken;

    for (int i = 0; i < 3; i++)
    {
        currentToken = VectorGet(++(*currentTokenIndex), tokens);
        if (strcmp("name", currentToken->strTokenValue) == 0)
        {
            // The next token should be a string value representing the name of the process.
            currentToken = VectorGet(++(*currentTokenIndex), tokens);
            if (currentToken->tokenType != String)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected digit character.\n");
                return false;
            }

            strcpy(process->processName, currentToken->strTokenValue);
        }
        else if (strcmp("arrival", currentToken->strTokenValue) == 0)
        {
            // The next token should be an integer value representing the arrival time of the process.
            currentToken = VectorGet(++(*currentTokenIndex), tokens);
            if (currentToken->tokenType != Number)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected alphabetic character.\n");
                return false;
            }

            process->arrivalTime = currentToken->numTokenValue;
        }
        else if (strcmp("burst", currentToken->strTokenValue) == 0)
        {
            // The next token should be an integer value representing the burst length of the process.
            currentToken = VectorGet(++(*currentTokenIndex), tokens);
            if (currentToken->tokenType != Number)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected alphabetic character.\n");
                return false;
            }

            process->burstLength = currentToken->numTokenValue;
        }
        else
        {
            // Unexpected value found.
            fprintf(stderr, "Invalid processes file specified. Found an unexpected value on a 'process' line.\n");
            return false;
        }
    }

    return true;
}

bool ReadScheduleFile(ScheduleData* fileData)
{
    // Create a lexer for parsing the file.
    Lexer* lexer;
    if (!LexerCreate(&lexer))
    {
        fprintf(stderr, "Failed to create a lexer.\n");
        return false;
    }

    // Read the file into the lexer.
    if (!LexerParseFile("processes.in", lexer))
    {
        fprintf(stderr, "The lexer failed to read the 'processes.in' file. Are you sure it exists in the same folder "
                "as the executable?\n");
        LexerDestroy(&lexer);
        return false;
    }

    // Get the vector of tokens that were pulled from the file.
    Vector* tokens = LexerGetTokens(lexer);

    // Loop through the tokens.
    unsigned int currentTokenIndex = 0;
    bool readSuccessful = true;
    while (currentTokenIndex < VectorCount(tokens))
    {
        // Get the current token.
        LexerToken* currentToken = VectorGet(currentTokenIndex, tokens);

        // At this point, the token should always be a string. At this level, we should never find a number token.
        if (currentToken->tokenType != String)
        {
            fprintf(stderr, "Invalid processes file specified. Found an unexpected digit.\n");
            readSuccessful = false;
            break;
        }

        // Do something depending on what the word is.
        if (strcmp(currentToken->strTokenValue, "processcount") == 0)
        {
            // The next value in the file should be a number representing the number of processes that
            // are in the file.
            currentToken = VectorGet(++currentTokenIndex, tokens);
            if (currentToken->tokenType != Number)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected alphabetic character.\n");
                readSuccessful = false;
                break;
            }

            // Using the number of expected processes, initialize the vector of processes to contain them all.
            if (!VectorCreate(sizeof(InputProcess), currentToken->numTokenValue, 0, NULL, &fileData->processes))
            {
                fprintf(stderr, "Failed to initialize a vector to contain the input processes.\n");
                readSuccessful = false;
                break;
            }
        }
        else if (strcmp(currentToken->strTokenValue, "runfor") == 0)
        {
            // The next value in the file should be a number representing how long to run the scheduler.
            currentToken = VectorGet(++currentTokenIndex, tokens);
            if (currentToken->tokenType != Number)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected alphabetic character.\n");
                readSuccessful = false;
                break;
            }

            fileData->runLength = currentToken->numTokenValue;
        }
        else if (strcmp(currentToken->strTokenValue, "use") == 0)
        {
            // The next value in the file should be a string representing the scheduler type we want to use.
            currentToken = VectorGet(++currentTokenIndex, tokens);
            if (currentToken->tokenType != String)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected digit.\n");
                readSuccessful = false;
                break;
            }

            if (strcmp(currentToken->strTokenValue, "fcfs") == 0)
            {
                fileData->schedulerType = FCFS;
            }
            else if (strcmp(currentToken->strTokenValue, "sjf") == 0)
            {
                fileData->schedulerType = ShortestJob;
            }
            else if (strcmp(currentToken->strTokenValue, "rr") == 0)
            {
                fileData->schedulerType = RoundRobin;
            }
            else
            {
                fprintf(stderr, "Invalid processes file specified. Unknown scheduler algorithm specified.\n");
                readSuccessful = false;
                break;
            }
        }
        else if (strcmp(currentToken->strTokenValue, "quantum") == 0)
        {
            // The next value in the file should be a number representing the time quantum value.
            currentToken = VectorGet(++currentTokenIndex, tokens);
            if (currentToken->tokenType != Number)
            {
                fprintf(stderr, "Invalid processes file specified. Found an unexpected alphabetic character.\n");
                readSuccessful = false;
                break;
            }

            fileData->timeQuantum = currentToken->numTokenValue;
        }
        else if (strcmp(currentToken->strTokenValue, "process") == 0)
        {
            // The next 6 tokens should all be part of a process.
            InputProcess process;
            readSuccessful = ParseProcess(tokens, &currentTokenIndex, &process);

            if (!readSuccessful)
                break;

            VectorAdd(&process, fileData->processes);
        }
        else if (strcmp(currentToken->strTokenValue, "end") == 0)
        {
            // No work needs to be done. We've reached the end of input.
            break;
        }
        else
        {
            // Unknown value specified.
            fprintf(stderr, "Invalid processes file specified. Found an unexpected string value where a process "
                    "configuration value was expected.\n");
            readSuccessful = false;
        }

        currentTokenIndex++;
    }

    // The call to LexerDestroy will handle this.
#if 0
    // Destroy each dynamically allocated string token.
    for (int i = 0; i < VectorCount(tokens); i++)
    {
        LexerToken* token = VectorGet(i, tokens);
        if (token->tokenType == String && token->strTokenValue != NULL)
        {
            free(token->strTokenValue);
            token->strTokenValue = NULL;
        }
    }
#endif

    // Destroy the lexer.
    LexerDestroy(&lexer);

    return readSuccessful;
}