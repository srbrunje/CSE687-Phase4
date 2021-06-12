#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <string>
#include <cstdint>
#include "json.hpp"
#include "utils.hpp"

using JSON = nlohmann::json;

class EndPoint
{
public:
    using Address = std::string;
    using Port = size_t;

    EndPoint(const Address& aAddress = "", const Port& aPort = 0)
        : address(aAddress), port(aPort) {}


    std::string toString()
    {
        return address + ":" + std::to_string(port);
    }
    static EndPoint fromString(const std::string& str)
    {
        EndPoint ep;
        size_t pos = str.find_first_of(':');
        if (pos == str.length())
            return ep;
        ep.address = str.substr(0, pos);
        std::string portStr = str.substr(pos + 1);
        ep.port = std::stoi(portStr);
        return ep;
    }


    bool operator==(const EndPoint& rhs) const {
        return (this->port == rhs.port
            && this->address.compare(rhs.address) == 0);
    }
    bool operator!=(const EndPoint& rhs) const {
        return !(*this == rhs);
    }

    Address address;
    Port port;
};






class Message
{
public:
    Message();
    Message(EndPoint aTo,
        EndPoint aFrom,
        const std::string& aAuthor = "",
        const std::string& aTimeStamp = "");

    // Setters
    void SetCommand(const std::string& aCommand);
    void SetAuthor(const std::string& aAuthor);
    void SetTo(EndPoint aTo);
    void SetFrom(EndPoint aFrom);
    void SetTimestamp(const std::string& aTimeDateStr);
    void SetName(const std::string& aName);
    void SetDLL(const std::string& aDLLPath);
    void SetStatus(const ResultStatus aStatus);
    void SetError(const std::string& aErrMsg);
    void SetStartTime(const timing::hack aStartTime);
    void SetEndTime(const timing::hack aEndTime);
    void SetLogLevel(const LogLevel aLogLevel);

    template<typename T>
    bool Set(const std::string& aKey, const T& aVal, const bool bAppend = false) {
        if (!bAppend) {                 // Overwriting
            _data[aKey] = aVal;
        }
        else if (_data.contains(aKey)) {    // Appending to an array (if possible
            if (!_data[aKey].is_array()) {
                return false; // must be an array type
            }
            _data[aKey].push_back(aVal);
        }
        else {                          // Appending to an uninitialized key: begin an array
            _data[aKey].push_back(aVal);
        }
        return true;
    }


    // Getters
    std::string GetCommand() const;
    std::string GetAuthor() const;
    EndPoint GetTo() const;
    EndPoint GetFrom() const;
    std::string GetTimestamp() const;
    std::string GetName() const;
    std::string GetDLL() const;
    ResultStatus GetStatus() const;
    std::string GetError() const;
    timing::hack GetStartTime() const;
    timing::hack GetEndTime() const;
    LogLevel GetLogLevel() const;
    std::string AsString(const int aFormat = 4) const;
    std::string Get(const std::string& aKey) const;

    template<typename T>
    T Get(const std::string& aKey) const {
        if (!_data.is_null() && _data.contains(aKey)) {
            JSON tmp = T();
            if (_data[aKey].type() == tmp.type()) { // ensure same type
                return _data[aKey];
            }
        }
        return T();
    }

    // Support Functions
    void Clear();
    bool Contains(const std::string& aKey) const;
    bool Contains(const std::vector<std::string>& aKeys) const;
    static Message FromString(const std::string& aMsgStr);

private:
    JSON _data;
};

#endif // !__MESSAGE_H