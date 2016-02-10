//////////////////////////////////////////////////////////////////////////
// Executive.cpp - Manage all Packages of Message Passing Communication///
// ver 1.0                                                              //
// Language:    C++, Visual Studio 13.0						            //
// Platform:    Dell Inspiron N5010, Win7 Professional, SP1             //
// Application: Demonstration for CSE 687, Project #4, Spring 2015      //
// Author:      Sunilkumar Lakkad, Syracuse University                  //
//              (315) 751-5834, lakkads1@gmail.com                      //
//////////////////////////////////////////////////////////////////////////

#include"../Client1/Client1.h"
#include "../Server1/Server1.h"
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include "Executive.h"
#include <iostream>
#include <thread>
using namespace std;

//----<the program starts from this point>------------------------------
#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[])
{
	string sourceIP = argv[1];
	int sourcePort = stoi(argv[2]);
	string path = argv[3];

	string s = "Server - Port: " + to_string(sourcePort);
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	SetConsoleTitle(r.c_str());

	Server1 sr(sourceIP, sourcePort, path);//create server instance
}
#endif