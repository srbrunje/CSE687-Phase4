// MathLibrary.h - Contains declarations of math functions
#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif


// Long run test
extern "C" MATHLIBRARY_API bool DivZero();

//short run test
extern "C" MATHLIBRARY_API bool AddOK();

extern "C" MATHLIBRARY_API bool AddFail();

