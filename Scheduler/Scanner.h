/**
 * @file Scanner.h
 */

#pragma once

#include <stdbool.h>

/**
 * Represents a scanner, which reads through a file character by character.
 */
typedef struct tagScanner Scanner;

/**
 * Represents the different types that a character can be as the Scanner scans through PL/0 code.
 */
typedef enum tagCharType {
    /**
     * The character is an alphabetic type.
     */
    Alpha,
    /**
     * The character is a numeric type.
     */
    Digit,
    /**
     * The character is some non alphanumeric symbol.
     */
    Symbol,
    /**
     * The character is some form of whitespace.
     */
    Whitespace,
    /**
     * There is no character, as the scanner has reached the end of file at this point.
     */
    EndFile
} CharType;

/**
 * Creates a new scanner object.
 * @param scan A pointer to the scanner object to initialize.
 * @return True if the operation succeeds, otherwise false.
 */
bool ScannerCreate(Scanner **scan);

/**
 * Destroys a scanner object.
 * @param scan The scanner object to destroy.
 */
void ScannerDestroy(Scanner **scan);

/**
 * Reads the contents of a file into the scanner.
 * @param filename The filename to read from.
 * @param scan The scanner object to load the file into.
 * @return True if the operation succeeds, otherwise false.
 */
bool ScannerReadFile(const char *filename, Scanner *scan);

/**
 * @brief Reads the next string from the file.
 *
 * Starting with the scanner's current position, which should be a non-whitespace character, reads until a string is
 * parsed, and stops when a whitespace character is found.
 * @param strStart Sets a pointer to the start position of the string in the scanner's internal file contents.
 * @param strLength Outputs the length of the string.
 * @param scan The scanner object to use.
 */
void ScannerReadNextString(char **strStart, unsigned int *strLength, Scanner *scan);

/**
 * Reads the next integer from the file.
 * @param scan The scanner to use.
 * @return The next integer that is detected.
 */
int ScannerReadNextInteger(Scanner *scan);

/**
 * Moves a scanner forward into the file by one character.
 * @param scan The scanner to move forward.
 */
void ScannerMoveForward(Scanner *scan);

/**
 * Moves the scanner forward until it has consumed the current line and is on the next line.
 * @param scan The scanner to move forward.
 */
void ScannerMoveToNextLine(Scanner *scan);

/**
 * Gets the type of the current character.
 * @param scan The scanner to use.
 * @return The type of the current character.
 */
CharType ScannerGetCharType(Scanner* scan);

/**
 * Gets the current character that the scanner is on.
 * @param scan The scanner to use.
 * @return The current character.
 */
char ScannerGetChar(Scanner* scan);

/**
 * Gets whether or not the scanner has reached the end of the file (nothing else to read).
 * @param scan The scanner to use.
 * @return True if the scanner has reached the end of the file, otherwise false.
 */
bool ScannerIsAtEnd(Scanner *scan);