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
void requestForString(Socket& socket_);
void replyForString(Socket& socket_);
void requestForFile(Socket& socket_);
void replyForFile(Socket& socket_);
void requestForPattern(Socket& socket_);
void requestForTextSearch(Socket& socket_);
void requestForFileDownload(Socket& socket_);
bool stringHandlingRequest(Socket& si);
bool stringHandlingReply(Socket& si);
bool fileHandlingRequest(Socket& si);
bool fileHandlingReply(Socket& si);
bool receivingBuffer(Socket& socket_, string& receivedContent);
void contentSaving(string text, string contentLength);
void interpretingReceivingMessage(vector<string> storage, string text);
void interpretingReceivingMessage1(vector<string> storage, string text);
bool handleDownloadRequest(Socket& socket_);
bool textSearchHandlingRequest(Socket& socket_);
bool patternHandlingRequest(Socket& si);
void interpretDownloadMessage(vector<string> storage);
void interpretFileSearchPattern(vector<string> storage);
void interpretTextSearch(vector<string> storage);
void findFiles(const string& path, const string& pattern);

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
	Receiver(BlockingQueue<Message>* rQueue_,string path_)
	{
		rQueue = rQueue_;
		path = path_;
	}
	void operator()(Socket& socket_);
	void requestForString(Socket& socket_);
	void replyForString(Socket& socket_);
	void requestForFile(Socket& socket_);
	void replyForFile(Socket& socket_);
	void requestForPattern(Socket& socket_);
	void requestForTextSearch(Socket& socket_);
	void requestForFileDownload(Socket& socket_);
	bool stringHandlingRequest(Socket& si);
	bool stringHandlingReply(Socket& si);
	bool fileHandlingRequest(Socket& si);
	bool fileHandlingReply(Socket& si);
	bool receivingBuffer(Socket& socket_, string& receivedContent);
	void contentSaving(string text, string contentLength);
	void interpretingReceivingMessage(vector<string> storage, string text);
	void interpretingReceivingMessage1(vector<string> storage, string text);
	bool handleDownloadRequest(Socket& socket_);
	bool textSearchHandlingRequest(Socket& socket_);
	bool patternHandlingRequest(Socket& si);
	void interpretDownloadMessage(vector<string> storage);
	void interpretFileSearchPattern(vector<string> storage);
	void interpretTextSearch(vector<string> storage);
	void findFiles(const string& path, const string& pattern);

private:
	BlockingQueue<Message>* rQueue;
	string originalContent;
	string reddfinedContent;
	string path;
	vector<string> allFiles;
};
#endif