#ifndef __LOGGER_H
#define __LOGGER_H

/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* File: Logger.h
*
* Description: Defines a class that is used for
*              logging output to a file and/or
*              a stream.
*
***********************************************/


#include <string>
#include <ostream>
#include <filesystem> // requires C++17 or later

#include "TestClass.h"
#include "Message.h"

class TestLogger {
private:
	// Private Member Variables (PMV)
	bool                  _outputToFile;     // logs to file if true, does not if false
	bool                  _outputToStream;   // logs to stream if true, does not if false
	std::filesystem::path _outputFile;       // file path to where data is logged
	std::ostream*         _outputStream;	 // ostream to pipe data to

public:
	// Constructors
	TestLogger(const std::string& aPathToOutputFile = "", const bool bOutputToStream = false);

	// Setters
	void SetOutputToFile(const bool bOutputToFile);
	void SetOutputToStream(const bool bOutputToStream);
	bool SetOutputFile(const std::string& aPathToFile);
	void SetOutputStream(std::ostream& aStream);

	// Getters
	bool IsOutputtingToFile() const;
	bool IsOutputtingToStream() const;
	std::string GetOutputFile() const;
	std::ostream* GetOutputStream() const;

	// Support Functions
	bool LogResult(const Message& aMsg);
	bool LogMessage(const std::string& aMessage);
	void ClearContents();
};

#endif // !__LOGGER_H
