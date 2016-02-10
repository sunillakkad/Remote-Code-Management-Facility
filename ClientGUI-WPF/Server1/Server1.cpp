///////////////////////////////////////////////////////////////////////
// Server1.h - participate as server in message passing communication//
// ver 1.0                                                           //
// Language:    Visual C++, Visual Studio 2013				         //
// Platform:    Levovo Y40, Core i7, Windows 8                       //
// Application: Spring, 2015 : XML Document Model                    //
// Author:      Dhaval Dholakiya, Syracuse University                //
//              (315) 447-7644, ddholaki@syr.edu                     //
///////////////////////////////////////////////////////////////////////

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Receiver/Receiver.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Sender/Sender.h"
#include "../Display/Display.h"
#include "Server1.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace ApplicationHelpers;

//----< Dequeue message from the receiving queue >-----------------
void Server1::extractMessage(BlockingQueue<Message>* rQueue, Sender* sd)
{
	try
	{
		do{
			Message msg = rQueue->deQ();
			sd->enQueueMessage(msg);
		} while (true);
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< initialize server instance >-----------------
Server1::Server1(string sourceIP, int sourcePort, string path)
{
	Verbose::show(string(79, '='), always);
	Verbose::show("Server1 Requirements", always);
	Verbose::show(string(79, '='), always);
	Verbose::show(string(79, ' '), always);
	string str = "Server will listen on IP: " + sourceIP + " and Port: " + to_string(sourcePort);
	Verbose::show(str, always);
	Verbose::show(string(79, ' '), always);
	Verbose::show(string(79, ' '), always);

	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(7070, Socket::IP6);
		BlockingQueue<Message> receivingQueue;
		BlockingQueue<Message> sendingQueue;
		Receiver receiver(&receivingQueue, path);
		Sender sender(&sendingQueue);
		sl.start(receiver);
		thread receivingThread = thread([&, this]{this->extractMessage(&receivingQueue, &sender); });
		thread sendingThread(&Sender::extractMessage, Sender(&sendingQueue));
		sendingThread.join();
		receivingThread.join();

		Verbose::show("press key to exit: ", always);
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< demonstration >--------------------------------------------------
#ifdef TEST_SERVER1
int main()
{
	Server1 server("localhost", 7070, "..\\TESTFOLDERSERVER1\\");
}
#endif