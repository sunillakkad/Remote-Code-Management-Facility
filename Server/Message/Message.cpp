///////////////////////////////////////////////////////////////////////
// Message.cpp - Define Message class	    						 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron N5010, Win7 Professional, SP1          //
// Application: Demonstration for CSE 687, Project #4, Spring 2015   //
// Author:      Sunilkumar Lakkad, Syracuse University               //
//              (315) 751-5834, lakkads1@gmail.com                   //
///////////////////////////////////////////////////////////////////////

#include "Message.h"

//----< buil Message >--------------------------------------------------
void Message::buildMessage(Message& msg, int sourcePort, int destPort, string text, int contentLength)
{
	map<string, string>& attributes = msg.getAttributes();
	attributes["sourceIP"] = "localhost";
	attributes["sourcePort"] = to_string(sourcePort);
	attributes["destIP"] = "localhost";
	attributes["destPort"] = to_string(destPort);
	attributes["contentLength"] = to_string(contentLength);
	attributes["body"] = text;
}


void Message::buildMessage(Message& msg, int sourcePort, int destPort, string fileName)
{
	map<string, string>& attributes = msg.getAttributes();
	attributes["sourceIP"] = "localhost";
	attributes["sourcePort"] = to_string(sourcePort);
	attributes["destIP"] = "localhost";
	attributes["destPort"] = to_string(destPort);
	attributes["contentLength"] = "1024";
	attributes["fileName"] = fileName;
}

//----< demonstration >--------------------------------------------------
#ifdef TEST_MESSAGE
int main()
{
	string test = "this is test string";
	Message msg("STRING_HANDLING_REQUEST");
	msg.buildMessage(msg, 1000, 2000, test, test.size());
	map<string, string> att = msg.getAttributes();

	cout<<" Command: " + msg.getCommand() <<endl;
	cout << " SourceIP: " + att["sourceIP"] << endl;
	cout << " SourcePort: " + att["sourcePort"] << endl;
	cout << " DestIP: " + att["destIP"] << endl;
	cout << " DestPort: " + att["destPort"] << endl;
	cout << " ContentLength: " + att["contentLength"] << endl;
}
#endif