/**
 * @file Lexer.h
 */

#pragma once

#include <CLib/Vector.h>
#include <stdbool.h>

typedef enum tagTokenType
{
    String = 0,
    Number = 1
} TokenType;

/**
 * Represents a token that the lexer parsed from the scanner.
 */
typedef struct tagLexerToken
{
    /**
     * The type of the token.
     */
    TokenType tokenType;
    /**
     * The string value of the token, stored as a C String.
     * - If tokenType = String, this is a dynamically allocated string that later needs to be released.
     * - If tokenType = Number, this is null.
     */
    char* strTokenValue;
    /**
     * The numerical value of the token, if tokenType = Number. Otherwise, this is just set to 0.
     */
    unsigned int numTokenValue;
} LexerToken;

/**
 * Represents a lexer, which breaks the input processes file into easier to process tokens.
 */
typedef struct tagLexer Lexer;

/**
 * Creates a new lexer.
 * @param lexer A pointer to the lexer object to initialize.
 * @return True if the operation succeeded, otherwise false.
 */
bool LexerCreate(Lexer** lexer);

/**
 * Destroys an existing lexer.
 * @param lexer A pointer to the lexer object to destroy.
 */
void LexerDestroy(Lexer** lexer);

/**
 * Directs the lexer to parse through a file, pulling out the tokens.
 * @param filename The filename that the lexer should parse.
 * @param lexer The lexer that should parse the file.
 * @return True if the parsing operation succeeds, otherwise false.
 */
bool LexerParseFile(const char* filename, Lexer* lexer);

/**
 * Gets the tokens from the lexer. Note that the returned vector needs to be destroyed
 * by the client.
 * @param lexer The lexer to get the tokens from.
 * @return The vector of tokens that the lexer parsed out.
 */
Vector* LexerGetTokens(Lexer* lexer);