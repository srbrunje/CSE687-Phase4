#pragma once

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
*              result of a test.
*
***********************************************/

#include <string>
#include "utils.hpp"		// common utility functions and constants


/* Class: TestResult
* Contains basic data describing the result of the test to include pass/fail status,
*	any error messages, test duration, and the desired log level
*/
class TestResult {
private:

	// Private Member Variables (PMV)
	std::string  _name;			// name of the test that resulted in this result
	ResultStatus _status;		// pass, fail, or not run status denotation
	std::string  _errorMessage;	// any/all error messages that occurred during test
	timing::hack _startTime;	// the time the test began to execute
	timing::hack _endTime;		// the time the test finished execution
	LogLevel     _logLevel;		// the level of logging desired

public:
	// Constructors
	TestResult(const std::string& aName = "");	// default constructor with option of setting name


	// Setters
	void SetName(const std::string& aName);
	void SetStatus(const ResultStatus aStatus);
	void SetErrorMessage(const std::string& aErrMsg);
	void SetStartTime(const timing::hack aStartTime);
	void SetEndTime(const timing::hack aEndTime);
	void SetLogLevel(const LogLevel aLogLevel);

	// Getters
	std::string  GetName() const;
	ResultStatus GetStatus() const;
	std::string  GetErrorMessage() const;
	timing::hack GetStartTime() const;
	timing::hack GetEndTime() const;
	double       GetDuration() const;	// returns the time taken to run the test in microseconds
	LogLevel     GetLogLevel() const;

	// Support Functions
	static std::string StatusToString(const ResultStatus aStatus);
};