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
 * Creates a new scanner object.
 * @param scan A pointer to the scanner object to initialize.
 * @return True if the operation succeeds, otherwise false.
 */
bool ScannerCreate(Scanner** scan);

/**
 * Destroys a scanner object.
 * @param scan The scanner object to destroy.
 */
void ScannerDestroy(Scanner** scan);

/**
 * Reads the contents of a file into the scanner.
 * @param filename The filename to read from.
 * @param scan The scanner object to load the file into.
 * @return True if the operation succeeds, otherwise false.
 */
bool ScannerReadFile(const char* filename, Scanner* scan);

/**
 * @brief Reads the next string from the file.
 *
 * Starting with the scanner's current position, which should be a non-whitespace character, reads until a string is
 * parsed, and stops when a whitespace character is found.
 * @param strStart Sets a pointer to the start position of the string in the scanner's internal file contents.
 * @param strLength Outputs the length of the string.
 * @param scan The scanner object to use.
 */
void ScannerReadNextString(char** strStart, unsigned int* strLength, Scanner* scan);

/**
 * Reads the next integer from the file.
 * @param scan The scanner to use.
 * @return The next integer that is detected.
 */
int ScannerReadNextInteger(Scanner* scan);

/**
 * Moves a scanner forward into the file by one character.
 * @param scan The scanner to move forward.
 */
void ScannerMoveForward(Scanner* scan);

/**
 * Gets whether or not the scanner has reached the end of the file (nothing else to read).
 * @param scan The scanner to use.
 * @return True if the scanner has reached the end of the file, otherwise false.
 */
bool ScannerIsAtEnd(Scanner* scan);