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


    EndPoint client1EP = EndPoint("localhost", 9892);
    EndPoint serverEP = EndPoint("localhost", 9890);

    EndPoint client2EP = EndPoint("localhost", 9894);

    //create the client
    TestClient testClient = TestClient("Client 1", client1EP, serverEP);

    testClient.SetOutputFile("_clientoutput.txt");

    TestClient testClient2 = TestClient("Client 2", client2EP, serverEP);

   // testClient.StartTest("LongRun4", LogLevel::Pass_Fail);

   testClient.StartTest("LongRunTestDLL.dll", "LongRunTest", LogLevel::Pass_Fail_with_error_message_and_test_duration);
   testClient.StartTest("LongRunTestDLL.dll", "ShortRunTest", LogLevel::Pass_Fail);

  // testClient2.StartTest("LongRunTestDLL.dll", "LongRunTest", LogLevel::Pass_Fail);
 //  testClient2.StartTest("LongRunTestDLL.dll", "ShortRunTest", LogLevel::Pass_Fail);
       
    testClient.StopTest();
  //  testClient2.StopTest();
    

    //get the results
    testClient.ReportResults();

  //  testClient2.ReportResults();

    StaticLogger<1>::flush();
    std::cout << "\n  press enter to quit test Harness";
    _getche();


    return 0;
}

