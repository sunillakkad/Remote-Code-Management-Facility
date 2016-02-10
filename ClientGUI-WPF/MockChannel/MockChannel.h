#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015                 //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using MessageS = std::string;

struct ISendr
{
	virtual void postMessage(const MessageS& msg) = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void postMessage(const MessageS& cmd, const MessageS &parameter) = 0;
  virtual std::string getMessage() = 0;

};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
	DLL_DECL IMockChannel* createMockChannel(std::string sourceIP, std::string sourcePort, std::string destIP, std::string destPort, std::string filePath);
  };
}

#endif


