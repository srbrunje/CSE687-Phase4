#pragma once
/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: TestManager.h
*
* Description: Implements a class to handle the
*              manage a group of tests
*
***********************************************/

#include <vector>
#include <string>
#include <ostream>

#include "TestClass.h"
#include "TestLogger.h"


class TestManager
{
public:
	TestManager();

	void CreateTest(TestClass::CallableObject aTestMethod,
		const LogLevel aLogLevel,
		const std::string& aName,
		const std::string& aError);

	void CreateTest(TestClass::CallableObject aTestMethod,
		const LogLevel aLogLevel,
		const std::string& aName);


	bool SetOutputFile(const std::string& aFilePath);
	void SetOutputStream(std::ostream& aStream);
	void SetOutputToFile(const bool bOutputToFile);
	void SetOutputToStream(const bool bOutputToStream);


	int FindTestNumber(const std::string& aName);
	bool RunTest(int aTestNumber, EndPoint requestor);
	bool RunTest(Message message);
	bool ExecuteTests();
	void ReportResults();


private:
	std::vector<TestClass> _tests;	// series of tests to execute
	TestLogger _logger;				// the logger to handle logging test results
	int _numPass;					// number of tests that passed
	int _numFail;					// number of tests that failed
	int _numExc;					// number of tests that had exceptions
	double _timeElapsed;			// time it took to execute entire batch of tests in microseconds
};
