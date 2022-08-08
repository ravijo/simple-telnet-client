// Copyright 2022 Ravi Joshi
// Date: 7 Aug 2022

#include <simple_telnet_client/exceptions.hpp>
#include <simple_telnet_client/telnet_client.hpp>
#include <string>

namespace simple_telnet_client
{

TelnetClient::TelnetClient(
  const std::string & serverIP,
  const std::string & serverPort,
  const size_t timeOut,
  const size_t bufferSize)
: mBuffer(bufferSize)
{
  // let's make socket
  mSockFd = makeConnection(serverIP, serverPort);

  // socket related configuarations
  configureReadWriteFd();
  configureTimeout(timeOut);

  // telnet options
  const telnet_telopt_t telnetOpts[] = {
    {TELNET_TELOPT_ECHO, TELNET_WONT, TELNET_DO},
    {TELNET_TELOPT_TTYPE, TELNET_WILL, TELNET_DONT},
    {TELNET_TELOPT_COMPRESS2, TELNET_WONT, TELNET_DO},
    {TELNET_TELOPT_MSSP, TELNET_WONT, TELNET_DO},
    {-1, 0, 0}};

  // initialize telnet box
  mTelnet = telnet_init(telnetOpts, trampoline, 0, this);
}

int TelnetClient::makeConnection(
  const std::string & serverIP,
  const std::string & serverPort)
{
  int retVal;
  int sockFd;
  struct addrinfo * addrInfo;
  struct addrinfo addrHints = {};

  // look up server host
  addrHints.ai_family = AF_INET;
  addrHints.ai_socktype = SOCK_STREAM;

  if ((retVal = getaddrinfo(serverIP.c_str(), serverPort.c_str(), &addrHints, &addrInfo)) != 0) {
    throw SocketConnectionError("getaddrinfo() failed: " + serverIP + " " + gai_strerror(retVal));
  }

  // create server socket
  if ((sockFd = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol)) == -1) {
    throw SocketConnectionError("socket() failed: " + std::string(strerror(errno)));
  }

  // connect
  if (connect(sockFd, addrInfo->ai_addr, addrInfo->ai_addrlen) == -1) {
    auto error = std::string(strerror(errno));
    close(sockFd);
    throw SocketConnectionError("connect() failed: " + error);
  }

  // free address lookup info
  freeaddrinfo(addrInfo);

  return sockFd;
}

void TelnetClient::trampoline(
  telnet_t *,
  telnet_event_t * event,
  void * user_data)
{
  TelnetClient * me = static_cast<TelnetClient *>(user_data);
  me->telnetEvent(event);
}

void TelnetClient::telnetEvent(telnet_event_t * event)
{
  switch (event->type) {
    // data received
    case TELNET_EV_DATA:
      mReceivedMsg = std::string(event->data.buffer, event->data.size);
      #if DEBUG_MODE
      std::cerr << "response: [" << mReceivedMsg << "]" << std::endl;
      #endif
      break;
    // data must be sent
    case TELNET_EV_SEND:
      sendAll(event->data.buffer, event->data.size);
      break;
    // execute to enable local feature (or receipt)
    case TELNET_EV_DO:
      throw NotImplemented();
    // demand to disable local feature (or receipt)
    case TELNET_EV_DONT:
      throw NotImplemented();
    // respond to TTYPE commands
    case TELNET_EV_TTYPE:
      throw NotImplemented();
    // respond to particular subnegotiations
    case TELNET_EV_SUBNEGOTIATION:
      throw NotImplemented();
    // error
    case TELNET_EV_ERROR:
      throw std::runtime_error("telnet error: " + std::string(event->error.msg));
    default:
      // ignore
      break;
  }
}

int TelnetClient::sendAll(const char * buffer, size_t size)
{
  int retVal = -1;  // default value

  // send data
  while (size > 0) {
    retVal = send(mSockFd, buffer, size, 0);
    if (retVal == 0 || errno == EINTR) {
      // try again
      continue;
    } else if (retVal == -1) {
      throw std::runtime_error("send() failed: " + std::string(strerror(errno)));
    }

    // update pointer and size to see if we've got more to send
    buffer += retVal;
    size -= retVal;
  }
  return retVal;
}

void TelnetClient::configureReadWriteFd()
{
  // clear the set ahead of time
  FD_ZERO(&mReadFd);
  FD_ZERO(&mWriteFd);

  // add our descriptors to the set
  FD_SET(mSockFd, &mReadFd);
  FD_SET(mSockFd, &mWriteFd);
}

void TelnetClient::configureTimeout(const size_t seconds)
{
  mTimeout.tv_sec = seconds;
  mTimeout.tv_usec = 0;
}

void TelnetClient::execute(const std::string & command)
{
  auto retVal = select(mSockFd + 1, NULL, &mWriteFd, NULL, &mTimeout);

  if (retVal == -1) {
    // error occurred in select()
    throw std::runtime_error("select() failed: " + std::string(strerror(errno)));
  } else if (retVal == 0) {
    throw std::runtime_error("timeout occurred");
  } else {
    // send the execute
    if (FD_ISSET(mSockFd, &mWriteFd)) {
      #if DEBUG_MODE
      // print only for debugging
      std::cerr << "request: [" << command << "]" << std::endl;
      #endif
      telnet_send(mTelnet, command.c_str(), command.length());
    }
  }
}

std::string TelnetClient::response()
{
  // erase previously received msg by setting it to an empty string
  mReceivedMsg.clear();

  auto retVal = select(mSockFd + 1, &mReadFd, NULL, NULL, &mTimeout);
  if (retVal == -1) {
    // error occurred in select()
    throw std::runtime_error("select() failed: " + std::string(strerror(errno)));
  } else if (retVal == 0) {
    throw std::runtime_error("timeout occurred");
  } else {
    // response the response
    if (FD_ISSET(mSockFd, &mReadFd)) {
      do {
        auto received = recv(mSockFd, mBuffer.data(), mBuffer.size(), 0);
        if (received > 0) {
          telnet_recv(mTelnet, mBuffer.data(), received);
        } else if (received == 0) {
          throw std::runtime_error("connection has been closed.");
        } else {
          throw std::runtime_error("recv() failed: " + std::string(strerror(errno)));
        }
      } while (errno == EINTR);
    }
  }
  return mReceivedMsg;
}

TelnetClient::~TelnetClient()
{
  // clean up
  telnet_free(mTelnet);
  close(mSockFd);
}

}  // namespace simple_telnet_client
