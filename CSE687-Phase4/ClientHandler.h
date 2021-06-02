#pragma once



#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "Message.h"
#include "../Sockets/Sockets.h"

#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>

using namespace Sockets;

//----< callable object posts incoming message to rcvQ >-------------
/*
*  This is ClientHandler for receiving messages and posting
*  to the receive queue.
*/
class ClientHandler
{
public:
    //----< acquire reference to shared rcvQ >-------------------------

    ClientHandler(BlockingQueue<Message>* pQ, const std::string& name = "clientHandler") : pQ_(pQ), clientHandlerName(name)
    {
        StaticLogger<1>::write("\n  -- starting ClientHandler");
    }
    //----< shutdown message >-----------------------------------------

    ~ClientHandler()
    {
        StaticLogger<1>::write("\n  -- ClientHandler destroyed;");
    }
    //----< set BlockingQueue >----------------------------------------

    void setQueue(BlockingQueue<Message>* pQ)
    {
        pQ_ = pQ;
    }
    //----< frame message string by reading bytes from socket >--------

    std::string readMsg(Socket& socket)
    {
        std::string temp, msgString;
        while (socket.validState())
        {
            temp = socket.recvString('\n');  // read attribute
            msgString += temp;
            if (temp.length() < 2)           // if empty line we are done
                break;
        }
        return msgString;
    }
    //----< reads messages from socket and enQs in rcvQ >--------------

    void operator()(Socket socket)
    {
        pSocket = &socket;
        while (socket.validState())
        {
            std::string msgString = readMsg(socket);
            if (msgString.length() == 0)
            {
                // invalid message
                break;
            }
            Message msg = Message::FromString(msgString);
            StaticLogger<1>::write("\n  -- " + clientHandlerName + " RecvThread read message: " + msg.GetName());
            //std::cout << "\n  -- " + clientHandlerName + " RecvThread read message: " + msg.name();
          
            pQ_->enQ(msg);
            //std::cout << "\n  -- message enqueued in rcvQ";
            if (msg.GetCommand() == "quit")
                break;
        }
        StaticLogger<1>::write("\n  -- terminating ClientHandler thread");
    }
private:
    BlockingQueue<Message>* pQ_;
    std::string clientHandlerName;
    Socket* pSocket = nullptr;
};


