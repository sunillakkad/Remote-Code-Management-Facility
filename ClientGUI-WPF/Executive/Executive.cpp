///////////////////////////////////////////////////////////////////////
// Executive.h - Manage all Packages of Message Passing Communication//
// ver 1.0                                                           //                                                           //
// Language:    Visual C++, Visual Studio 2013				         //
// Platform:    Levovo Y40, Core i7, Windows 8                       //
// Application: Spring, 2015 : XML Document Model                    //
// Author:      Dhaval Dholakiya, Syracuse University                //
//              (315) 447-7644, ddholaki@syr.edu                     //
///////////////////////////////////////////////////////////////////////

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
	string destIP = argv[3];
	int destPort = stoi(argv[4]);
	string file = FileSystem::Path::fileSpec("../TESTFOLDERCLIENT1" , "test.txt");
	string filepath = FileSystem::Path::getFullFileSpec(file);
	Client1 client(sourceIP, std::to_string(sourcePort),destIP, std::to_string(destPort), filepath);				//create client instance
}
#endif