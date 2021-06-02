#ifndef __UTILS_H
#define __UTILS_H


/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: utils.h
*
* Data Defined: namespace timing
*               enum class LogLevel
*
* Description: Provides a space for commonly
*              used data and functions, while
*              protecting them with a namespace.
*
***********************************************/


#include <chrono>       // for timing
#include <string>       // for std::string
#include <exception>    // for std::exception
#include <sstream>      // for std::stringstream in DblToStr()
#include <iomanip>      // for std::fixed and std::setpricision in DblToStr()




typedef unsigned long long ull;


// Enum class to define the level of logging
enum class LogLevel
{
	Pass_Fail,
	Pass_Fail_with_error_message,
	Pass_Fail_with_test_duration,
	Pass_Fail_with_error_message_and_test_duration,
	INVALID
};


/** DblToStr - public
* Description: Converts a double into a string using a specified number of decimals along with
*              a comma separator if not deselected.
* Parameter 0: the double value to convert to a string
* Parameter 1: the number of decimal places to include (default=2)
* Parameter 2: use a comma separator (true, default) or don't (false)
* Return: the double value in std::string form
*/
static std::string DblToStr(const double aVal,
	const unsigned int aNumDecimalPlaces = 2,
	const bool useCommaSeparator = true)
{
	// Use a stringstream with iomanip to perform decimal place specification
	// and then place the given value into that stringstream
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(aNumDecimalPlaces) << aVal;

	// Conver the stringstream into a string
	std::string str = ss.str();

	// If using a comma separator, insert commas where appropriate
	if (useCommaSeparator) {
		// Start by finding the start of the number in the 1's column
		size_t pos = str.find('.'); // if the string contains a decimal point, start there
		if (pos == std::string::npos) {
			// If it doesn't, simply start at the end of the string and move backwards
			pos = str.length();
		}
		// Add a comma ever 3 positions left of the decimal place until the front is reached
		while (pos > 3 && pos != std::string::npos) {
			pos -= 3;
			str.insert(pos, 1, ',');
		}
	}
	return str;
}


/** IntToStr - public
* Description: Converts an integer into a string with a minimum number of places (prepends with 0's)
* Parameter 0: the int value to convert to a string
* Parameter 1: the minimum number of places (default=2)
* Return: the integer value in std::string form with zeros prepending the value if needed to fill the space
*/
static std::string IntToStr(int aInt, const int aMinPlaces = 2)
{
	std::string minus = "";
	if (aInt < 0) {
		minus = "-";
		aInt *= -1;
	}
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(aMinPlaces) << aInt;
	return minus + ss.str();
}


// Set of shorthands for timing purposes
namespace timing {

	typedef std::chrono::system_clock clock;
	typedef clock::time_point         hack;

	static hack now() {
		return clock::now();
	}

	static double duration_us(hack aStartTime, hack aEndTime = now()) {
		return std::chrono::duration<double, std::micro>(aEndTime - aStartTime).count();
	}

	static std::string toString(hack aHack)
	{
		clock::duration t = aHack.time_since_epoch();
		ull dur = t.count();
		return std::to_string(dur);
	}

	static hack fromULL(ull ticks)
	{
		clock::duration tmp(ticks);
		hack tp(tmp);
		return tp;
	}
	static hack fromULLStr(const std::string& ticks)
	{
		ull st = std::stoull(ticks, 0, 0);
		return fromULL(st);
	}


	static std::string GetDateStr(hack aTime = now())
	{
		std::time_t ttNow = std::chrono::system_clock::to_time_t(aTime);
		std::tm     tmNow;
		localtime_s(&tmNow, &ttNow);
		std::string str = IntToStr(tmNow.tm_year + 1900) + "-"
			+ IntToStr(tmNow.tm_mon + 1) + "-"
			+ IntToStr(tmNow.tm_mday) + "|"
			+ IntToStr(tmNow.tm_hour) + ":"
			+ IntToStr(tmNow.tm_min) + ":"
			+ IntToStr(tmNow.tm_sec);
		return str;
	}


	static std::string FormatTimeString(double dDurration)
	{
		std::string rtn = "";

		if (dDurration > 1000000) //scale to seconds
		{
			rtn = DblToStr(dDurration / 1000000) + " seconds";
		}
		else if (dDurration > 1000) //scale to milliseconds
		{
			rtn = DblToStr(dDurration / 1000) + " milliseconds";
		}
		else if (dDurration < 1) //scale to nanoseconds
		{
			rtn = DblToStr(dDurration * 1000) + " nanoseconds";
		}
		else //microseconds
		{
			rtn = DblToStr(dDurration) + " microseconds";
		}
		return rtn;
	}
}



#endif // !__UTILS_H
