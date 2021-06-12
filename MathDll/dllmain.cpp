// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MathDLL.h"
#include <csignal>
#include <exception>

int AddInts(int a, int b)
{
	return a + b;
}

bool DivZero()
{
	int a = 0;
	int b = 5;

//	int r = b / a;  //throw div zero
	throw std::exception("Divide by zero exception");
	return false;
}


bool AddOK()
{
	int expected = 5;

	return (expected == AddInts(2, 3));
}

bool AddFail()
{
	int expected = 2;

	return (expected == AddInts(2, 3));
}

