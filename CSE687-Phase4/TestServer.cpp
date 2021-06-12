#include "TestServer.h"
#include "..\ThreadPool\ThreadPool.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>

using namespace MsgPassingCommunication;

TestServer::TestServer()
    : _mgr{ TestManager() }
    , _numMsgsSent{ 0 }
    , _msgsRcvd{ BlockingQueue<Message>() }
    , _shutdown{ false }
{}


void TestServer::InitilizeTestServer()
{
    //_mgr.SetOutputFile("output.txt"); // Define your output here, either relative or absolute
    _mgr.SetOutputToStream(true);

    // Math with integers
    AddFunctor<int> iAdd(4, 0, 4);  // should PASS 4 + 0 == 4
    SubFunctor<int> iSub(4, 0, 4);  // should PASS 4 - 0 == 4
    MulFunctor<int> iMul(4, 0, 4);  // should FAIL 4 * 0 != 4
    DivFunctor<int> iDiv(4, 0, 4);  // should FAIL with divide by zero exception
    LongRunFunctor lRun1(1);        // long run, factor of 1
    LongRunFunctor lRun2(2);        // long run, factor of 2
    LongRunFunctor lRun3(3);        // long run, factor of 3
    LongRunFunctor lRun4(4);        // long run, factor of 4

    _mgr.CreateTest(iAdd, LogLevel::STATUS_ONLY, "Add: 4+0=4", "This should never happen");
    _mgr.CreateTest(iSub, LogLevel::WITH_DUR, "Sub: 4-0=4", "This should never happen");
    _mgr.CreateTest(iMul, LogLevel::WITH_ERR, "Mul: 4*0=4", "This test should fail");
    _mgr.CreateTest(iDiv, LogLevel::ALL, "Div: 4/0=4", "This test should fail");
    _mgr.CreateTest(lRun1, LogLevel::ALL, "LongRun1");
    _mgr.CreateTest(lRun2, LogLevel::ALL, "LongRun2");
    _mgr.CreateTest(lRun3, LogLevel::ALL, "LongRun3");
    _mgr.CreateTest(lRun4, LogLevel::ALL, "LongRun4");
}

void TestServer::StartServer()
{
    //initilize the tests
    InitilizeTestServer();

    std::function<void()> listen = [this] {
        ThreadPool<4> trpl;
        Comm comm(EndPoint(ADDRESS, PORT), "Server");
        comm.start();

        Message msg;
        size_t count = 0;
        while (true)
        {
            // display each incoming message
            msg = comm.getMessage();
            std::cout << "\n  " + comm.name() + " received Test Request: " << msg.GetName() + " From: " +
                msg.GetAuthor();

            if (msg.GetCommand() == "stop") {
                break;
            }

            ThreadPool<4>::CallObj t = [this, msg]() ->bool { _mgr.RunTest(msg); return true; };
            trpl.workItem(t);

            if (_shutdown) { // safe since _shutdown is atomic
                std::cout << "\nServer shutting down...\n";
                break;
            }
        }
        trpl.wait();
        comm.stop();
    };

    //start the listner thread
    _shutdown = false;
    std::thread testServer(listen);
    testServer.detach();
}

void TestServer::StopServer(const bool bHard)
{
    if (bHard) {
        _shutdown = true; // safe since _shutdown is atomic
    }
    else {
        Message stop;
        stop.SetCommand("stop");
        Comm comm(EndPoint(ADDRESS, PORT), "Server");
        comm.start();
        comm.postMessage(stop);
        comm.stop();
    }
}