// MathLibrary.h - Contains declarations of math functions
#pragma once

#ifdef LONGRUNLIBRARY_EXPORTS
#define LONGRUNLIBRARY_API __declspec(dllexport)
#else
#define LONGRUNLIBRARY_API __declspec(dllimport)
#endif


// Long run test
extern "C" LONGRUNLIBRARY_API bool LongRunTest();
