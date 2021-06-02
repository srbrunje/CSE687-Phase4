#include "Message.h"

Message::Message()
{
	Clear();
}

Message::Message(EndPoint aTo,
	EndPoint aFrom,
	const std::string& aAuthor,
	const std::string& aTimeStamp)
{
	Clear();
	SetTo(aTo);
	SetFrom(aFrom);
	SetAuthor(aAuthor);
	SetTimestamp(aTimeStamp);
}


// Setters
void Message::SetName(const std::string& aName)
{
	if (aName.empty()) {
		_body["name"] = "No name specified";
	}
	else {
		_body["name"] = aName;
	}
}
void Message::SetCommand(const std::string& aCommand)
{
	_body["command"] = aCommand; // empty = no command
}
void Message::SetTo(EndPoint aTo)
{
	_body["to"]["address"] = aTo.address;
	_body["to"]["port"] = aTo.port;
}
void Message::SetFrom(EndPoint aFrom)
{
	_body["from"]["address"] = aFrom.address;
	_body["from"]["port"] = aFrom.port;
}

void Message::SetAuthor(const std::string& aAuthor)
{
	if (aAuthor.empty()) {
		_body["author"] = "No author specified";
	}
	else {
		_body["author"] = aAuthor;
	}
}

void Message::SetTimestamp(const std::string& aTimeDateStr)
{
	if (aTimeDateStr.empty()) {
		_body["timestamp"] = "No timestamp specified";
	}
	else {
		_body["timestamp"] = aTimeDateStr;
	}
}


// Getters
std::string Message::GetName() const
{
	return _body["name"];
}
std::string Message::GetCommand() const
{
	return _body["command"];
}
EndPoint Message::GetTo() const
{
	return EndPoint(_body["to"]["address"],_body["to"]["port"]);
}
EndPoint Message::GetFrom() const
{
	return EndPoint(_body["from"]["address"], _body["from"]["port"]);
}
std::string Message::GetAuthor() const
{
	return _body["author"];
}
std::string Message::GetTimestamp() const
{
	return _body["timestamp"];
}
std::string Message::GetBodyStr(const int aFormat) const
{
	if (!_body.is_null()) {
		return _body.dump(aFormat);
	}
	return "";
}


// Support Functions

void Message::Clear()
{
	_body = {};
	SetName("");
	SetCommand("");
	SetTo(EndPoint());
	SetFrom(EndPoint());
	SetAuthor("");
	SetTimestamp("");
}

bool Message::Contains(const std::string& aKey) const
{
	return _body.contains(aKey);
}

bool Message::Contains(const std::vector<std::string>& aKeys) const
{
	for (const std::string& k : aKeys) {
		if (!Contains(k)) {
			return false;
		}
	}
	return true;
}

Message Message::FromString(const std::string& aMsgStr)
{
	Message msg;
	try {
		JSON tmp = JSON::parse(aMsgStr);
		if (!tmp.is_null() && tmp.contains("name") && tmp.contains("command")
			&& tmp.contains("to") && tmp.contains("from")
			&& tmp.contains("author") && tmp.contains("timestamp"))
		{
			msg._body = tmp;
		}
	}
	catch (const std::exception& e) {
		// parser error occurred (string did not follow json format)
		// will return an empty message
	}
	return msg;
}