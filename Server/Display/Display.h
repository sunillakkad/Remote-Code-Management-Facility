#ifndef DISPLAY_H
#define DISPLAY_H
///////////////////////////////////////////////////////////////////////
// Display.h - display output on console							 //
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
* It outputs the functionality of requirements. It contains the declarations 
  of the methods defined in Display.cpp

Public Interface:
=================
	void displayMessage(Message& msg,string type);
	void displayString(string str);
	void displayLine(char lineType);
	void displayPeerInformation(string peerName, string ip, int port);

Required Files:
===============
Display.cpp, AppHelpers.h, Message.h

Build Command:
==============
cl /EHa /TEST_DISPLAY Dispaly.cpp

Maintenance History:
====================
ver 1.0 : 27 April 15
- first release
*/

#include "../Message/Message.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Dispaly namespace
namespace Display
{
	void displayMessage(Message& msg,string type);
	void displayString(string str);
	void displayLine(char lineType);
	void displayPeerInformation(string peerName, string ip, int port);
};
#endif