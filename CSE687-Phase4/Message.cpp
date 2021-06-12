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
void Message::SetCommand(const std::string& aCommand)
{
    _data["command"] = aCommand; // empty = no command
}
void Message::SetAuthor(const std::string& aAuthor)
{
    if (aAuthor.empty()) {
        _data["author"] = "No author specified";
    }
    else {
        _data["author"] = aAuthor;
    }
}
void Message::SetTo(EndPoint aTo)
{
    _data["to"]["address"] = aTo.address;
    _data["to"]["port"] = aTo.port;
}
void Message::SetFrom(EndPoint aFrom)
{
    _data["from"]["address"] = aFrom.address;
    _data["from"]["port"] = aFrom.port;
}
void Message::SetTimestamp(const std::string& aTimeDateStr)
{
    if (aTimeDateStr.empty()) {
        _data["timestamp"] = "No timestamp specified";
    }
    else {
        _data["timestamp"] = aTimeDateStr;
    }
}
void Message::SetName(const std::string& aName)
{
    if (aName.empty()) {
        _data["name"] = "No title specified";
    }
    else {
        _data["name"] = aName;
    }
}
void Message::SetDLL(const std::string& aDLLPath)
{
    _data["dll"] = aDLLPath;
}
void Message::SetStatus(const ResultStatus aStatus)
{
    _data["status"] = static_cast<enum_t>(aStatus);
}
void Message::SetError(const std::string& aErr)
{
    _data["error"] = aErr;
}
void Message::SetStartTime(const timing::hack aStartTime)
{
    _data["startTime"] = timing::toString(aStartTime);
}
void Message::SetEndTime(const timing::hack aEndTime)
{
    _data["endTime"] = timing::toString(aEndTime);
}
void Message::SetLogLevel(const LogLevel aLogLevel)
{
    _data["logLevel"] = static_cast<enum_t>(aLogLevel);
}





// Getters
std::string Message::GetCommand() const
{
    return Get("command");
}
std::string Message::GetAuthor() const
{
    return Get("author");
}
EndPoint Message::GetTo() const
{
    if (Contains("to")
        && _data["to"].contains("address")
        && _data["to"].contains("port"))
    {
        return EndPoint(_data["to"]["address"], _data["to"]["port"]);
    }
    return EndPoint();
}
EndPoint Message::GetFrom() const
{
    if (Contains("from")
        && _data["from"].contains("address")
        && _data["from"].contains("port"))
    {
        return EndPoint(_data["from"]["address"], _data["from"]["port"]);
    }
    return EndPoint();
}
std::string Message::GetTimestamp() const
{
    return Get("timestamp");
}
std::string Message::GetName() const
{
    return Get("name");
}
std::string Message::GetDLL() const
{
    return Get("dll");
}
ResultStatus Message::GetStatus() const
{
    return static_cast<ResultStatus>(Get<enum_t>("status"));
}
std::string Message::GetError() const
{
    return Get("error");
}
timing::hack Message::GetStartTime() const
{
    return timing::fromULLStr(Get("startTime"));
}
timing::hack Message::GetEndTime() const
{
    return timing::fromULLStr(Get("endTime"));
}
LogLevel Message::GetLogLevel() const
{
    return static_cast<LogLevel>(Get<enum_t>("logLevel"));
}
std::string Message::AsString(const int aFormat) const
{
    if (!_data.is_null()) {
        return _data.dump(aFormat);
    }
    return "";
}
std::string Message::Get(const std::string& aKey) const {
    if (!_data.is_null() && _data.contains(aKey) && _data[aKey].is_string()) {
        return _data[aKey];
    }
    return "";
}


// Support Functions
void Message::Clear()
{
    _data = {};
}

bool Message::Contains(const std::string& aKey) const
{
    if (_data.is_null()) {
        return false;
    }
    return _data.contains(aKey);
}

bool Message::Contains(const std::vector<std::string>& aKeys) const
{
    if (_data.is_null()) {
        return false;
    }
    for (const std::string& k : aKeys) {
        if (!_data.contains(k)) {
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
        if (!tmp.is_null()) {
            msg._data = tmp;
        }
    }
    catch (const std::exception& e) {
        // parser error occurred (string did not follow json format)
        // will return an empty message
    }
    return msg;
}