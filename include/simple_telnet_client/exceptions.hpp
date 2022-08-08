// Copyright 2022 Ravi Joshi
// Date: 7 Aug 2022

#ifndef SIMPLE_TELNET_CLIENT__EXCEPTIONS_HPP_
#define SIMPLE_TELNET_CLIENT__EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace simple_telnet_client
{
class NotImplemented : public std::logic_error
{
public:
  NotImplemented()
  : std::logic_error("Function not yet implemented") {}
};

class SocketConnectionError : public std::runtime_error
{
public:
  explicit SocketConnectionError(const std::string & err)
  : std::runtime_error(err) {}
};
}  // namespace simple_telnet_client

#endif  // SIMPLE_TELNET_CLIENT__EXCEPTIONS_HPP_
