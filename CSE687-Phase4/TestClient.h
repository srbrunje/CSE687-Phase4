#pragma once

#include "Comm.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>
#include "ClientHandler.h"
#include "TestLogger.h"


class TestClient
{
public:
	TestClient(const std::string clientName, const EndPoint clientEP, const EndPoint serverEP);
	
	void SetName(const std::string name);
	void SetEndpoint(const EndPoint endPoint);
	void SetServerEndpoint(const EndPoint endPoint);

	std::string GetClientName();
	EndPoint GetEndPoint();
	EndPoint GetServerEndPoint();

	bool SetOutputFile(const std::string& aFilePath);
	void SetOutputToFile(const bool bOutput);
	void SetOutputStream(std::ostream& aStream);
	void SetOutputToStream(const bool bOutput);

	void StartTest(const std::string& aDLLName, const std::string& aFuncName, const LogLevel aLogLevel);
	void ReportResults();

private:
	void StartListenerThread();

	std::string _name;
	EndPoint _EP;
	EndPoint _serverEP;
	int _numMsgsSent;
	BlockingQueue<Message> _msgsRcvd;
	TestLogger _logger;
};

