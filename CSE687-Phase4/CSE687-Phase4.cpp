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

int main()
{
    SocketSystem ss;

    SUtils::Title("Start Test");

    Utilities::putline();

    StaticLogger<1>::attach(&std::cout);


    // Remove comment below to show extra details
    //StaticLogger<1>::start();

    //start the server
    TestServer testServer = TestServer();
    testServer.StartServer();


    EndPoint client1EP = EndPoint("localhost", 9891);

    //create the client
    TestClient testClient = TestClient("Client 1", client1EP);

    testClient.SetOutputFile("_clientoutput.txt");

   // testClient.StartTest("LongRun4", LogLevel::Pass_Fail);

    testClient.StartTest("LongRunTestDLL.dll", "LongRunTest", LogLevel::Pass_Fail);

    //request some tests
/*    testClient.StartTest("LongRun4", LogLevel::Pass_Fail);
    testClient.StartTest("LongRun3", LogLevel::Pass_Fail_with_error_message);
    testClient.StartTest("LongRun2", LogLevel::Pass_Fail_with_test_duration);
    testClient.StartTest("LongRun1", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    testClient.StartTest("Add: 4+0=4", LogLevel::Pass_Fail_with_error_message);
    testClient.StartTest("Mul: 4*0=4", LogLevel::Pass_Fail_with_test_duration);
    testClient.StartTest("Div: 4/0=4", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    testClient.StartTest("LongRun4", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    testClient.StartTest("Add: 4+0=4", LogLevel::Pass_Fail_with_error_message);
    testClient.StartTest("Mul: 4*0=4", LogLevel::Pass_Fail_with_test_duration);
    testClient.StartTest("Sub: 4-0=4", LogLevel::Pass_Fail);
    testClient.StartTest("Div: 4/0=4", LogLevel::Pass_Fail);
    */
    testClient.StopTest();
    

    //get the results
    testClient.ReportResults();


    StaticLogger<1>::flush();
    std::cout << "\n  press enter to quit test Harness";
    _getche();


    return 0;
}

