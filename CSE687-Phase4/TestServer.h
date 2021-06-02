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

class TestServer
{
public:
	TestServer();

	void InitilizeTestServer();
	void StartServer();
	void StopServer();
/*	bool SetOutputFile(const std::string& aFilePath);
	void SetOutputToFile(const bool bOutput);
	void SetOutputStream(std::ostream& aStream);
	void SetOutputToStream(const bool bOutput);

	void StartTest(const std::string& aName,
		const LogLevel aLogLevel);
	void StopTest();
	*/
	void ProcessReplies();
	//void ReportResults();

private:
	
	TestManager* _mgr;
	bool _shutdown;
	int _numMsgsSent;
	BlockingQueue<Message> _msgsRcvd;
};

