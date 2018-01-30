/**
 * @file CLibraryExtensions.h
 * @author Nkosi Dean
 */

#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "CLibraryHeaders.h"
#include <stdbool.h>

// A HashCode Function type that objects that want to be useable with a HashTable must implement.
// Implementing this allows the HashTable to pick appropriate buckets for storing and retrieving
// the object.
typedef int HashCodeFunction(const void* obj);

// An Equals Function type that can be used to check if two objects are equal to each other.
// Implementing this for different types allows the HashTable to compare objects and allows 
// "Contains" methods to work.
typedef bool EqualsFunction(const void* obj1, const void* obj2);

// A CompareTo Function type that can be used to compare two objects, determining if they are
// equal, one is less than the other, or one is greater than the other.
typedef int ComapreToFunction(const void* obj1, const void* obj2);

// An implementation for the Equals function that compares two characters to each other.
CLIBRARY_API bool CharacterEquals(const void* obj1, const void* obj2);

// An implementation for the Equals function that compares two integers to each other.
CLIBRARY_API bool IntegerEquals(const void* obj1, const void* obj2);

// An implementation for the Equals function that compares two strings to each other.
CLIBRARY_API bool StringEquals(const void* obj1, const void* obj2);

// A hash code implementation for strings.
CLIBRARY_API int StringHashCode(const void* keyObj);

// A hash code implementation for integers.
CLIBRARY_API int IntegerHashCode(const void* keyObj);

#include "Stack.h"
#include "Vector.h"
#include "LinkedList.h"
#include "HashTable.h"