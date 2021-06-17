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
    : _numMsgsSent{ 0 }
    , _msgsRcvd{ BlockingQueue<Message>() }
    , _shutdown{ false }
{}

void TestServer::StartServer()
{
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
                _shutdown = true;
                break;
            }

            { // critical section
                std::unique_lock<std::mutex> l(_mtx);
                _tests.emplace_back(TestClass());
                ThreadPool<4>::CallObj t = [this, msg]() ->bool { _tests.back().RunTest(msg); return true; };
                trpl.workItem(t);
            } // end critical section

            if (_shutdown) { // safe since _shutdown is atomic
                std::cout << "\nServer shutting down...\n";
                break;
            }
        }

        // Wait for all tests to complete
        ThreadPool<4>::CallObj exit = []() ->bool { return false; };
        trpl.workItem(exit); // ensures all threads terminate
        trpl.wait();
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