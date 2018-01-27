#include "../include/CLibraryExtensions.h"
#include <string.h>
#include <limits.h>
#include <stdlib.h>

bool CharacterEquals(const void* obj1, const void* obj2)
{
	if (*(char*)obj1 == *(char*)obj2)
		return true;
	return false;
}

bool IntegerEquals(const void* obj1, const void* obj2)
{
	if (*(int*)obj1 == *(int*)obj2)
		return true;
	return false;
}

bool StringEquals(const void* obj1, const void* obj2)
{
	if (strcmp(obj1, obj2) == 0)
		return true;
	return false;
}

int StringHashCode(const void* keyObj)
{
	char* str = (char*)keyObj;
	int hash = 37;
	int strLength = (int)strlen(str);
	
	for (int i = 0; i < strLength; i++)
		hash = abs((hash * 13 + str[i])) % INT_MAX;
	
	return hash;

	// Used the accepted answer of this StackOverflow article for computing a string HashCode.
	// http://stackoverflow.com/questions/7666509/hash-function-for-string
	/*int c;
	while ((c = *str++) != 0)
		hash = (((hash << 2) + hash) + c) % INT_MAX;
	return hash;*/
}

int IntegerHashCode(const void* keyObj)
{
	// Hash code function for an integer is just the integer.
	return *(int*)keyObj;
}
