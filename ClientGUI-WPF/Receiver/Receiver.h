#ifndef RECEIVER_H
#define RECEIVER_H
///////////////////////////////////////////////////////////////////////
// Receiver.h - Receive message from other peer						 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron N5010, Win7 Professional, SP1          //
// Application: Demonstration for CSE 687, Project #4, Spring 2015   //
// Author:      Sunilkumar Lakkad, Syracuse University               //
//              (315) 751-5834, lakkads1@gmail.com                   //
///////////////////////////////////////////////////////////////////////

/*
* Module Operations :
*== == == == == == == == ==
* Receiver receive message from the the other peer and then interpret it
based on the interpretation it will enqueue received message into receiving queue.

Public Interface:
=================
public:
void operator()(Socket& socket_);
void handingReplyFromServer(string command);
void requestForString(Socket& socket_);
void replyForString(Socket& socket_);
void requestForFile(Socket& socket_);
void replyForFile(Socket& socket_);
bool receivingBuffer(Socket& socket_, string& receivedContent);
void saveContents(string text, string contentLength);
bool stringHandlingRequest(Socket& socket_);
bool stringHandlingReply(Socket& socket_);
bool fileHandlingRequest(Socket& socket_);
bool fileHandlingReply(Socket& socket_);
void fileHandlingReply1(Socket& socket_);
void handleFileSearchResults(Socket& socket_);
void handleFileDownloadReply(Socket& socket_);
bool handleFileDownload(Socket& socket_);
bool handleSearchResults(Socket& socket_);
void interpretingReceivingMessageForString(vector<string> storage, string text);
void interpretingReceivingMessageForFile(vector<string> storage, string text);
void interpretingReceivingMessageForDownload(vector<string> storage, string text);
string getAckMessage();

Required Files:
===============
Display.h, Receiver.cpp, Sender.h, sockets.h, AppHelpers.h, BlockingQueue.h, Message.h, FileSystem.h

Build Command:
==============
cl /EHa /TEST_RECEIVER Receiver.cpp

Maintenance History:
====================
ver 1.0 : 27 April 15
- first release
*/
#include "../sockets/sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"

/////////////////////////////////////////////////////////////////////////////
// Receiver class
class Receiver
{
public:
	Receiver(){}
	Receiver(BlockingQueue<Message>* receivingQueue_, string path_)
	{
		receivingQueue = receivingQueue_;
		path = path_;
	}
	void operator()(Socket& socket_);
	void handingReplyFromServer(string command);
	void requestForString(Socket& socket_);
	void replyForString(Socket& socket_);
	void requestForFile(Socket& socket_);
	void replyForFile(Socket& socket_);
	bool receivingBuffer(Socket& socket_, string& receivedContent);
	void saveContents(string text, string contentLength);
	bool stringHandlingRequest(Socket& socket_);
	bool stringHandlingReply(Socket& socket_);
	bool fileHandlingRequest(Socket& socket_);
	bool fileHandlingReply(Socket& socket_);
	void fileHandlingReply1(Socket& socket_);
	void handleFileSearchResults(Socket& socket_);
	void handleFileDownloadReply(Socket& socket_);
	bool handleFileDownload(Socket& socket_);
	bool handleSearchResults(Socket& socket_);
	void interpretingReceivingMessageForString(vector<string> storage, string text);
	void interpretingReceivingMessageForFile(vector<string> storage, string text);
	void interpretingReceivingMessageForDownload(vector<string> storage, string text);
	string getAckMessage();
private:
	BlockingQueue<Message>* receivingQueue;
	string originalContent;
	string modifiedContent;
	string path;
	string ackMessage;
};
#endif