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
#include "TestClass.h"

class TestServer
{
public:
	TestServer();

	void StartServer();
	void StopServer(const bool bHard = false);

	const std::string ADDRESS = "localhost";
	const Port PORT = 9890;

private:
	std::vector<TestClass> _tests;
	int _numMsgsSent;
	BlockingQueue<Message> _msgsRcvd;
	std::atomic<bool> _shutdown;
	std::mutex _mtx;
};

