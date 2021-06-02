/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: TestClass.cpp
*
* Description: Implements a class to handle the
*              creation and manipulation of a
*			   single test.
*
***********************************************/

#include "TestClass.h"
#include <future>
#include <thread>


/*************************************************************************
*
* Constructor for the TestClass class
*
* Parameter: 	None
*
* return:	None
*
*************************************************************************/
TestClass::TestClass()
{
	_name = "";
	_logLevel = LogLevel::Pass_Fail;
	_testResult = TestResult(_name);
}

/** Custom Constructor - public
* Description: Constructs a TestClass object with given specific parameters
* Parameter 0: a method to be tested
* Parameter 1: a log level for output
* Parameter 2: a name for the test (defaults to an empty string)
* Parameter 3: a custom error message if the test fails (contains default)
* Return: nothing
*/
TestClass::TestClass(CallableObject aTestMethod,
	LogLevel aLogLevel,
	std::string aName,
	std::string aErrMsg)
	: _testMethod{ aTestMethod }
	, _logLevel{ aLogLevel }
	, _name{ aName }
	, _errorMessage{ aErrMsg }
{
	_testResult = TestResult(_name);
}

/*************************************************************************
*
* Setter for the test name
*
* Parameter: 	aName: The name of the test
*
* return:	None
*
*************************************************************************/
void TestClass::SetName(std::string aName)
{
	_name = aName;
}

/*************************************************************************
*
* Getter for the test name
*
* Parameter: 	none
*
* return:	The name of the test
*
*************************************************************************/
std::string TestClass::GetName() const
{
	return _name;
}

/*************************************************************************
*
* Setter for the test method refrence
*
* Parameter: 	aMethod: a refrence to the test that will be run
*
* return:	None
*
*************************************************************************/
void TestClass::SetTestMethod(TestClass::CallableObject aMethod)
{
	_testMethod = aMethod;
}

/*************************************************************************
*
* Getter for the test method refrence
*
* Parameter: 	None
*
* return:	a refrence to the test that will be run
*
*************************************************************************/
TestClass::CallableObject TestClass::GetTestMethod() const
{
	return _testMethod;
}

/*************************************************************************
*
* Setter for the log level
*
* Parameter: 	aLevel: the level of logging for this test
*
* return:	None
*
*************************************************************************/
void TestClass::SetLogLevel(LogLevel aLevel)
{
	_logLevel = aLevel;
}

/*************************************************************************
*
* Getter for the log level
*
* Parameter: 	None
*
* return:	the level of logging for this test
*
*************************************************************************/
LogLevel TestClass::GetLogLevel() const
{
	return _logLevel;
}

/** RunTest - public
* Description: Runs the _testMethod, collects the results, and then returns
*    them in a const pointer to the _testResult private member variable.
* Parameters: none
* Return: const pointer to _testResult PMV
*/
const TestResult* TestClass::RunTest()
{
	// Set common data members and prepare for test
	_testResult.SetName(_name);
	_testResult.SetLogLevel(_logLevel);
	bool success = false;
	std::string errMsg = "";

	// Begin timing test execution
	_testResult.SetStartTime(timing::now());

	//run as a thread
	std::future<bool> fp = std::async(_testMethod);

	// Run the callable test method
	try {

		

		//set the max wait time -- this should be passed a a parameter for each test at creation
		std::chrono::milliseconds span(1000);

		while (fp.wait_for(span) == std::future_status::timeout)
		{

		}

		//check to see if the thread terminated

		success = fp.get();

		//success = _testMethod();	// assuming all callables only return booleans
	}
	catch (std::exception& e) {
		// record exception data in the errMsg local variable		
		errMsg = e.what();
		_testResult.SetStatus(TestResult::Status::FAIL_EXC);
		_testResult.SetErrorMessage(e.what());
	}

	// Finish timing of test execution before recording results to avoid added time
	_testResult.SetEndTime(timing::now());

	// Set result and error message data
	if (_testResult.GetStatus() != TestResult::Status::FAIL_EXC) {
		_testResult.SetStatus(success ? TestResult::Status::PASS : TestResult::Status::FAIL);
	}

	if (_testResult.GetStatus() == TestResult::Status::FAIL) {
		_testResult.SetErrorMessage(_errorMessage); // use error message specified at construction
	}

	// Return a constant pointer to the result
	return &_testResult;
}


/*************************************************************************
*
* Getter for the result of the test
*
* Parameter: 	None
*
* return:	results of the test (pass/fail, error messages, etc.)
*
*************************************************************************/
TestResult TestClass::GetTestResult() const
{
	return _testResult;
}