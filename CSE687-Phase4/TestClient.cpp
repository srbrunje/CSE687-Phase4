#include "TestClient.h"

using namespace MsgPassingCommunication;


TestClient::TestClient(const std::string clientName, const EndPoint clientEP, const EndPoint serverEP)
{
    _numMsgsSent = 0;
    _name = clientName;
    _EP = clientEP;
    _serverEP = serverEP;
    StartListenerThread();
}

void TestClient::SetName(const std::string name)
{
    _name = name;
}
void TestClient::SetEndpoint(const EndPoint endPoint)
{
    _EP = endPoint;
}
void TestClient::SetServerEndpoint(const EndPoint endPoint)
{
    _serverEP = endPoint;
}


std::string TestClient::GetClientName()
{
    return _name;
}
EndPoint TestClient::GetEndPoint()
{
    return _EP;
}
EndPoint TestClient::GetServerEndPoint()
{
    return _serverEP;
}


bool TestClient::SetOutputFile(const std::string& aFilePath)
{
    return _logger.SetOutputFile(aFilePath);
}
void TestClient::SetOutputToFile(const bool bOutput)
{
    _logger.SetOutputToFile(bOutput);
}
void TestClient::SetOutputStream(std::ostream& aStream)
{
    _logger.SetOutputStream(aStream);
}
void TestClient::SetOutputToStream(const bool bOutput)
{
    _logger.SetOutputToStream(bOutput);
}





void TestClient::StartTest(const std::string& aDLLName, const std::string& aFuncName, const LogLevel aLogLevel)
{
    //create the comm connection
    Comm comm(_serverEP, "Send Test Request");
    comm.start();

    // create the message
    Message testRequest(_serverEP, _EP);
    testRequest.SetName(aFuncName);
    testRequest.SetDLL(aDLLName);
    testRequest.SetLogLevel(aLogLevel);

    // send the message
    _numMsgsSent++;
    comm.postMessage(testRequest);
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
    _logger.ClearContents(); // erase any old data stored in the output file (if any)
    while (_msgsRcvd.size() > 0) {
        _logger.LogMessage("\n--------------------------------------------------------------\n");
        _logger.LogResult(_msgsRcvd.deQ());
        _logger.LogMessage("--------------------------------------------------------------\n\n");
    }
}




//starts the listener thread to handle replies from the test server
void TestClient::StartListenerThread()
{
    std::function<void()> listen = [this] {
        Comm comm = Comm(_EP, "Client Status");
        comm.start();

        Message msg;
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
    };
    std::thread listener(listen);
    listener.detach();
}

