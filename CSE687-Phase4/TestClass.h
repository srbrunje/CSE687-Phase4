#ifndef __TEST_CLASS_H
#define __TEST_CLASS_H


/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: TestClass.h
*
* Description: Implements a class to handle the
*              creation and manipulation of a
*			   single test.
*
***********************************************/

#include <string>
#include <functional>

#include "utils.hpp"
#include "Message.h"
#include "Comm.h"

using namespace MsgPassingCommunication;


/* Class: TestClass
* Enables the ability to handle the creation and manipulation of a single test.
*/
class TestClass {
public:

	typedef std::function<bool()> CallableObject;

	/*************************************************************************
	*
	* Default constructor for the TestClass class
	*
	* Parameter: 	None
	*
	* return:	None
	*
	*************************************************************************/
	TestClass();

	// Custom Constructor
	TestClass(CallableObject aTestMethod,
		LogLevel aLogLevel,
		std::string aName = "",
		std::string aErrMsg = "Default Error Message: The test did not pass. Something wrong must have occurred");

	void SetName(std::string name);
	std::string GetName() const;

	void SetTestMethod(CallableObject method);
	CallableObject GetTestMethod() const;
	
	std::string LoadTest(const Message& Msg, TestClass::CallableObject* testMethod);
	const Message* RunTest(const Message& Msg);
	Message GetTestResult() const;

private:
	void PostResult() const;

	std::string _name;           // the name of the test
	std::string _errorMessage;   // an error message to display if the test fails without exceptions
	CallableObject _testMethod;  // a method to test
	Message _testResult;         // the result of the test after calling RunTest()
};

#endif // !__TEST_CLASS_H