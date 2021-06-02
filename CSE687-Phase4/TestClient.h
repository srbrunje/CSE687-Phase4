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
#include "TestManager.h"
#include "ExampleTest.h"
#include "ClientHandler.h"

class TestClient
{
public:
	TestClient(const std::string clientName, const EndPoint clientEP);

	void SetClientName(const std::string name);

	std::string GetClientName();

	void SetServerEndpoint(const EndPoint endPoint);

	void SetClientEndpoint(const EndPoint endPoint);

	EndPoint GetClientEndPoint();

	EndPoint GetServerEndPoint();

	bool SetOutputFile(const std::string& aFilePath);
	void SetOutputToFile(const bool bOutput);
	void SetOutputStream(std::ostream& aStream);
	void SetOutputToStream(const bool bOutput);


	void StartTest(const std::string& aTestName, const LogLevel aLogLevel);
	void StopTest();

	
	void ReportResults();

	private:
		void StartListenerThread();

		void ProcessReplies();

		std::string _myName;
		EndPoint::Port _myPort;

		int _numMsgsSent;
		BlockingQueue<Message> _msgsRcvd;

		TestLogger _lgr;

		EndPoint _clientEP;
		EndPoint _serverEP;

};

