// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "LongRunTestDLL.h"
#include <iostream>


bool LongRunTest()
{
    int _A = 4;
    int _B = 500;

    if (_A < 1) _A = 1;
    if (_B < 200) _B = 200;

    std::cout << "New Start\n";
    for (int i = 0; i < _A; i++)
    {
        ::Sleep(_B);
    }
    std::cout << "New END\n";
    return true;
}


