#ifndef __UTILS_H
#define __UTILS_H


#include <chrono>       // for timing
#include <cstdint>      // for specific int types
#include <string>       // for std::string
#include <exception>    // for std::exception
#include <sstream>      // for std::stringstream in DblToStr()
#include <iomanip>      // for std::fixed and std::setpricision in DblToStr()



typedef unsigned long long ull;
typedef long long Port;
typedef uint8_t enum_t;



// Enum class to define the level of logging
enum class LogLevel
{
    STATUS_ONLY,
    WITH_ERR,
    WITH_DUR,
    ALL,
    INVALID
};

// Enum class to define the status of a test's result
enum class ResultStatus {
    PASS, 
    FAIL, 
    FAIL_ERR, 
    FAIL_LOAD,
    NOT_RUN
};

static std::string StatusToString(const ResultStatus status)
{
    switch (status)
    {
    case ResultStatus::PASS:
        return "PASS";
    case ResultStatus::FAIL:
        return "FAIL";
    case ResultStatus::FAIL_ERR:
        return "FAIL WITH EXCEPTION";
    case ResultStatus::FAIL_LOAD:
        return "FAILED TO LOAD DLL/FUNCTION";
    case ResultStatus::NOT_RUN:
        return "NOT RUN";
    default:
        return "INVALID";
    }
}


/* DblToStr - public
* Description: Converts a double into a string using a specified number of decimals along with
*              a comma separator if not deselected.
* Param:aVal              - the double value to convert to a string
* Param:aNumDecimalPlaces - the number of decimal places to include (default=2)
* Param:useCommaSeparator - use a comma separator (true, default) or don't (false)
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


/* IntToStr - public
* Description: Converts an integer into a string with a minimum number of places (prepends with 0's)
* Param:aInt       - the int value to convert to a string
* Param:aMinPlaces - the minimum number of places (default=2)
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
        try {
            ull st = std::stoull(ticks, 0, 0);
            return fromULL(st);
        }
        catch (const std::exception& e) {
            // stoull needs a number string, no characters/empty/etc.
        }
        return timing::hack();
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

} // end namespace timing

#endif // !__UTILS_H
