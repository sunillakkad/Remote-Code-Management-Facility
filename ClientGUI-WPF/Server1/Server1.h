#ifndef SERVER1_H
#define SERVER1_H
///////////////////////////////////////////////////////////////////////
// Server1.h - participate as server in message passing communication//
// ver 1.0                                                           //
// Language:    Visual C++, Visual Studio 2013				         //
// Platform:    Levovo Y40, Core i7, Windows 8                       //
// Application: Spring, 2015 : XML Document Model                    //
// Author:      Dhaval Dholakiya, Syracuse University                //
//              (315) 447-7644, ddholaki@syr.edu                     //
///////////////////////////////////////////////////////////////////////

/*
* Module Operations :
*== == == == == == == == ==
* when server is created it will listen request or reply on particular
port and ip. after that once it get message from  the other peer it will
interpret it via receiver and send appropiate reply to sending peer via sender

Public Interface:
=================
public:
Server1();
Server1(string ip, int port);
void extractMessage(BlockingQueue<Message> rQueue);
void Server1::getQueueMessage(BlockingQueue<Message>* rQueue, Sender* sd);

Required Files:
===============
Display.h, Server1.cpp, Sender.h, Receiver.h, sockets.h, AppHelpers.h, BlockingQueue.h, Message.h, FileSystem.h

Build Command:
==============
cl /EHa /TEST_SERVER Server.cpp

Maintenance History:
====================
ver 1.0 : 10 April 15
- first release
*/
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../Sender/Sender.h"

/////////////////////////////////////////////////////////////////////////////
// Server class
class Server1
{
public:
	Server1(){}
	Server1(string sourceIP, int sourcePort, string path);
	void extractMessage(BlockingQueue<Message>* receivingQueue, Sender* sender);
};
#endif