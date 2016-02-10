///////////////////////////////////////////////////////////////////////
// Receiver.h - Receive message from other peer						 //
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
#include "Receiver.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace ApplicationHelpers;
using namespace FileSystem;
using namespace std;
using namespace Display;

//----< Validates the type of incoming request >-----------------
void Receiver::operator()(Socket& socket_)
{
	while (true)
	{
		std::string commandString = socket_.recvString();
		size_t index = commandString.find(":");
		std::string command = commandString.substr(index + 1);
		if (command.size() == 0)
			break;
		Verbose::show(string(79, '='), always);
		Verbose::show("command: " + command);
		if (command == "STRING_HANDLING_REQUEST")
		{
			requestForString(socket_);
			continue; // go back and get another command
		}
		if (command == "STRING_HANDLING_REPLY")
		{
			replyForString(socket_);
			continue;  // go back and get another command
		}
		if (command == "FILE_HANDLING_REQUEST")
		{
			requestForFile(socket_);
			continue; // go back and get another command
		}
		if (command == "FILE_SEARCH_RESULTS")
		{
			handleFileSearchResults(socket_);
		}
		if (command == "FILE_DOWNLOAD_REPLY")
		{
			handleFileDownloadReply(socket_);
		}
		if (command == "FILE_HANDLING_REPLY")
		{
			fileHandlingReply1(socket_);
			continue; // go back and get another command
		}
	}
	socket_.shutDown();
	socket_.close();
}

//----< Check String Handling Request Passed or Failed >-----------------
void Receiver::requestForString(Socket& socket_)
{
	if (stringHandlingRequest(socket_))
		Verbose::show("----String Handling Request test passed----", always);
	else
		Verbose::show("----String Handling Request test failed----", always);
}

//----< Check String Handling Reply Passed or Failed >-----------------
void Receiver::replyForString(Socket& socket_)
{
	if (stringHandlingReply(socket_))
		Verbose::show("----String Handling Reply test passed----", always);
	else
		Verbose::show("----String Handling Reply test failed----", always);
}

//----< Check File Handling Request Passed or Failed >-----------------
void Receiver::requestForFile(Socket& socket_)
{
	if (fileHandlingRequest(socket_))
	{
		ackMessage = "----File Handling Request test passed----";
		Verbose::show("----File Handling Request test passed----", always);
	}
	else
	{
		ackMessage = "----File Handling Request test failed----";
		Verbose::show("----File Handling Request test failed----", always);
	}
}

//----< Check File Handling Reply Passed or Failed >-----------------
void Receiver::replyForFile(Socket& socket_)
{
	if (fileHandlingReply(socket_))
		Verbose::show("----File Handling Reply test passed----", always);
	else
		Verbose::show("----File Handling Reply test failed----", always);
}

//----< Return acknowledgement message >-----------------
string Receiver::getAckMessage()
{
	return ackMessage;
}

//----< Check File Download Request test passed or Failed >-----------------
void Receiver::handleFileDownloadReply(Socket& socket_)
{
	if (handleFileDownload(socket_))
		Verbose::show("--File Download Request test passed--");
	else
		Verbose::show("--File Download Request test failed--");
}

//----< Check File File search Request test passed or Failed >-----------------
void Receiver::handleFileSearchResults(Socket& socket_)
{
	if (handleSearchResults(socket_))
		Verbose::show("--File search Request test passed--");
	else
		Verbose::show("--File search Request test passed--");
}

//----< test string handling request >-----------------------------------------------
bool Receiver::stringHandlingRequest(Socket& socket_)
{

	std::vector<std::string> attributesFill;
	for (int i = 0; i < 6; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "\n")
				break;
			else
				attributesFill.push_back(str);
		}
		else
			break;
	}
	string receivedContent;
	if (!receivingBuffer(socket_, receivedContent))
		return false;
	displayString("Server receiving one buffer of string from Client");
	interpretingReceivingMessageForString(attributesFill, receivedContent);
	return true;
}

