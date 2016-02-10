///////////////////////////////////////////////////////////////////////
// Client.h - participate as client in message passing communication //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron N5010, Win7 Professional, SP1          //
// Application: Demonstration for CSE 687, Project #4, Spring 2015   //
// Author:      Sunilkumar Lakkad, Syracuse University               //
//              (315) 751-5834, lakkads1@gmail.com                   //
///////////////////////////////////////////////////////////////////////

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include "Client1.h"
#include <iostream>
#include <condition_variable>

using namespace ApplicationHelpers;
using namespace std;
using namespace Display;

//----< Dequeue message from the receiving queue >-----------------
void Client1::extractMessage(BlockingQueue<Message>* receivingQueue, Sender* sender)
{
	try
	{
		do{
			Message message = receivingQueue->deQ();
			sender->enQueueMessage(message);
		} while (true);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}

}

//----< Enqueue message for processing >-----------------
void Client1::enQueue(const std ::string& cmd, const std::string& param)
{
	try	{
		Sender sender(&sendingQueue);
		Message message(cmd);
		if (cmd == "STRING_HANDLING_REQUEST")
		{
			message.buildMessage(message, sourcePort, destPort, param, param.size());
		}
		if (cmd == "FILE_HANDLING_REQUEST")
		{
			message.buildMessage(message, sourcePort, destPort, param);
		}
		if (cmd == "FILE_SEARCH_PATTERN")
		{
			auto index = param.find("\\");
			std::string path = param.substr(0, index);
			std::string pattern = param.substr(index + 1, param.length() - 1);
			message.buildMessage(message, sourcePort, destPort, path, pattern);
		}
		if (cmd == "FILE_TEXT_SEARCH")
		{
			auto index1 = param.find_first_of("\\");
			auto index2 = param.find_last_of("\\");
			std::cout << param;
			std::string path = param.substr(0, index1);
			std::string pattern = param.substr(index1 + 1, index2 - index1-1);
			std::string text = param.substr(index2 + 1, param.length() - 1);
			message.buildMessage(message,sourcePort,destPort,path,pattern,text);
		}
		if (cmd == "FILE_DOWNLOAD_REQUEST")
		{
			auto index = param.find("\\");
			std::string path = param.substr(0, index);
			std::string filename = param.substr(index + 1, param.length() - 1);
			message.buildDownloadMessage(message, sourcePort, destPort, path,filename);
		}
		sendingQueue.enQ(message);
		thread sendingThread(&Sender::extractMessage, Sender(&sendingQueue));
		sendingThread.join();
	}
	catch (std::exception& ex)	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< Dequeue message from the receiving queue >-----------------
string Client1::deQueue()
{
	Sleep(1000);
	Verbose v(true);
	SocketSystem ss;
	SocketListener sl(sourcePort, Socket::IP6);
	Receiver receiver(&receivingQueue, "ClientGUI-WPF\\TESTFOLDER");
	sl.start(receiver);

	Message message = receivingQueue.deQ();

	map<string, string> attributes = message.getAttributes();
	string str = "";
	if (message.getCommand() == "FILE_SEARCH_RESULTS")
		str = attributes["body"];
	else if (message.getCommand() == "FILE_DOWNLOAD_OK")
		str = "FILE_DOWNLOAD_OK";
	else if (message.getCommand() == "FILE_HANDLING_REPLY")
		str = "FILE_HANDLING_REPLY";

	return str;
}

//----< initialize client instance >-----------------
Client1::Client1(string sIP, string sPort, string dIP, string dPort,string fPath)
{
	sourceIP = sIP;
	sourcePort = std::stoi(sPort);
	destIP = dIP;
	destPort = std::stoi(dPort);
	filePath = fPath;

	string s = "Client1- Port: " + to_string(sourcePort);
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	SetConsoleTitle(r.c_str());

	title("Testing Socket Client", '=');
	displayPeerInformation("Client", sourceIP, sourcePort);
}

//----< Test Stub >--------------------------------------------------

#ifdef TEST_CLIENT1
int main()
{
	string fs = FileSystem::Path::fileSpec("..\\TESTFOLDER\\", "text.txt");
	string path = FileSystem::Path::getFullFileSpec(fs);
	Client1 cl("localhost", "1001","localhost", "2001",path);
}
#endif