/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include"../Client1/Client1.h"
#include "../Server1/Server1.h"
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Message/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/Display.h"
#include "../Receiver/Receiver.h"
#include <string>
#include <thread>
#include <iostream>

using BQueue = BlockingQueue < MessageS >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const MessageS& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void Sendr::postMessage(const MessageS& msg)
{
  sendQ_.enQ(msg);
  string file = FileSystem::Path::fileSpec("../TESTFOLDERCLIENT1", "test.txt");
  string filepath = FileSystem::Path::getFullFileSpec(file);
  Client1 c1("localhost","1001","localhost","2001",filepath);
  c1.enQueue("TEST_FILE_REQUEST",filepath);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	MessageS getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

MessageS Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(string sourceIP, string sourcePort, string destIP, string destPort, string filePath) : c1(sourceIP, sourcePort, destIP, destPort, filePath)
	{
	}
  void start();
  void stop();
  void postMessage(const MessageS& cmd, const MessageS& parameter);
  MessageS getMessage();
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
  BQueue sendQ_;
  BQueue recvQ_;
  Client1 c1;
};

void MockChannel::postMessage(const MessageS& cmd, const MessageS& parameter)
{
	c1.enQueue(cmd, parameter);
	sendQ_.enQ(cmd);
	sendQ_.enQ(parameter);
}

MessageS MockChannel::getMessage()
{
	recvQ_.deQ();
	return c1.deQueue();
}


//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  thread_ = std::thread(
    [this] {
    while (!stop_)
    {
      std::cout << "\n  channel deQing message";
	  MessageS msg = sendQ_.deQ();  // will block here so send quit message when stopping
	  std::cout << "\n  channel enQing message";
      recvQ_.enQ(msg);
	  std::cout << msg;
	}
    std::cout << "\n  Server stopping\n\n";
  });
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(string sourceIP, string sourcePort, string destIP, string destPort, string filePath)
{ 
	return new MockChannel(sourceIP, sourcePort, destIP, destPort, filePath);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