//----< interpreting string handling request message for string >------------------------------
void Receiver::interpretingReceivingMessageForString(vector<string> storage, string text)
{
	string oneKB = "1024";
	Message msg("STRING_HANDLING_REPLY");
	map<string, string>& attributes = msg.getAttributes();
	for (auto str : storage)
	{
		size_t index = str.find(":");
		std::string attribute = str.substr(0, index);
		std::string value = str.substr(index + 1);
		attributes[attribute] = value;
	}
	saveContents(text, attributes["contentLength"]);
	if (!(attributes["contentLength"] == oneKB))
	{
		string temp = attributes["sourceIP"];
		attributes["sourceIP"] = attributes["destIP"];
		attributes["destIP"] = temp;
		string temp1 = attributes["sourcePort"];
		attributes["sourcePort"] = attributes["destPort"];
		attributes["destPort"] = temp1;
		attributes["body"] = "string Received Successfully";
		displayString("Server received whole string from Client");
		Verbose::show("\n Received Content  : " + originalContent, always);
		originalContent = "";
		Verbose::show("\n Reveised Content: " + modifiedContent, always);
		modifiedContent = "";
		receivingQueue->enQ(msg);//enQ message after receiving whole string
	}
}

//----< Save content received from the other peer >-----------------
void Receiver::saveContents(string text, string contentLength)
{
	int oneKB = 1024;
	int length = stoi(contentLength);
	originalContent = originalContent + text;
	if (length == oneKB)
	{
		for (unsigned int i = 0; i < text.size() - 1; i++)
			modifiedContent = modifiedContent + text[i];
	}
	else
	{
		for (int i = 0; i < length; i++)
			modifiedContent = modifiedContent + text[i];
	}
}

//----< Receive Buffer from the other peer >-----------------
bool Receiver::receivingBuffer(Socket& socket_, string& receivedContent)
{
	const int BufLen = 1024;
	Socket::byte buffer[BufLen];
	bool ok;
	ok = socket_.recv(BufLen, buffer);
	if (socket_ == INVALID_SOCKET || !ok)
		return false;
	for (size_t i = 0; i < BufLen; ++i)
		receivedContent += buffer[i];
	return true;
}

//----< test string handling reply >-----------------------------------------------
bool Receiver::stringHandlingReply(Socket& socket_)
{
	map<string, string> attributes;
	for (int i = 0; i < 5; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			size_t index = str.find(":");
			std::string attribute = str.substr(0, index);
			std::string value = str.substr(index + 1);
			attributes[attribute] = value;
		}
		else
			break;
	}
	displayString("Client receiving string received acknowledgement from Server" + attributes["sourcePort"]);
	return true;
}


//----< test file handling reply >-----------------------------------------------
bool Receiver::fileHandlingReply(Socket& socket_)
{

	map<string, string> attributes;
	for (int i = 0; i < 6; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			size_t index = str.find(":");
			std::string attribute = str.substr(0, index);
			std::string value = str.substr(index + 1);
			attributes[attribute] = value;
		}
		else
			break;
	}
	displayString("Client receiving file received acknowledgement from Server" + attributes["sourcePort"]);
	return true;
}

//----< test file handling Request >--------------------------------------
bool Receiver::fileHandlingRequest(Socket& socket_)
{
	std::vector<std::string> receiverStorage;
	for (int i = 0; i < 7; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "\n")
				break;
			else
				receiverStorage.push_back(str);
		}
		else
			break;
	}
	string receivedContent;
	if (!receivingBuffer(socket_, receivedContent))
		return false;
	interpretingReceivingMessageForFile(receiverStorage, receivedContent);
	return true;
}

//----< interpreting file handling request message for file >------------------------------
void Receiver::interpretingReceivingMessageForFile(vector<string> storage, string text)
{
	Message message("FILE_HANDLING_REPLY");
	map<string, string>& attributes = message.getAttributes();
	for (auto str : storage)
	{
		size_t index = str.find(":");
		std::string attribute = str.substr(0, index);
		std::string value = str.substr(index + 1);
		attributes[attribute] = value;
	}
	string fileName = Path::getName(attributes["fileName"], true);
	displayString("Server receiving one buffer of " + fileName + " from Client");

	int length = 1024;
	if (!(attributes["contentLength"] == "1024"))
		length = stoi(attributes["contentLength"]);
	FileSystem::File::byte buffer[1024];
	for (int i = 0; i < 1024; i++)
	{
		if (i < length)
			buffer[i] = text[i];
		else
			buffer[i] = ' ';
	}
	string temp = Path::fileSpec(path, fileName);
	string temp1 = Path::getFullFileSpec(temp);
	string save(buffer, length);
	std::ofstream outfile;
	outfile.open(temp1, std::ios_base::app | std::ios_base::binary);
	outfile << save;
	if (!(attributes["contentLength"] == "1024"))
	{
		string swap = attributes["sourceIP"];
		attributes["sourceIP"] = attributes["destIP"];
		attributes["destIP"] = swap;
		string swap1 = attributes["sourcePort"];
		attributes["sourcePort"] = attributes["destPort"];
		attributes["destPort"] = swap1;
		attributes["body"] = "file Received Successfully";
		string fs = Path::fileSpec(path, fileName);
		string filePath = Path::getFullFileSpec(fs);
		displayString("\nServer has Received file Path:" + filePath);
		receivingQueue->enQ(message);//enQ message after receiving whole file
	}
}


