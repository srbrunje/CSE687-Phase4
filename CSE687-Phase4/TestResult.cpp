/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: TestResult.cpp
*
* Description: Implements a class to handle the
*              result of a test.
*
***********************************************/

#include "TestResult.h"


/** Default Constructor - public
* Paramter 0: the test's name (defaults to an empty string)
* Return: nothing
*/
TestResult::TestResult(const std::string& aName)
{
	SetName(aName);
	SetStatus(ResultStatus::NOT_RUN);
	SetErrorMessage("");
	SetStartTime(timing::now());
	SetEndTime(timing::now());
	SetLogLevel(LogLevel::STATUS_ONLY);
}

/** SetName - public
 * Description: Sets the _name PMV to the given string
 * Parameter 0: a string containing a name for the test
 * Return: nothing
*/
void TestResult::SetName(const std::string& aName)
{
	_name = aName;
}

/** SetStatus - public
 * Description: Sets the _status PMV to the given Status enumeration
 * Parameter 0: a Status enumeration for the test
 * Return: nothing
*/
void TestResult::SetStatus(const ResultStatus aStatus)
{
	_status = aStatus;
	if (_status == ResultStatus::PASS) {
		_errorMessage = ""; // erase any default error messages if it passed
	}
}

/** SetErrorMessage - public
 * Description: Sets the _errorMessage PMV to the given string
 * Parameter 0: a string containing any/all error messages for the test
 * Return: nothing
*/
void TestResult::SetErrorMessage(const std::string& aErrMsg)
{
	_errorMessage = aErrMsg;
}

/** SetStartTime - public
 * Description: Sets the _startTime PMV to the given timing::hack
 * Parameter 0: a timing::hack (std::chrono::high_resolution::clock::time_point)
 *    denoting when the test began executing
 * Return: nothing
*/
void TestResult::SetStartTime(const timing::hack aStartTime)
{
	_startTime = aStartTime;
}

/** SetEndTime - public
 * Description: Sets the _endTime PMV to the given timing::hack
 * Parameter 0: a timing::hack (std::chrono::high_resolution::clock::time_point)
 *    denoting when the test finished executing
 * Return: nothing
*/
void TestResult::SetEndTime(const timing::hack aEndTime)
{
	_endTime = aEndTime;
}

/** SetLogLevel - public
 * Description: Sets the _logLevel PMV to the given LogLevel enumeration
 * Parameter 0: a LogLevel enumeration for the test
 * Return: nothing
*/
void TestResult::SetLogLevel(const LogLevel aLogLevel)
{
	_logLevel = aLogLevel;
}



/** GetName - public
 * Description: Returns the _name string PMV
 * Parameters: none
 * Return: string (_name PMV)
*/
std::string TestResult::GetName() const
{
	return _name;
}

/** GetStatus - public
 * Description: Returns the _status Status enum PMV
 * Parameters: none
 * Return: Status enumeration (_status PMV)
*/
ResultStatus TestResult::GetStatus() const
{
	return _status;
}

/** GetErrorMessage - public
 * Description: Returns the _errorMessage string PMV
 * Parameters: none
 * Return: string (_errorMessage PMV)
*/
std::string TestResult::GetErrorMessage() const
{
	return _errorMessage;
}

/** GetStartTime - public
 * Description: Returns the _startTime timing::hack PMV
 * Parameters: none
 * Return: timing::hack (aka std::chrono::system_clock::time_point)
*/
timing::hack TestResult::GetStartTime() const
{
	return _startTime;
}

/** GetEndTime - public
 * Description: Returns the _endTime timing::hack PMV
 * Parameters: none
 * Return: timing::hack (aka std::chrono::system_clock::time_point)
*/
timing::hack TestResult::GetEndTime() const
{
	return _endTime;
}

/** GetDuration - public
 * Description: Returns the time it took to completely execute the test
 *    in milliseconds, or -1.0 if the test has not completed yet.
 * Parameters: none
 * Return: double (microseconds of run time)
*/
double TestResult::GetDuration() const
{
	if (_status == ResultStatus::NOT_RUN) {	// has not run yet
		return -1.0;	// return with some notably erroneous value
	}
	return timing::duration_us(_startTime, _endTime);
}

/** GetLogLevel - public
 * Description: Returns the _logLevel LogLevel enum PMV
 * Parameters: none
 * Return: LogLevel enumeration (_logLevel PMV)
*/
LogLevel TestResult::GetLogLevel() const
{
	return _logLevel;
}




/** StatusToString - public
 * Description: Takes a Status as input and returns a string describing it
 * Parameters: a Status to convert
 * Return: std::string describing the given status
*/
std::string TestResult::StatusToString(const ResultStatus aStatus)
{
	switch (aStatus)
	{
	case ResultStatus::PASS:
		return "PASS";
	case ResultStatus::FAIL:
		return "FAIL";
	case ResultStatus::FAIL_ERR:
		return "FAIL WITH EXCEPTION";
	case ResultStatus::NOT_RUN: // fall-through
	default:
		return "NOT RUN";
	}
}