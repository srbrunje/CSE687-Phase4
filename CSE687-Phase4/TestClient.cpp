#include "TestClient.h"

using namespace MsgPassingCommunication;

TestClient::TestClient(const std::string clientName, const EndPoint clientEP)
{
    std::cout << "create test client class\n";    
    _numMsgsSent = 0;
    _myName = clientName;
    _myPort = clientEP.port;

    _clientEP = clientEP;// EndPoint("localhost", _myPort);

    _serverEP = EndPoint("localhost", 9890);

    StartListenerThread();
}

void TestClient::SetClientName(const std::string name)
{
    _myName = name;
}

std::string TestClient::GetClientName()
{
    return _myName;
}



void TestClient::SetServerEndpoint(const EndPoint endPoint)
{
    _serverEP = endPoint;
}

void TestClient::SetClientEndpoint(const EndPoint endPoint)
{
    _clientEP = endPoint;
}

EndPoint TestClient::GetClientEndPoint()
{
    return _clientEP;
}

EndPoint TestClient::GetServerEndPoint()
{
    return _serverEP;
}


bool TestClient::SetOutputFile(const std::string& aFilePath)
{
    return _lgr.SetOutputFile(aFilePath);
}

void TestClient::SetOutputToFile(const bool bOutput)
{
    _lgr.SetOutputToFile(bOutput);
}

void TestClient::SetOutputStream(std::ostream& aStream)
{
    _lgr.SetOutputStream(aStream);
}

void TestClient::SetOutputToStream(const bool bOutput)
{
    _lgr.SetOutputToStream(bOutput);
}


void TestClient::StartTest(const std::string& aTestName, const LogLevel aLogLevel)
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Request");
    comm.start();

    // create the message
    Message testRequest(_serverEP, _clientEP);
    testRequest.SetName(aTestName);
    testRequest.SetValue("logLevel", (int)aLogLevel);

    testRequest.SetAuthor(_myName);

    // send the message
    _numMsgsSent++;
    comm.postMessage(testRequest);
    comm.stop();
}

// Sends a message to the server to indicate that no more test requests will be comming from this client
void TestClient::StopTest()
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Stop Request");
    comm.start();    

    // create the message
    Message testRequest(_serverEP, _clientEP);
    testRequest.SetName("stop"); //the name of the test to run
    testRequest.SetCommand("stop");
    testRequest.SetAuthor(_myName);

    // send the message
    comm.postMessage(testRequest);
    comm.stop();
}

//starts the listener thread to handle replies from the test server
void TestClient::StartListenerThread()
{
    std::thread reply(&TestClient::ProcessReplies, this);
    reply.detach();
}

// process the replies from the test server
void TestClient::ProcessReplies()
{
    EndPoint listenerServerEP("localhost", 9893);
    Comm comm(listenerServerEP, "Client Status");
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

// genreates a report of all of the tests that this client as send and recieved replies for
void TestClient::ReportResults()
{
    // Wait until all replies have been heard - or a max waiting time is up
    timing::hack beginWait = timing::now();
    while (_msgsRcvd.size() < _numMsgsSent) {
        if (timing::duration_us(beginWait) > 10000000) { // 10s wait limit
            break;
        }
    }

    int i = 1;
    _lgr.ClearContents(); // erase any old data stored in the output file (if any)
    while (_msgsRcvd.size() > 0) {
        _lgr.LogMessage("\n--------------------------------------------------------------\nTest "
            + std::to_string(i++) + " of " + std::to_string(_numMsgsSent) + ":\n\n");
        _lgr.LogResult(_msgsRcvd.deQ());
        _lgr.LogMessage("--------------------------------------------------------------\n\n");
    }
}