void Receiver::fileHandlingReply1(Socket& socket_)
{
	Message message("FILE_HANDLING_REPLY");
	map<string, string>& attributes = message.getAttributes();
	for (int i = 0; i < 6; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (str.size() > 0)
		{
			size_t index = str.find(":");
			std::string attribute = str.substr(0, index);
			std::string value = str.substr(index + 1);
			attributes[attribute] = value;
		}
		else
			break;
	}
	receivingQueue->enQ(message);
}

//----< test file download Request >--------------------------------------
bool Receiver::handleFileDownload(Socket& socket_)
{
	std::vector<std::string> receiverStorage;
	for (int i = 0; i < 7; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "\n")
				break;
			else
				receiverStorage.push_back(str);
		}
		else
			break;
	}
	string receivedContent;
	if (!receivingBuffer(socket_, receivedContent))
		return false;
	interpretingReceivingMessageForDownload(receiverStorage, receivedContent);
	return true;
}

//----< inrterpreting message for file download Request >--------------------------------------
void Receiver::interpretingReceivingMessageForDownload(vector<string> storage, string text)
{
	Message msg("FILE_DOWNLOAD_OK");
	map<string, string>& attributes = msg.getAttributes();
	for (auto str : storage)
	{
		size_t index = str.find(":");
		std::string attribute = str.substr(0, index);
		std::string value = str.substr(index + 1);
		attributes[attribute] = value;
	}
	string fileName = Path::getName(attributes["filename"], true);
	displayString("Client receiving one buffer of " + fileName + " from Server");
	
	int length = 1024;
	if (!(attributes["contentLength"] == "1024"))
		length = stoi(attributes["contentLength"]);
	FileSystem::File::byte buffer[1024];
	for (int i = 0; i < 1024; i++)
	{
		if (i < length)
			buffer[i] = text[i];
		else
			buffer[i] = ' ';
	}
	string temp = Path::fileSpec("ClientGUI-WPF\\TESTFOLDER", fileName);
	string temp1 = Path::getFullFileSpec(temp);
	string save(buffer, length);
	std::ofstream outfile;
	outfile.open(temp1, std::ios_base::app | std::ios_base::binary);
	outfile << save;
	if (!(attributes["contentLength"] == "1024"))
	{
		string swap = attributes["sourceIP"];
		attributes["sourceIP"] = attributes["destIP"];
		attributes["destIP"] = swap;
		string swap1 = attributes["sourcePort"];
		attributes["sourcePort"] = attributes["destPort"];
		attributes["destPort"] = swap1;
		string fs = Path::fileSpec(path, fileName);
		string filePath = Path::getFullFileSpec(fs);
		displayString("\nClient has Received file Path:" + filePath);
		receivingQueue->enQ(msg);//enQ message after receiving whole file
	}
}

//----< test file search Request >--------------------------------------
bool Receiver::handleSearchResults(Socket& socket_)
{
	Message m("FILE_SEARCH_RESULTS");
	std::map<std::string, std::string>& attributes = m.getAttributes();
	std::vector<std::string> searchResults;
	for (int i = 0; i < 6; i++)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		std::string str = socket_.recvString();
		if (socket_ == INVALID_SOCKET)
			return false;
		if (str.size() > 0)
		{
			if (str == "\n")
	
				break;
			else
				searchResults.push_back(str);
		}
		else
			break;
	}

	for (auto s : searchResults)
	{
		size_t index = s.find(":");
		std::string attribute = s.substr(0, index);
		std::string value = s.substr(index + 1);
		attributes[attribute] = value;
	}
	string temp = attributes["sourceIP"];
	attributes["sourceIP"] = attributes["destIP"];
	attributes["destIP"] = temp;
	string temp1 = attributes["sourcePort"];
	attributes["sourcePort"] = attributes["destPort"];
	attributes["destPort"] = temp1;
	receivingQueue->enQ(m);

	return true;
}

//----< test stub >----------------------------------------------
struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

//----< test stub >------------------------------------------------------
#ifdef TEST_RECEIVER
int main()
{
	title("Testing Receiver", '=');
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(9080, Socket::IP6);
		BlockingQueue<Message> queue;
		Receiver receiver(&queue, "../TESTFOLDERSERVER2");
		sl.start(receiver);
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
#endif

