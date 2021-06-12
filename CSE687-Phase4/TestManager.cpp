/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: TestManager.cpp
*
* Description: Implements a class to handle the
*              manage a group of tests
*
***********************************************/

#include "TestManager.h"
#include "TestClass.h"
#include "utils.hpp"
#include "Comm.h"
#include <iostream>
#include <thread>
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>



using namespace MsgPassingCommunication;


TestManager::TestManager()
{
	// Set _tests to an empty vector and _logger to a default Logger
	_tests = std::vector<TestClass>();
	_logger = TestLogger();

	// Set all test tracking values to zero
	_numPass = 0;
	_numFail = 0;
	_numExc = 0;
	_timeElapsed = 0.0;
}


void TestManager::CreateTest(TestClass::CallableObject aTestMethod,
	const LogLevel aLogLevel,
	const std::string& aName,
	const std::string& aError)
{
	_tests.push_back(TestClass(aTestMethod, aLogLevel, aName, aError));
}

void TestManager::CreateTest(TestClass::CallableObject aTestMethod,
	const LogLevel aLogLevel,
	const std::string& aName)
{
	_tests.push_back(TestClass(aTestMethod, aLogLevel, aName));
}


void TestMSG(TestResult aResult, EndPoint requestor)
{
	EndPoint senderEP("localhost", 9890);
	Comm comm(senderEP, "TestMSG");
	comm.start();

	// Create and populate the message
	Message msg(requestor, senderEP);
	msg.SetAuthor("Some author");
	msg.SetTimestamp(timing::GetDateStr()); // uses current time as timestamp
	msg.SetName(aResult.GetName());
	msg.Set("status", static_cast<enum_t>(aResult.GetStatus()));
	msg.Set("errMsg", aResult.GetErrorMessage());
	msg.Set("startTime", timing::toString(aResult.GetStartTime()));
	msg.Set("endTime", timing::toString(aResult.GetEndTime()));
	msg.Set("logLevel", static_cast<enum_t>(aResult.GetLogLevel()));

	// send the message
	comm.postMessage(msg);
	comm.stop();
}

/*************************************************************************
*
* Runs all of the tests
*
* Parameter: 	None
*
* return:	true if all tests pass, else false
*
*************************************************************************/
bool TestManager::ExecuteTests()
{
	// This will change to send a message to the server to run a particular test.
	// The message will also set the log parameters
	

	// If there are no tests, return false (the user probally meant to do something)
	if (_tests.size() <= 0) {
		std::cerr << "Error: No tests have been queued to run!\n";
		return false;
	}

	// Reset the test results from any previous runs
	_numPass = 0;
	_numFail = 0;
	_numExc = 0;

	// Start timer for overall time of test execution
	timing::hack testStart = timing::now();

	//loop through and execute each test
	for (int i = 0; i < _tests.size(); i++)
	{
		const TestResult* result = _tests[i].RunTest();

		// create the message reply and send
		TestResult r = *result;

		//std::thread r1(TestMSG);
		//r1.join();
//		TestMSG(r);

		if (result->GetStatus() == ResultStatus::PASS) {
			_numPass++;
		}
		else if (result->GetStatus() == ResultStatus::FAIL_ERR) {
			_numExc++;
			_numFail++;
		}
		else {
			_numFail++;
		}
	}

	// Record the time it took to run all tests
	_timeElapsed = timing::duration_us(testStart);

	// Only return true if every test passed, false otherwise
	return _numFail == 0;
}


/*************************************************************************
*
* Sets the name of the file to write test results to
*
* Parameter: 	aFilePath: the path to the file to log data to
*
* return:	bool: successfully set (true) or not (false)
*
*************************************************************************/
bool TestManager::SetOutputFile(const std::string& aFilePath)
{
	// If the filepath is valid, this will also set TestLogger::_outputToFile to true
	return _logger.SetOutputFile(aFilePath);
}


/*************************************************************************
*
* Sets the output destination for the console messages
*
* Parameter: 	aStream: the output stream
*
* return:	None
*
*************************************************************************/
void TestManager::SetOutputStream(std::ostream& aStream)
{
	_logger.SetOutputStream(aStream);
}

/*************************************************************************
*
* Sets the TestLogger to output to a file or not based on the input
*
* Parameter: 	bool: output to file if true, do not if false
*
* return:	None
*
*************************************************************************/
void TestManager::SetOutputToFile(const bool bOutputToFile)
{
	_logger.SetOutputToFile(bOutputToFile);
}

/*************************************************************************
*
* Sets the TestLogger to output to a stream or not based on the input
*
* Parameter: 	bool: output to stream if true, do not if false
*
* return:	None
*
*************************************************************************/
void TestManager::SetOutputToStream(const bool bOutputToStream)
{
	_logger.SetOutputToStream(bOutputToStream);
}

/*************************************************************************
*
* Passes all TestResult objects contained as members in the _tests vector
*   to the TestLogger for data output and logging
*
* Parameter: 	None
*
* return:	None
*
*************************************************************************/
void TestManager::ReportResults()
{
	// Clear old test output in preparation for new run
	_logger.ClearContents();

	// Log the overview of the results
	_logger.LogMessage(
		"Completed " + std::to_string(_tests.size()) + " tests with "
		+ std::to_string(_numPass) + " passes and " + std::to_string(_numFail)
		+ " failures with " + std::to_string(_numExc)
		+ " exceptions\nTotal time elapsed: " + timing::FormatTimeString(_timeElapsed) + "\n\n"
	);

	// Log the results per individual test
	// TODO: potentially add a LogResults(std::vector<TestResult>) function to the
	//   TestLogger class to reduce the number of times a file is opened and closed, etc.
	for (int i = 0; i < _tests.size(); i++) {
		// Describe the test number / total batch progress
		_logger.LogMessage("Test " + std::to_string(i + 1) + " of " + std::to_string(_tests.size()) + ":\n");

		// Log the result of the test currently being evaluated
		_logger.LogResult(_tests[i].GetTestResult());
	}
}

bool TestManager::RunTest(int aTestNumber, EndPoint requestor)
{
	if (aTestNumber < 0 || aTestNumber >= _tests.size()) {
		return false; // invalid test number for index
	}
	const TestResult* result = _tests[aTestNumber].RunTest();
	TestMSG(*result,requestor); // create the message reply and send
	return true;
}

bool TestManager::RunTest(Message message)
{
	TestClass test = TestClass();
	std::string DLLName = message.GetDLL();
	std::string FuncName = message.GetName();
	EndPoint requestor = message.GetFrom();
	test.SetLogLevel((LogLevel)message.Get<enum_t>("logLevel"));

	if (DLLName.empty()) {
		// No DLL, try and run the built-in function
		return RunTest(FindTestNumber(FuncName), requestor);
	}

	const TestResult* result = test.RunDLL(DLLName, FuncName);
	TestMSG(*result,requestor); // create the message reply and send
	return true;
}

int TestManager::FindTestNumber(const std::string& aName) 
{
	for (int i = 0; i < _tests.size(); i++) {
		if (_tests[i].GetName().compare(aName) == 0) {
			return i;
		}
	}
	return -1;
}