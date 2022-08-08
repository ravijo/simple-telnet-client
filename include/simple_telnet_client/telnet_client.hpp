// Copyright 2022 Ravi Joshi
// Date: 7 Aug 2022

#ifndef SIMPLE_TELNET_CLIENT__TELNET_CLIENT_HPP_
#define SIMPLE_TELNET_CLIENT__TELNET_CLIENT_HPP_

#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <libtelnet.h>

#include <iostream>
#include <string>
#include <vector>

// flag to print requests and responses in the console for debugging purposes
#define DEBUG_MODE false

namespace simple_telnet_client
{
class TelnetClient
{
public:
  TelnetClient(
    const std::string & serverIP,
    const std::string & serverPort,
    const size_t timeOut = 1,
    const size_t bufferSize = 2048);

  // execute a command on the server
  void execute(const std::string & command);

  // receive the response from the server
  std::string response();

  ~TelnetClient();

private:
  int mSockFd;
  std::string mReceivedMsg;
  fd_set mReadFd;
  fd_set mWriteFd;
  struct timeval mTimeout;
  telnet_t * mTelnet;
  std::vector<char> mBuffer;

  static void trampoline(
    telnet_t * telnet,
    telnet_event_t * event,
    void * user_data);

  int makeConnection(
    const std::string & serverIP,
    const std::string & serverPort);

  void telnetEvent(telnet_event_t * event);

  int sendAll(
    const char * mBuffer,
    size_t size);

  void configureTimeout(const size_t seconds);

  void configureReadWriteFd();
};

}  // namespace simple_telnet_client

#endif  // SIMPLE_TELNET_CLIENT__TELNET_CLIENT_HPP_
