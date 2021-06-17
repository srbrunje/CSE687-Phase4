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
#include <iostream>

#include <windows.h>
#include <atlbase.h> //for wstrings
#include <atlconv.h> //for wstrings

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
	_testResult = Message();
	_testResult.SetName(_name);
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
	, _name{ aName }
	, _errorMessage{ aErrMsg }
{
	_testResult = Message();
	_testResult.SetName(_name);
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


std::string TestClass::LoadTest(const Message& Msg, TestClass::CallableObject* testMethod)
{

	// Convert the DLL name to a wstring and load the library
	std::string dll = Msg.GetDLL();
	CA2W ca2w(dll.c_str());
	LPCWSTR ddlname = ca2w;
	HINSTANCE hGetProcIDDLL = LoadLibrary(ddlname);
	if (!hGetProcIDDLL) {
		return "Unable to load DLL \"" + dll + "\"";
	}

	// Resolve function address
	typedef bool(__stdcall* func_t)(); //typedef for bool function(void) 
	std::string funcName = Msg.GetName();
	func_t funcAddr = (func_t)GetProcAddress(hGetProcIDDLL, funcName.c_str());
	if (!funcAddr) {
		return "Unable to resolve address of function \"" + funcName
			+ "\" in DLL \"" + dll + "\"";
	}

	*testMethod = funcAddr;
	return "";
}


const Message* TestClass::RunTest(const Message& Msg)
{
	std::string funcName = Msg.GetName();
	std::string DLLName = Msg.GetDLL();

	// Set common data members and prepare for test
	_testResult = Msg; // copy data from Msg (overwrite some later)
	_testResult.SwapAddressees(); // reply to original sender
	_testResult.SetName(funcName + " from " + DLLName);
	bool success = false;

	std::string errMsg = LoadTest(Msg, &_testMethod);
	if (!errMsg.empty()) {
		_testResult.SetError(errMsg);
		_testResult.SetStatus(ResultStatus::FAIL_LOAD);
		PostResult();
		return &_testResult;
	}

	// Begin timing test execution
	_testResult.SetStartTime(timing::now());

	// Run the callable test method
	try {
		std::future<bool> fp = std::async(_testMethod);
		while (fp.wait_for(std::chrono::milliseconds(10000)) == std::future_status::timeout)
		{}
		success = fp.get();
	}
	catch (std::exception& e) {
		// record exception data in the errMsg local variable		
		_testResult.SetStatus(ResultStatus::FAIL_ERR);
		_testResult.SetError(e.what());
	}

	// Finish timing of test execution before recording results to avoid added time
	timing::hack tf = timing::now(); // time finished
	_testResult.SetEndTime(tf);
	_testResult.SetTimestamp(timing::GetDateStr(tf));

	// Set result and error message data
	if (_testResult.GetStatus() != ResultStatus::FAIL_ERR) {
		_testResult.SetStatus(success ? ResultStatus::PASS : ResultStatus::FAIL);
	}

	if (_testResult.GetStatus() == ResultStatus::FAIL) {
		_testResult.SetError(_errorMessage); // use error message specified at construction
	}

	// Return a constant pointer to the result
	PostResult();
	return &_testResult;
}


Message TestClass::GetTestResult() const
{
	return _testResult;
}


void TestClass::PostResult() const
{
	Comm comm(_testResult.GetFrom(), "Post Result");
	comm.start();
	comm.postMessage(_testResult);
	comm.stop();
}