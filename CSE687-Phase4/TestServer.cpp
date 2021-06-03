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
{
    std::cout << "create test server class\n";
    _shutdown = false;
    _mgr = TestManager::GetInstance();
    _numMsgsSent = 0;
}


/// <summary>
/// setup all of the tests
/// </summary>
void TestServer::InitilizeTestServer()
{
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();

    //_mgr.SetOutputFile("output.txt"); // Define your output here, either relative or absolute
    _mgr->SetOutputToStream(true);


    // Math with integers
    AddFunctor<int> iAdd(4, 0, 4); // should PASS 4 + 0 == 4
    SubFunctor<int> iSub(4, 0, 4); // should PASS 4 - 0 == 4
    MulFunctor<int> iMul(4, 0, 4); // should FAIL 4 * 0 != 4
    DivFunctor<int> iDiv(4, 0, 4); // should FAIL with divide by zero exception

    LongRunFunctor lRun1(1);
    LongRunFunctor lRun2(2);
    LongRunFunctor lRun3(3);
    LongRunFunctor lRun4(4);

    _mgr->CreateTest(iAdd, LogLevel::Pass_Fail, "Add: 4+0=4", "This should never happen");
    _mgr->CreateTest(iSub, LogLevel::Pass_Fail_with_error_message, "Sub: 4-0=4", "This should never happen");
    _mgr->CreateTest(iMul, LogLevel::Pass_Fail_with_error_message_and_test_duration, "Mul: 4*0=4", "This test should fail");
    _mgr->CreateTest(iDiv, LogLevel::Pass_Fail_with_error_message_and_test_duration, "Div: 4/0=4", "This test should fail");

    _mgr->CreateTest(lRun1, LogLevel::Pass_Fail_with_error_message, "LongRun1");
    _mgr->CreateTest(lRun2, LogLevel::Pass_Fail_with_error_message, "LongRun2");
    _mgr->CreateTest(lRun3, LogLevel::Pass_Fail_with_error_message, "LongRun3");
    _mgr->CreateTest(lRun4, LogLevel::Pass_Fail_with_error_message, "LongRun4");
}




bool runTest(TestManager* _mgr, int testNumber, EndPoint requestor)
{
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();
    _mgr->RunTest(testNumber,requestor);
    return true;
}


void ListenerThread(TestManager* _mgr)
{
  //  DebugLog::attach(&std::cout);
  //  DebugLog::start();

    //create the thread pool
    ThreadPool<4> trpl;
       

    EndPoint serverEP("localhost", 9890);
   

    Comm comm(serverEP, "Server Listener");

    comm.start();

    Message msg, rply;
    rply.SetName("reply");
    size_t count = 0;
    while (true)
    {
        // display each incoming message

        msg = comm.getMessage();
        std::cout << "\n  " + comm.name() + " received Test Request: " << msg.GetName() + " From: " +
            msg.GetAuthor();

        if (msg.GetCommand() == "stop")
        {
            break;
        }

        std::string DLLName = msg.GetAuthor();
        std::string FuncName = msg.GetName();

        EndPoint requester =  msg.GetFrom();

        ThreadPool<4>::CallObj t = [_mgr, DLLName, FuncName,requester]() ->bool { _mgr->RunDLL(DLLName,FuncName,requester); return true; };
        trpl.workItem(t);
       
        /*

        //get the test number
        if (_mgr == nullptr) _mgr = TestManager::GetInstance();
        int testNumber = _mgr->FindTestNumber(msg.GetName());

        if (testNumber >= 0)
        {
            //set the new log level
           

           //run the test
            ThreadPool<4>::CallObj t = [_mgr,testNumber]() ->bool { _mgr->RunTest(testNumber); return true; };
            trpl.workItem(t);


        //   std::thread t1(runTest, _mgr, testNumber);
        //   t1.detach();

           //_mgr->runTest(testNumber);

        }
        */

       

    }
    trpl.wait();

    comm.stop();

}

void TestServer::StartServer()
{
 
    //initilize the tests
    InitilizeTestServer();

    //start the listner thread
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();
    std::thread testServer(ListenerThread,_mgr);
    testServer.detach();   
    
}

void TestServer::StopServer()
{
}

/*
bool TestServer::SetOutputFile(const std::string& aFilePath)
{
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();
    return _mgr->SetOutputFile(aFilePath);
}
void TestServer::SetOutputToFile(const bool bOutput)
{
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();
    _mgr->SetOutputToFile(bOutput);
}
void TestServer::SetOutputStream(std::ostream& aStream)
{
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();
    _mgr->SetOutputStream(aStream);
}
void TestServer::SetOutputToStream(const bool bOutput)
{
    if (_mgr == nullptr) _mgr = TestManager::GetInstance();
    _mgr->SetOutputToStream(bOutput);
}


void TestServer::StartTest(const std::string& aTestName, const LogLevel aLogLevel)
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Request");
    comm.start();
    EndPoint serverEP("localhost", 9890);
    EndPoint clientEP("localhost", 9891);

    // create the message
    Message testRequest(serverEP, clientEP);
    testRequest.SetName(aTestName);
    testRequest.SetValue("logLevel", (int)aLogLevel);

    // send the message
    _numMsgsSent++;
    comm.postMessage(testRequest);
    comm.stop();
}
*/
/*
void TestServer::StopTest()
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Request");
    comm.start();
    EndPoint serverEP("localhost", 9890);
    EndPoint clientEP("localhost", 9891);

    // create the message
    Message testRequest(serverEP, clientEP);
    testRequest.SetName("stop"); //the name of the test to run
    testRequest.SetCommand("stop");

    // send the message
    comm.postMessage(testRequest);
    comm.stop();
}




void TestServer::ProcessReplies()
{
    EndPoint serverEP("localhost", 9893);
    Comm comm(serverEP, "Client Status");
    comm.start();

    Message msg, rply;
    rply.SetName("reply");
    size_t count = 0;
    while (true)
    {
        // display each incoming message, and track the number
        msg = comm.getMessage();
        std::cout << "\n" + comm.name() + " received Test Result: " << msg.GetName();

        if (msg.GetCommand() == "stop") {
            break;
        }

        _msgsRcvd.enQ(msg);
    }
    comm.stop();
}

void TestServer::ReportResults()
{
    // Wait until all replies have been heard - or a max waiting time is up
    timing::hack beginWait = timing::now();
    while (_msgsRcvd.size() < _numMsgsSent) {
        if (timing::duration_us(beginWait) > 10000000) { // 10s wait limit
            break;
        }
    }

    // Go through all results and log them
    if (_mgr == nullptr) {
        _mgr = TestManager::GetInstance();
    }
    TestLogger* lgr = _mgr->GetLoggerPtr();
    int i = 1;
    lgr->ClearContents(); // erase any old data stored in the output file (if any)
    while(_msgsRcvd.size() > 0) {
        lgr->LogMessage("\n--------------------------------------------------------------\nTest "
            + std::to_string(i++) + " of " + std::to_string(_numMsgsSent) + ":\n\n");
        lgr->LogResult(_msgsRcvd.deQ());
        lgr->LogMessage("--------------------------------------------------------------\n\n");
    }
    
} */