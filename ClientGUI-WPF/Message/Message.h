#ifndef MESSAGE_H
#define MESSAGE_H
///////////////////////////////////////////////////////////////////////
// Message.h - Define Message class									 //
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
* when server is created it will listen request or reply on particular
port and ip. after that once it get message from  the other peer it will
interpret it via receiver and send appropiate reply to sending peer via sender

Public Interface:
=================
public:
Message(){}
Message(string command_) :command(command_){}
string getCommand(){ return command; }
map<string, string>& getAttributes(){ return attributes; }
void buildMessage(Message& msg, int sourcePort, int destPort, string text, int contentLength);
void buildMessage(Message& msg, int sourcePort, int destPort, string fileName);
void buildMessage(Message& msg, int sourcePort, int destPort, string path, string pattern);
void buildMessage(Message& msg, int sourcePort, int destPort, string path, string pattern, string text);
void buildDownloadMessage(Message& msg, int sourcePort, int destPort, string path, string filename);
void setBody(string body) { body_ = body; }
string getBody(){ return body_; }

Required Files:
===============
Message.cpp

Build Command:
==============
cl /EHa /TEST_Message Message.cpp

Maintenance History:
====================
ver 1.0 : 27 April 15
- first release
*/
#include <string>
#include <iostream>
#include<map>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Message class

class Message
{
public:
	Message(){}
	Message(string command_) :command(command_){}
	string getCommand(){ return command; }
	map<string, string>& getAttributes(){ return attributes; }
	void buildMessage(Message& msg, int sourcePort, int destPort, string text, int contentLength);
	void buildMessage(Message& msg, int sourcePort, int destPort, string fileName);
	void buildMessage(Message& msg, int sourcePort, int destPort, string path, string pattern);
	void buildMessage(Message& msg, int sourcePort, int destPort, string path, string pattern, string text);
	void buildDownloadMessage(Message& msg, int sourcePort, int destPort, string path, string filename);
	void setBody(string body) { body_ = body; }
	string getBody(){ return body_; }
private:
	string command;
	map<string, string> attributes;
	string body_;
};
#endif