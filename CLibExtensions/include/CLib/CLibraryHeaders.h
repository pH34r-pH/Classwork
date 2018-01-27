/**
 * @file CLibraryHeaders.h
 * @author Nkosi Dean
 */

#pragma once

#include <stdbool.h>

#ifdef _WIN32

#ifdef _MSC_VER
#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#endif

// Export or import public API functions if the library is being built into a shared lib or used as a shared lib.
#ifdef CLIBRARY_DYNAMIC_LINK

// Export public API functions if the library is being built into a shared lib.
#ifdef CLIBRARY_EXPORT_FUNCTIONS
#define CLIBRARY_API __declspec(dllexport)
#else
// Import public API functions if the library is being used as a shared lib.
#define CLIBRARY_API __declspec(dllimport)
#endif

#else

// Building a static library.
#define CLIBRARY_API

// End CLIBRARY_DYNAMIC_LINK for Windows.
#endif



// The Visual C compiler only exports symbols that have been explictely marked to be exported. GCC exports all by
// default. If using Visual Studio, CLIBRARY_NO_EXPORT defines to nothing.
#define CLIBRARY_NO_EXPORT

// End Windows specific.

#else

// Export or import public API functions if the library is being built into a shared lib or used as a shared lib.
#ifdef CLIBRARY_DYNAMIC_LINK

// Export public API functions if the library is being built into a shared lib.
#ifdef CLIBRARY_EXPORT_FUNCTIONS
#define CLIBRARY_API __attribute__ ((dllexport))
#else
// Import public API functions if the library is being used as a shared lib.
#define CLIBRARY_API __attribute__ ((dllimport))
#endif

#else

// Building a static library.
#define CLIBRARY_API

// End CLIBRARY_DYNAMIC_LINK for Linux.
#endif

// The GCC compiler exports everything by default. Use CLIBRARY_NO_EXPORT to prevent export of functions that should
// not be exported.
#define CLIBRARY_NO_EXPORT __attribute__ ((visibility("hidden")))

// End Linux specific.
#endif