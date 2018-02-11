/**
 * @file Lexer.c
 */

#include "Lexer.h"
#include "Scanner.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Internal representation of a Lexer.
 */
typedef struct tagLexer
{
    /**
     * A scanner object to help make parsing the processes file easier.
     */
    Scanner* scanner;
    /**
     * A vector that contains all the tokens, in the order they were found in the file.
     */
    Vector* tokens;
} Lexer;

bool LexerCreate(Lexer** lexer)
{
    *lexer = calloc(1, sizeof(Lexer));
    if (*lexer == NULL)
        return false;

    // Instantiate the vector.
    if (!VectorCreate(sizeof(LexerToken), 50, 0, NULL, NULL, &(*lexer)->tokens))
    {
        free(*lexer);
        return false;
    }

    // Instantiate the scanner.
    if (!ScannerCreate(&(*lexer)->scanner))
    {
        VectorDestroy(&(*lexer)->tokens);
        free(*lexer);
        return false;
    }

    return true;
}

void LexerDestroy(Lexer** lexer)
{
    // Since the string value of a token is dynamically allocated, that memory needs to be freed.
    // Go through each string token and free their string memory.
    Vector* tokens = (*lexer)->tokens;
    for (int i = 0; i < VectorCount(tokens); i++)
    {
        LexerToken* token = VectorGet(i, tokens);
        if (token->tokenType == String && token->strTokenValue != NULL)
        {
            free(token->strTokenValue);
            token->strTokenValue = NULL;
        }
    }

    // Free everything else.
    ScannerDestroy(&(*lexer)->scanner);
    VectorDestroy(&(*lexer)->tokens);
    free(*lexer);
    *lexer = NULL;
}

bool LexerParseFile(const char* filename, Lexer* lexer)
{
    // Direct the scanner to read the input file.
    if (!ScannerReadFile(filename, lexer->scanner))
    {
        fprintf(stderr, "Scanner failed to read input file.\n");
        return false;
    }

    Scanner* scan = lexer->scanner;
    while (!ScannerIsAtEnd(scan))
    {
        switch (ScannerGetCharType(scan))
        {
            case Alpha:
            {
                // This is a word. Read the rest of the word and store it as a string token.
                LexerToken token;
                token.tokenType = String;

                // Read a string from input.
                char* parsedString;
                unsigned int currentWordLength;
                ScannerReadNextString(&parsedString, &currentWordLength, scan);

                // In order to use the word the scanner found, it has to be copied out and turned into a C String.
                token.strTokenValue = calloc(currentWordLength + 1, sizeof(char));
                memcpy(token.strTokenValue, parsedString, currentWordLength);
                token.strTokenValue[currentWordLength] = '\0';

                // Store the token.
                VectorAdd(&token, lexer->tokens);

                break;
            }
            case Digit:
            {
                // This is a number. Read the number and store it as a numerical token.
                LexerToken token;
                token.tokenType = Number;
                token.numTokenValue = (unsigned int)ScannerReadNextInteger(scan);
                VectorAdd(&token, lexer->tokens);
                break;
            }
            case Symbol:
            {
                // Is the symbol the comment (#) character?
                if (ScannerGetChar(scan) == '#')
                {
                    // This line is a comment. Ignore the rest of the line.
                    ScannerMoveToNextLine(scan);
                }
            }
            case Whitespace:
            {
                // Skip the whitespace.
                ScannerMoveForward(scan);
                break;
            }
            case EndFile:
                break;
        }
    }

    return true;
}

Vector* LexerGetTokens(Lexer* lexer)
{
    Vector* copyVector;
    if (!VectorCreate(sizeof(LexerToken), VectorCount(lexer->tokens), 0, NULL, NULL, &copyVector))
    {
        return NULL;
    }

    // Copy the contents of the lexer's vectors to give to the client.
    if (!VectorCopy(copyVector, lexer->tokens))
    {
        VectorDestroy(&copyVector);
        return NULL;
    }

    // Return the copy.
    return copyVector;
}