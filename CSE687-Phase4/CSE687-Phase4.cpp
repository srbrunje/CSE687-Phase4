// CSE687_Phase4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


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


using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;



struct TestComponents {

    std::string testName;
    std::string dllName;
    std::string funcName;
    EndPoint clientEP;
    EndPoint serverEP;
    bool completed = 0;
    LogLevel _logLevel;
};

std::vector<TestComponents> tests;


void AddTestClient(std::vector<TestComponents>& tests) {


    TestComponents component;
    std::string testName{ "Leo" };
    int choice, clientIP, serverIP{ 9890 }, confirm{ 0 }, loglevel{ 0 };

    while (confirm == 0) {

        std::cout << "TestName: ";
        std::getline(std::cin, testName);
        component.testName = testName;

        std::cout << "Choose DLL:\n";
        std::cout << "1). LongRunTestDLL.dll  ||   2). MathDLL.dll" << std::endl;
        std::cin >> choice;
        component.dllName = (choice == 1) ? "LongRunTestDLL.dll" : "MathDLL.dll";

        std::cout << "Choose FuncName:\n";
        if (choice == 1)
        {
            std::cout << "1). LongRunTest  ||   2). ShortRunTest" << std::endl;
            std::cin >> choice;
            component.funcName = (choice == 1) ? "LongRunTest" : "ShortRunTest";
        }
        else
        {
            std::cout << "1). AddOK  ||   2). AddFail || 3). DivZero" << std::endl;
            std::cin >> choice;
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


        std::cout << "Client IP: ";
        std::cin >> clientIP;
        component.serverEP = EndPoint("localhost", serverIP);
        component.clientEP = EndPoint("localhost", (int)clientIP);


        std::cout << "Log level\n";
        std::cout << "================\n";
        std::cout << "1). Pass/Fail - Shows only fail or pass status\n";
        std::cout << "2). Pass/Fail with message - Shows only fail or pass status with message log\n";
        std::cout << "3). Pass/Fail - Shows only fail with test duration\n";
        std::cout << "4). Pass/Fail With error message and test duration\n";
        std::cin >> loglevel;

        while (loglevel <= 0 && loglevel > 4) {
            if (loglevel == 1) component._logLevel = LogLevel::STATUS_ONLY;
            else if (loglevel == 2) component._logLevel = LogLevel::WITH_ERR;
            else if (loglevel == 3) component._logLevel = LogLevel::WITH_DUR;
            else if (loglevel == 4) component._logLevel = LogLevel::ALL;
            else {
                std::cout << "Invalid choice, please try again!\n";
            }
        }

        std::cout << "Will you like to save your changes? [YES = 1, NO = 0]";
        std::cin >> confirm;

        std::cin.ignore();
    }

    tests.push_back(component);


    system("pause");

}


void EditTest(std::vector<TestComponents>& tests) {

    int choice{ 0 };
    std::cout << "\Edit TESTS\n**This action can not be undone\nTo exit, insert -1\n\n\n";

    for (int x = 0; x < tests.size(); x++) {
        std::cout << "[" << x + 1 << "]" <<
            "Test Name: " << tests[x].testName << std::endl;
    }

    std::cout << "\n\nTest to Edit: (1 - " << tests.size() << "): ";
    std::cin >> choice;


    if (choice != -1) {

        std::string testName{ "Leo" };
        int clientIP, confirm{ 0 }, loglevel{ 0 };

        // testing | ignore my horrible var

        while (confirm == 0) {

            std::cout << "TestName: ";
            std::getline(std::cin, testName);
            tests[choice].testName = testName;

            std::cout << "Choose DLL:\n";
            std::cout << "1). LongRunTestDLL.dll  ||   2). MathDLL.dll" << std::endl;
            std::cin >> choice;
            tests[choice].dllName = (choice == 1) ? "LongRunTestDLL.dll" : "LongRunTestDLL.dll";

            std::cout << "Choose FuncName:\n";
            std::cout << "1). LongRunTest  ||   2). ShortRunTest" << std::endl;
            std::cin >> choice;
            tests[choice].funcName = (choice == 1) ? "LongRunTest" : "ShortRunTest";


            std::cout << "Client IP: ";
            std::cin >> clientIP;
            tests[choice].clientEP = EndPoint("localhost", (int)clientIP);


            std::cout << "Log level\n";
            std::cout << "================\n";
            std::cout << "1). Pass/Fail - Shows only fail or pass status\n";
            std::cout << "2). Pass/Fail with message - Shows only fail or pass status with message log\n";
            std::cout << "3). Pass/Fail - Shows only fail with test duration\n";
            std::cout << "4). Pass/Fail With error message and test duration\n";
            std::cin >> loglevel;

            while (loglevel <= 0 && loglevel > 4) {
                if (loglevel == 1) tests[choice]._logLevel = LogLevel::STATUS_ONLY;
                else if (loglevel == 2) tests[choice]._logLevel = LogLevel::WITH_ERR;
                else if (loglevel == 3) tests[choice]._logLevel = LogLevel::WITH_DUR;
                else if (loglevel == 4) tests[choice]._logLevel = LogLevel::ALL;
                else {
                    std::cout << "Invalid choice, please try again!\n";
                }
            }

            std::cout << "Will you like to save your changes? [YES = 1, NO = 0]";
            std::cin >> confirm;

            std::cin.ignore();
        }




    }
    system("cls");

}



void RemoveTestClient(std::vector<TestComponents>& tests) {

    int choice{ 0 };
    std::cout << "\nREMOVE TESTS\n**This action can not be undone\nTo exit, insert -1\n\n\n";

    for (int x = 0; x < tests.size(); x++) {
        std::cout << "[" << x + 1 << "]" <<
            "Test Name: " << tests[x].testName << std::endl;
    }

    std::cout << "\n\nTest to remove: (1 - " << tests.size() << "): ";
    std::cin >> choice;


    if (choice != -1) {
        tests.erase(tests.begin() + (choice - 1));
    }
    system("cls");

}


void ViewTestClients(std::vector<TestComponents>& tests) {

    int choice{ 0 };
    std::cout << "\nVIEW TESTS\n\n\n";

    for (int x = 0; x < tests.size(); x++) {
        std::cout << "[" << x + 1 << "]" <<
            "Test Name: " << tests[x].testName << " || " <<
            "Dll Name: " << tests[x].dllName << " || " <<
            "Func Name: " << tests[x].funcName << " || " <<
            "Client Enpoint: " << tests[x].clientEP.port << " || " <<
            "Completed: " << tests[x].completed << std::endl;

        std::cout << "====================================================================================================================\n\n";
    }

    system("pause");

}



void RunTestClients(std::vector<TestComponents>& tests) {

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

        if (tests[x].completed != 1) {

            TestClient testClient = TestClient(tests[x].testName, tests[x].clientEP, tests[x].serverEP);
            testClient.SetOutputFile("_clientoutput.txt");
            testClient.StartTest(tests[x].dllName, tests[x].funcName, tests[x]._logLevel);
            tests[x].completed = 1;
            testClient.ReportResults();
        }
    }



    StaticLogger<1>::flush();
    std::cout << "\n  press enter to quit test Harness";
    _getche();

    system("pause");




}


