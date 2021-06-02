#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <string>
#include <cstdint>

#include "json.hpp"
using JSON = nlohmann::json;



struct EndPoint
{
    using Address = std::string;
    using Port = size_t;
    Address address;
    Port port;
    EndPoint(Address anAddress = "", Port aPort = 0);
    std::string toString();
    static EndPoint fromString(const std::string& str);
};

inline EndPoint::EndPoint(Address anAddress, Port aPort) : address(anAddress), port(aPort) {}

inline std::string EndPoint::toString()
{
    return address + ":" + std::to_string(port);
}

inline EndPoint EndPoint::fromString(const std::string& str)
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


class Message
{
public:
    Message();
    Message(EndPoint aTo, 
        EndPoint aFrom, 
        const std::string& aAuthor = "",
        const std::string& aTimeStamp = "");
    
    // Setters
    void SetName(const std::string& aName);
    void SetCommand(const std::string& aCommand);
    void SetTo(EndPoint aTo);
    void SetFrom(EndPoint aFrom);
    void SetAuthor(const std::string& aAuthor);
    void SetTimestamp(const std::string& aTimeDateStr);
    
    template<typename T>
    bool SetValue(const std::string& aKey, const T& aVal, const bool bAppend = false) {
        if (!bAppend) {                 // Overwriting
            _body[aKey] = aVal;
        }
        else if (_body.contains(aKey)) {    // Appending to an array (if possible
            if (!_body[aKey].is_array()) {
                return false; // must be an array type
            }
            _body[aKey].push_back(aVal);
        }
        else {                          // Appending to an uninitialized key: begin an array
            _body[aKey].push_back(aVal);
        }
        return true;
    }

    // Getters
    std::string GetName() const;
    std::string GetCommand() const;
    EndPoint GetTo() const;
    EndPoint GetFrom() const;
    std::string GetAuthor() const;
    std::string GetTimestamp() const;
    std::string GetBodyStr(const int aFormat = 4) const;

    template<typename T>
    T GetValue(const std::string& aKey) const {
        if (_body.contains(aKey)) {
            JSON tmp = T();
            if (_body[aKey].type() == tmp.type()) { // ensure same type
                return _body[aKey];
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
    JSON        _body;
};





#endif // !__MESSAGE_H