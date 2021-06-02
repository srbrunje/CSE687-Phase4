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

#include "utils.h"		// common utility functions and constants
#include "TestResult.h"	// TestResult class



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

	/*************************************************************************
	*
	* Setter for the test name
	*
	* Parameter: 	name: The name of the test
	*
	* return:	None
	*
	*************************************************************************/
	void SetName(std::string name);

	/*************************************************************************
	*
	* Getter for the test name
	*
	* Parameter: 	none
	*
	* return:	The name of the test
	*
	*************************************************************************/
	std::string GetName() const;

	/*************************************************************************
	*
	* Setter for the test method refrence
	*
	* Parameter: 	method: a refrence to the test that will be run
	*
	* return:	None
	*
	*************************************************************************/
	void SetTestMethod(CallableObject method);

	/*************************************************************************
	*
	* Getter for the test method refrence
	*
	* Parameter: 	None
	*
	* return:	a refrence to the test that will be run
	*
	*************************************************************************/
	CallableObject GetTestMethod() const;

	/*************************************************************************
	*
	* Setter for the log level
	*
	* Parameter: 	level: the level of logging for this test
	*
	* return:	None
	*
	*************************************************************************/
	void SetLogLevel(LogLevel level);


	/*************************************************************************
	*
	* Getter for the log level
	*
	* Parameter: 	None
	*
	* return:	the level of logging for this test
	*
	*************************************************************************/
	LogLevel GetLogLevel() const;


	/*************************************************************************
	*
	* Runs the _testMethod and returns a pointer to the _testResult generated
	*
	* Parameter: 	None
	*
	* return:	const pointer to _testResult
	*
	*************************************************************************/
	const TestResult* RunTest();


	/*************************************************************************
	*
	* Getter for the result of the test
	*
	* Parameter: 	None
	*
	* return:	pass or fail or not run
	*
	*************************************************************************/
	TestResult GetTestResult() const;

private:
	std::string _name;           // the name of the test
	std::string _errorMessage;   // an error message to display if the test fails without exceptions
	CallableObject _testMethod;  // a method to test
	LogLevel _logLevel;          // a verbosity at which to log data
	TestResult _testResult;      // the result of the test after calling RunTest()
};

#endif // !__TEST_CLASS_H