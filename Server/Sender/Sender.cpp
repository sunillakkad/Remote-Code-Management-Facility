///////////////////////////////////////////////////////////////////////
// Sender.cpp - Send message to other peer							 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron N5010, Win7 Professional, SP1          //
// Application: Demonstration for CSE 687, Project #4, Spring 2015   //
// Author:      Sunilkumar Lakkad, Syracuse University               //
//              (315) 751-5834, lakkads1@gmail.com                   //
///////////////////////////////////////////////////////////////////////

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include"Sender.h"
#include <iostream>
#include <sys/stat.h>

using namespace ApplicationHelpers;
using namespace Display;
using namespace FileSystem;

//----< Dequeue message from the sending queue >-----------------
void Sender::getQueueMessage()
{
	try
	{
		do
		{
			Message msg = sQueue->deQ();
			interpretingSendingMessage(msg);
		} while (true);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}

}

//----< Enqueue message to the sending queue from Receiving queue >-----------------
void Sender::enQFromReceivingQueue(Message msg)
{
	try
	{
		sQueue->enQ(msg);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< interpret sending message >-----------------
void Sender::interpretingSendingMessage(Message& msg)
{
	string command = msg.getCommand();
	map<string, string> att = msg.getAttributes();
	string destIP = att["destIP"];
	int destPort = atoi(att["destPort"].c_str());
	try{
		displayLine('=');
		Verbose v(true);
		SocketSystem ss;
		SocketConnecter si;
		BlockingQueue<Message> sQueue;
		while (!si.connect(destIP, destPort))
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		handlingSendingReply(si, msg, command);
		si.shutDownSend();
	}
	catch (std::exception& ex){
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
	displayLine('-');
}

//----< handle sending activity >-----------------
void Sender::handlingSendingReply(Socket& si, Message& msg, string command)
{
	if (command == "STRING_HANDLING_REQUEST")
	{
		displayString("Demonstrating: Sending string to one peer to other peer");
		displayLine(' ');
		stringHandlingRequest(si, msg);
	}
	if (command == "STRING_HANDLING_REPLY")
	{
		displayString("Demonstrating: Reply from one peer to other peer - String Received");
		displayLine(' ');
		stringHandlingReply(si, msg);
	}
	if (command == "FILE_SEARCH_RESULTS")
	{
		displayString("Demonstrating: Sending Search Results to Client");
		displayLine(' ');
		searchHandlingReply(si, msg);
	}
	if (command == "FILE_HANDLING_REQUEST")
	{
		displayString("Demonstrating: Sending file to one peer to other peer");
		displayLine(' ');
		fileHandlingRequest(si, msg);
	}
	if (command == "FILE_HANDLING_REPLY")
	{
		displayString("Demonstrating: Reply from one peer to other peer - File Received");
		displayLine(' ');
		fileHandlingReply(si, msg);
	}
	if (command == "FILE_DOWNLOAD_REPLY")
	{
		displayString("Demonstrating: Sending file from server to client");
		displayLine(' ');
		fileDownloadReply(si, msg);
	}
}

//----< sending string handling reply to client >--------
void Sender::stringHandlingReply(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("body:" + att["body"]);

	displayString("Server Sending string received acknowledgement to Client");
}

//----< sending file handling reply to client >--------
void Sender::fileHandlingReply(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("fileName:" + att["fileName"]);
	si.sendString("body:" + att["body"]);

	displayString("Server Sending file received acknowledgement to Client");
}

//----< sending string handling request to client >--------
void Sender::stringHandlingRequest(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("contentLength:" + att["contentLength"]);
	si.sendString("\n");

	const int BufLen = 1024;
	Socket::byte buffer[BufLen];
	std::string text = att["body"];
	for (size_t i = 0; i < BufLen; ++i)
	{
		if (i < text.size())
			buffer[i] = text[i];
		else
			buffer[i] = '#';
	}
	buffer[BufLen - 1] = '\0';
	si.send(BufLen, buffer);
	
	displayString("command:" + msg.getCommand());
	displayString("Client Sending one buffer of string to server");
	Verbose::show("client sending buffer: " + std::string(buffer), always);
}

//----< sending file handling request to client >--------
void Sender::fileHandlingRequest(Socket& si, Message& msg)
{
	map<string, string> att = msg.getAttributes();
	displayString("command:" + msg.getCommand());
	displayString("\nClient sending file located at->" + att["fileName"] + "\n\n");

	if (!findFile(att["fileName"])){
		Verbose::show("file Not Found!!", always);
		return;
	}
	const size_t bufLen = 1024;
	File::byte buffer[bufLen];
	File bufferIn(att["fileName"]);
	bufferIn.open(File::in, File::binary);

	while (true)
	{
		int resultSize = bufferIn.getBuffer(bufLen, buffer);
		att["contentLength"] = std::to_string(resultSize);
		string temp1(buffer, resultSize);

		if (resultSize < 1024)
		{
			for (int i = temp1.size(); i < 1024; i++)
				buffer[i] = '#';
			resultSize = 1024;
		}
		si.sendString("command:" + msg.getCommand());
		si.sendString("sourceIP:" + att["sourceIP"]);
		si.sendString("sourcePort:" + att["sourcePort"]);
		si.sendString("destIP:" + att["destIP"]);
		si.sendString("destPort:" + att["destPort"]);
		si.sendString("fileName:" + att["fileName"]);
		si.sendString("contentLength:" + att["contentLength"]);
		si.sendString("\n");
		si.send(resultSize, buffer);
		displayString("Client Sending  one buffer of " + Path::getName(att["fileName"], true) + " to server");
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			bufferIn.close();
			displayString("----Client has sent whole " + Path::getName(att["fileName"], true) + " to server----");
			break;
		}
	}
}

//----< check file is located on specified path >--------
bool Sender::findFile(std::string name)
{
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

//----< sending file doenload reply to client >--------
void Sender::fileDownloadReply(Socket& si, Message& msg)
{
	map<string, string> att = msg.getAttributes();
	displayString("command:" + msg.getCommand());
	std::string filename = "Server\\" + att["path"] + "\\" + att["filename"];
	std::string fileSpec = FileSystem::Path::getFullFileSpec(filename);
	std::cout << "ON SERVER\n\n";
	std::cout << fileSpec;
	displayString("\n Server sending file located at->" + fileSpec + "\n\n");
	if (!findFile(fileSpec)){
		Verbose::show("file Not Found!!", always);
		return;
	}
	const size_t bufLen = 1024;
	File::byte buffer[bufLen];
	File bufferIn(fileSpec);
	bufferIn.open(File::in, File::binary);
	while (true)
	{
		int resultSize = bufferIn.getBuffer(bufLen, buffer);
		att["contentLength"] = std::to_string(resultSize);
		string temp1(buffer, resultSize);
		if (resultSize < 1024)
		{
			for (int i = temp1.size(); i < 1024; i++)
				buffer[i] = '#';
			resultSize = 1024;
		}
		si.sendString("command:" + msg.getCommand());
		si.sendString("sourceIP:" + att["sourceIP"]);
		si.sendString("sourcePort:" + att["sourcePort"]);
		si.sendString("destIP:" + att["destIP"]);
		si.sendString("destPort:" + att["destPort"]);
		si.sendString("filename:" + att["filename"]);
		si.sendString("contentLength:" + att["contentLength"]);
		si.sendString("\n");
		si.send(resultSize, buffer);
		displayString("Server Sending  one buffer of " + filename + " to Client");
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			bufferIn.close();
			displayString("----Server has sent whole " + filename + " to Client----");
			break;
		}
	}
}

//----< sending search handling reply to client >--------
void Sender::searchHandlingReply(Socket& si, Message& msg)
{
	si.sendString("command:" + msg.getCommand());
	map<string, string> att = msg.getAttributes();
	si.sendString("sourceIP:" + att["sourceIP"]);
	si.sendString("sourcePort:" + att["sourcePort"]);
	si.sendString("destIP:" + att["destIP"]);
	si.sendString("destPort:" + att["destPort"]);
	si.sendString("body:" + att["body"]);

	displayString("Server Sending search results to Client");
}

//----< demonstration >--------------------------------------------------
#ifdef TEST_SENDER
int main()
{
	title("Testing Sender", '=');
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(1000, Socket::IP6);
		BlockingQueue<Message> rQueue;
		BlockingQueue<Message> sQueue;
		Sender sd(&sQueue);

		string test = "Test Message";
		string fileName = "test1.txt";
		Message msg("STRING_HANDLING_REQUEST");
		msg.buildMessage(msg, 1000, 2000, test, test.size());

		displayMessage(msg, "Constructing");
		Message msg2("FILE_HANDLING_REQUEST");
		msg.buildMessage(msg2, 1000, 2000, fileName);
		sQueue.enQ(msg);
		sQueue.enQ(msg2);

		std::thread sThread(&Sender::getQueueMessage, Sender(&sQueue));
		sThread.join();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif
