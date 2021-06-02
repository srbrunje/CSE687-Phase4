//// CallableTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
//#include <functional>
//#include <iostream>
//#include "callable.h"
//#include "TestClass.h"
//#include "TestManager.h"
//
//int main()
//{
//    callable test = callable(5, 6, 11);
//
//    bool rtn = test.add();
//
//    std::cout << "Test was " << rtn << " \n";
//
//    callable test2(5, 5, 11);
//
//    std::cout << "Test was " << test2() << " \n";
//
//    TestClass tc1 = TestClass(test2,LogLevel::Pass_Fail_with_error_message,"Test 1");
//
//    bool r = tc1.GetTestMethod()();
//
//    std::cout << tc1.GetName() << " was run at " << tc1.GetLogLevel() << " log level and " <<
//        tc1.GetTestResult() << " " << r << "\n";
//
//    TestManager tm = TestManager();
//
//    tm.CreateTest(test2);
//
//    tm.ExecuteTests();
//
//    
//}
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