void ViewResults() {

    TestLogger logger;
    logger.GetOutputFile();

}


void action(int choice, std::vector<TestComponents>& tests) {


    switch (choice) {
    case 1:
        AddTestClient(tests);
        break;
    case 2:
        RemoveTestClient(tests);
        break;
    case 3:
        ViewTestClients(tests);
        break;
    case 4:
        RunTestClients(tests);
        break;
    case 5:
        exit(1);
        break;
    default:
        std::cout << "Invalid Choice!" << std::endl;
    }
}


int Menu() {

    int choice{ 0 };

    do {
        // displaying choices
        system("cls");

        std::cout << "CSE687-Phase4\n";
        std::cout << "TestComponents: " << tests.size();
        std::cout << "\n\n========================\n\n";


        std::cout << "Please select from the following options\n\n";

        std::cout << "1) Add Tests\n";
        std::cout << "2) Remove Tests\n";
        std::cout << "3) View Tests\n";
        std::cout << "4) Run Tests\n";
        std::cout << "5) Exit\n";

        std::cout << "\nChoice[1-5]: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cin.ignore();

    } while ((choice > 5 || choice <= 0)); // keep on calling this loop until user selects valid choice

    return choice;

}




int main() {


    //start the server
    TestServer testServer = TestServer();
    testServer.StartServer();

    int choice{ 0 }; // holds user choice value

    do {

        choice = Menu();
        action(choice, tests);

    } while (choice != 5);



    return 0;
}