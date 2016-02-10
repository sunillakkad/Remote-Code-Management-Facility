///////////////////////////////////////////////////////////////////////
// Sender.h - Send message to other peer							 //
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
using namespace FileSystem;
using namespace Display;

//----< Dequeue message from the sending queue >-----------------
void Sender::extractMessage()
{
	try
	{

		Message message = senderQueue->deQ();
		validatingMessage(message);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< Enqueue message to the sending queue from Receiving queue >-----------------
void Sender::enQueueMessage(Message message)
{
	try
	{
		senderQueue->enQ(message);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< interpret sending message >-----------------
void Sender::validatingMessage(Message& message)
{
	string command = message.getCommand();
	map<string, string> att = message.getAttributes();
	string destIP = att["destIP"];
	displayLine('=');
	try{
		int destPort = atoi(att["destPort"].c_str());
		Verbose v(true);
		SocketSystem ss;
		SocketConnecter si;
		BlockingQueue<Message> sQueue;
		int connectCount = 0;
		while (!si.connect(destIP, destPort))
		{
			connectCount++;
			Verbose::show("client waiting to connect");
			::Sleep(2000);
			Verbose::show(string(79, '='), always);
			if (connectCount == 3)
				return;
		}
		handlingSendingRequest(si,message,command);
		si.shutDownSend();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< handling sending message >-----------------
void Sender::handlingSendingRequest(Socket& si, Message& message, string command)
{
	if (command == "STRING_HANDLING_REQUEST")
	{
		displayString("Demonstrating: Sending string to Server");
		displayLine(' ');
		stringHandlingRequest(si, message);
	}
	if (command == "STRING_HANDLING_REPLY")
	{
		displayString("Demonstrating: Receiving string sent acknowledgement from Server");
		displayLine(' ');
		stringHandlingReply(si, message);
	}
	if (command == "FILE_HANDLING_REQUEST")
	{
		displayString("Demonstrating: Sending file to Server");
		displayLine(' ');
		fileHandlingRequest(si, message);
	}
	if (command == "FILE_HANDLING_REPLY")
	{
		displayString("Demonstrating: Receiving file sent acknowledgement from Server");
		displayLine(' ');
		fileHandlingReply(si, message);
	}
	if (command == "FILE_SEARCH_PATTERN")
	{
		displayString("Demonstrating: Sending search pattern to Server");
		displayLine(' ');
		patternSearchRequest(si, message);
	}
	if (command == "FILE_TEXT_SEARCH")
	{
		displayString("Demonstrating: Sending search text to Server");
		displayLine(' ');
		textSearchRequest(si, message);
	}
	if (command == "FILE_DOWNLOAD_REQUEST")
	{
		displayString("Demonstrating: Sending file download request to Server");
		displayLine(' ');
		downloadRequest(si, message);
	}
}

//----< sending string handling request to other peer >--------
void Sender::stringHandlingRequest(Socket& socket, Message& message)
{
	socket.sendString("command:" + message.getCommand());
	map<string, string> attributes = message.getAttributes();
	socket.sendString("sourceIP:" + attributes["sourceIP"]);
	socket.sendString("sourcePort:" + attributes["sourcePort"]);
	socket.sendString("destIP:" + attributes["destIP"]);
	socket.sendString("destPort:" + attributes["destPort"]);
	socket.sendString("contentLength:" + attributes["contentLength"]);
	socket.sendString("\n");
	const int BufLen = 1024;
	Socket::byte buffer[BufLen];
	std::string text = attributes["body"];
	for (size_t i = 0; i < BufLen; ++i)
	{
		if (i < text.size())
			buffer[i] = text[i];
		else
			buffer[i] = '#';
	}
	buffer[BufLen - 1] = '\0';
	socket.send(BufLen, buffer);

	displayString("command:" + message.getCommand());
	displayString("Client Sending one buffer of string to server");
	Verbose::show("client sending buffer: " + std::string(buffer), always);
}
//----< sending string handling reply to other peer >--------
void Sender::stringHandlingReply(Socket& socket, Message& message)
{
	Sleep(4000);
	socket.sendString("command:" + message.getCommand());
	map<string, string> attributes = message.getAttributes();
	socket.sendString("sourceIP:" + attributes["sourceIP"]);
	socket.sendString("sourcePort:" + attributes["sourcePort"]);
	socket.sendString("destIP:" + attributes["destIP"]);
	socket.sendString("destPort:" + attributes["destPort"]);
	socket.sendString("body:" + attributes["body"]);

	Verbose::show(string(79, '='), always);
	displayString("Demonstrating: Reply from one peer to other peer - String Received");
	displayString("Server Sending string received acknowledgement to Client" + attributes["destPort"]);
}


//----< sending file handling request to other peer >--------
void Sender::fileHandlingRequest(Socket& socket, Message& message)
{
	map<string, string> attributes = message.getAttributes();
	displayString("command:" + message.getCommand());
	displayString("\nClient sending file located at->" + attributes["fileName"] + "\n\n");
	ifstream f((attributes["fileName"]).c_str());
	if (!(f.good())) {
		f.close();
		Verbose::show("file Not Found!!", always);
		return;
	}
	f.close();
	const size_t bufLen = 1024;
	File::byte buffer[bufLen];
	File bufferIn(attributes["fileName"]);
	bufferIn.open(File::in, File::binary);
	while (true){
		int resultSize = bufferIn.getBuffer(bufLen, buffer);
		attributes["contentLength"] = std::to_string(resultSize);
		string temp1(buffer, resultSize);
		if (resultSize < 1024){
			for (int i = temp1.size(); i < 1024; i++)
				buffer[i] = '#';
			resultSize = 1024;
		}
		socket.sendString("command:" + message.getCommand());
		socket.sendString("sourceIP:" + attributes["sourceIP"]);
		socket.sendString("sourcePort:" + attributes["sourcePort"]);
		socket.sendString("destIP:" + attributes["destIP"]);
		socket.sendString("destPort:" + attributes["destPort"]);
		socket.sendString("fileName:" + attributes["fileName"]);
		socket.sendString("contentLength:" + attributes["contentLength"]);
		socket.sendString("\n");
		socket.send(resultSize, buffer);
		displayString("Client Sending one buffer of " + Path::getName(attributes["fileName"], true) + " to server");
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			bufferIn.close();
			displayString("----Client has sent whole " + Path::getName(attributes["fileName"], true) + "file to server----");
			Verbose::show(string(79, '='), always);
			Verbose::show(string(79, ' '), always);
			Verbose::show(string(79, ' '), always);
			break;
		}
	}
}

//----< sending file handling reply to other peer >--------
void Sender::fileHandlingReply(Socket& socket, Message& message)
{
	socket.sendString("command:" + message.getCommand());
	map<string, string> attributes = message.getAttributes();
	socket.sendString("sourceIP:" + attributes["sourceIP"]);
	socket.sendString("sourcePort:" + attributes["sourcePort"]);
	socket.sendString("destIP:" + attributes["destIP"]);
	socket.sendString("destPort:" + attributes["destPort"]);
	socket.sendString("fileName:" + attributes["fileName"]);
	socket.sendString("body:" + attributes["body"]);

	Verbose::show(string(79, '='), always);
	displayString("Demonstrating: Reply from one peer to other peer - File Received");
	displayString("Server Sending file received acknowledgement to Client " + attributes["destPort"]);
	Verbose::show(string(79, '='), always);
	Verbose::show(string(79, ' '), always);
	Verbose::show(string(79, ' '), always);
}

//----< handling downloading request >-----------------
void Sender::downloadRequest(Socket& socket, Message& message)
{
	Sleep(4000);
	socket.sendString("command:" + message.getCommand());
	map<string, string> attributes = message.getAttributes();
	socket.sendString("sourceIP:" + attributes["sourceIP"]);
	socket.sendString("sourcePort:" + attributes["sourcePort"]);
	socket.sendString("destIP:" + attributes["destIP"]);
	socket.sendString("destPort:" + attributes["destPort"]);
	socket.sendString("path:" + attributes["path"]);
	socket.sendString("filename:" + attributes["filename"]);
	displayString("Demonstrating: download file server");
	displayString("Client sending filename to download from server" + attributes["destPort"]);
}

//----< handling test search request >-----------------
void Sender::textSearchRequest(Socket& socket, Message& message)
{
	Sleep(4000);
	socket.sendString("command:" + message.getCommand());
	map<string, string> attributes = message.getAttributes();
	socket.sendString("sourceIP:" + attributes["sourceIP"]);
	socket.sendString("sourcePort:" + attributes["sourcePort"]);
	socket.sendString("destIP:" + attributes["destIP"]);
	socket.sendString("destPort:" + attributes["destPort"]);
	socket.sendString("path:" + attributes["path"]);
	socket.sendString("pattern:" + attributes["pattern"]);
	socket.sendString("text:" + attributes["text"]);

	Verbose::show(string(79, '='), always);
	displayString("Demonstrating: text search request to server");
	displayString("Server Sending pattern and text for matching to client" + attributes["destPort"]);
}

//----< handling file search by pattern request >-----------------
void Sender::patternSearchRequest(Socket& socket, Message& message)
{
	Sleep(4000);
	socket.sendString("command:" + message.getCommand());
	map<string, string> attributes = message.getAttributes();
	socket.sendString("sourceIP:" + attributes["sourceIP"]);
	socket.sendString("sourcePort:" + attributes["sourcePort"]);
	socket.sendString("destIP:" + attributes["destIP"]);
	socket.sendString("destPort:" + attributes["destPort"]);
	socket.sendString("path:" + attributes["path"]);
	socket.sendString("pattern:" + attributes["pattern"]);

	Verbose::show(string(79, '='), always);
	displayString("Demonstrating: Pattern search request to server");
	displayString("Server Sending pattern search for matching to client" + attributes["destPort"]);
}

//----< Test Stub >--------------------------------------------------
#ifdef TEST_SENDER
int main()
{
	title("Testing Sender", '=');
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(5000, Socket::IP6);
		BlockingQueue<Message> sendingQueue;
		Sender sd(&sendingQueue);
		string test = "Test Message";
		string fileName = "test1.txt";
		Message message("STRING_HANDLING_REQUEST");
		message.buildMessage(message, 5000, 8000, test, test.size());

		displayMessage(message, "Constructing");
		Message message2("FILE_HANDLING_REQUEST");
		message.buildMessage(message2, 5000, 8000, fileName);
		sendingQueue.enQ(message);
		sendingQueue.enQ(message2);
		std::thread sThread(&Sender::extractMessage, Sender(&sendingQueue));
		sThread.join();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif
