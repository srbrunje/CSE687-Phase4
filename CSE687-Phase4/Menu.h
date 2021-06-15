#ifndef __MENU_H
#define __MENU_H

#include "Comm.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>
#include "TestManager.h"
#include "ExampleTest.h"
#include "ClientHandler.h"
#include "TestServer.h"
#include "TestClient.h"
#include <map>


// Prototypes
int GetInt();
int GetIntInBounds(const int aMin, const int aMax, const int aIgnore);
void RunMenu();
void AddTests();
void RemoveTests();
void ViewTests();
void RunTests();
void Exit();

// NameSpaces
using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;


typedef void (*Menu_Processing_Function_Ptr)(); // pointer to function


// Our Test class data members
struct TestComponents {
    std::string testName;
    std::string dllName;
    std::string funcName;
    EndPoint clientEP;
    EndPoint serverEP;
    bool completed = 0;
};

std::vector<TestComponents> tests; // contains all local tests


// Contains our function and function titles
struct Menu_Item_Attributes
{
    const char* text;
    Menu_Processing_Function_Ptr p_processing_function;
};


typedef std::map<unsigned int, Menu_Item_Attributes> Menu_Item_Container; // list of functions



// ADD TESTS
void AddTests() {

    TestComponents component;
    std::string testName{ "Leo" };
    int choice{-1}, clientIP, serverIP{ 9890 }, confirm{ 0 };

    while (confirm == 0) {

        // Getting the test name
        std::cout << "TestName: ";
        std::getline(std::cin, testName);
        component.testName = testName;


        while (choice == -1) {
            // User choice of DLL
            std::cout << "Choose DLL:\n";
	        std::cout << "1). LongRunTestDLL.dll  ||   2). MathDLL.dll\n[Choice]: ";

            choice = GetInt();
            component.dllName = (choice == 1) ? "LongRunTestDLL.dll" : "MathDLL.dll";
        }
		
		if (choice == 1)
        {
			// User choice of funcName
            std::cout << "Choose FuncName:\n";
            std::cout << "1). LongRunTest  ||   2). ShortRunTest\n[Choice]: ";
            choice = GetInt();
            component.funcName = (choice == 1) ? "LongRunTest" : "ShortRunTest";
        }
        else
        {
            std::cout << "1). AddOK  ||   2). AddFail || 3). DivZero" << std::endl;
            choice = GetInt();
            switch (choice)
            {
            case 1:
                component.funcName = "AddOK";
                break;
            case 2:
                component.funcName = "AddFail";
                break;
            case 3:
                component.funcName = "DivZero";
                break;
            default:
                component.funcName = "AddOK";
            }
        }

        // User Port Number
        std::cout << "Client Port: ";
        std::cin >> clientIP;
        component.serverEP = EndPoint("localhost", serverIP);
        component.clientEP = EndPoint("localhost", (int)clientIP);



        std::cout << "Will you like to save your changes? [YES = 1, NO = 0]";
        confirm = GetInt();

        std::cin.ignore();
    }

    tests.push_back(component);


    system("pause");
}


// REMOVE TESTS
void RemoveTests() {

    if (tests.size() > 0) {

        int choice{ 0 };

        std::cout << "\nREMOVE TESTS\n**This action can not be undone\nTo exit, insert -1\n\n\n";

        for (int x = 0; x < tests.size(); x++)  std::cout << "[" << x + 1 << "]" << "Test Name: " << tests[x].testName << std::endl; // displaying tests

        std::cout << "\n\nTest to remove: (1 - " << tests.size() << "): ";

        // Params: LowerBound/HigherBound/Ignore
        choice = GetIntInBounds(-1, tests.size(), 0);


        if (choice > 0) tests.erase(tests.begin() + (choice - 1)); // if choice is to remove then proceed


    }
    else {
        std::cout << "There is no test to remove since no test have yet been created! Please create a test!\n";
        system("pause");
    }


    system("cls");

}

// VIEW TESTS
void ViewTests() {

    std::cout << "\nVIEW TESTS\n\n\n";

    if (tests.size() > 0) {
        for (int x = 0; x < tests.size(); x++) {
            std::cout << "[" << x + 1 << "]" <<
                "Test Name: " << tests[x].testName << " || " <<
                "Dll Name: " << tests[x].dllName << " || " <<
                "Func Name: " << tests[x].funcName << " || " <<
                "Client Enpoint: " << tests[x].clientEP.port << " || " <<
                "Completed: " << tests[x].completed << std::endl;

            std::cout << "====================================================================================================================\n\n";
        }
    }
    else {
        std::cout << "There has been no test created! Please create a test first in order for you to continue!\n\n";
    }

    system("pause");

}

// RUN TESTS
void RunTests() {

    if (tests.size() > 0) {

        int choice{ 0 };
        std::cout << "\RUNNING TESTS\n\n\n";

        SocketSystem ss;
        SUtils::Title("Start Test");
        Utilities::putline();
        StaticLogger<1>::attach(&std::cout);

        // Remove comment below to show extra details
        //StaticLogger<1>::start();

        //start the server
        TestServer testServer = TestServer();
        testServer.StartServer();


        for (int x = 0; x < tests.size(); x++) {

            TestClient testClient = TestClient(tests[x].testName, tests[x].clientEP, tests[x].serverEP);
            testClient.SetOutputFile("_clientoutput.txt");
            testClient.StartTest(tests[x].dllName, tests[x].funcName, LogLevel::Pass_Fail_with_error_message_and_test_duration);
            testClient.StopTest();

            tests[x].completed = 1;

            testClient.ReportResults();
        }



        StaticLogger<1>::flush();
        std::cout << "\n  press enter to quit test Harness";
        _getche();
    }
    else {
        std::cout << "There has been no test created! Please create a test first in order for you to continue!\n";
    }

    system("pause");

}

// EXIT PROGRAM
void Exit() {
    exit(1);
}


// RUN THE MENU
void RunMenu() {

    Menu_Item_Attributes add = { "Add Test", AddTests };
    Menu_Item_Attributes remove = { "Remove Test", RemoveTests };
    Menu_Item_Attributes view = { "View Tests", ViewTests };
    Menu_Item_Attributes run = { "Run Tests", RunTests };
    Menu_Item_Attributes Quit = { "Exit Program", Exit };

    Menu_Item_Container menu = { {1, add}, {2, remove},{3, view},{4, run}, {5, Quit} };
    int selection{ 0 };


    std::cout << "CSE687-Phase4\n";
    std::cout << "TestComponents: " << tests.size();
    std::cout << "\n\n========================\n\n";


    std::cout << "Please select from the following options\n\n";

    for (const auto& p : menu) {
        std::cout << "[" << p.first << "] = " << p.second.text << '\n';
    }



    std::cout << "Enter selection: ";
    selection = GetInt();

    try {
        system("cls");
        Menu_Item_Attributes attributes = menu.at(selection);
        (attributes.p_processing_function)();
    }
    catch (std::out_of_range& e) {
        std::cout << "Invalid selection" << std::endl;
        std::cin.clear();
        //exit(1);
    }


    // return selection;

}



/// <summary>
/// ZACH DEMERS IMPLEMENTATION OF SANITY CHECKS
/// </summary>
/// <returns></returns>
int GetInt()
{
    std::string input;				// initialize string for input storage
    std::getline(std::cin, input);	// store user input into local variable string
    if (!input.empty()) {	// if the input string is not empty
        // Use a try-catch block to ensure proper user input to match the data type
        try {
            int i = std::stoi(input);	// convert the input into an integer (may throw an exception below)
            return i;					// return the integer
        }
        catch (const std::invalid_argument& invalArg) { // thrown when containing non-numeric characters, etc.
            std::cerr << "Error: " << invalArg.what() << " for string \'" << input << "\'\n\n";
        }
        catch (const std::out_of_range& outOfRng) {		// thrown when magnitude is too large for int type
            std::cerr << "Error: " << outOfRng.what() << " for string \'" << input << "\'\n\n";
        }
    }
    // If the input was empty or an invalid/out-of-range value was entered, return -1 as an error value
    return -1;
}
int GetIntInBounds(const int aMin, const int aMax, const int aIgnore)
{
    // Safely retrieve a value from std::cin
    int in = GetInt();
    // Check if value retrieved was within bounds
    while (in < aMin || in > aMax || in == aIgnore) {
        // If not within bounds, request a valid user input
        std::cout << "Please enter a valid option from "
            << aMin << " to " << aMax << ": ";
        in = GetInt();	// safely retrieve another value from std::cin
    }
    return in;
}



#endif
