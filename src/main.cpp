// Copyright 2022 Ravi Joshi
// Date: 4 Aug 2022

#include <simple_telnet_client/telnet_client.hpp>
#include <memory>
#include <string>
#include <vector>

int main()
{
  std::unique_ptr<simple_telnet_client::TelnetClient> clientPtr;
  try {
    std::cout << "Connecting to server..." << std::endl;
    clientPtr = std::make_unique<simple_telnet_client::TelnetClient>("192.168.10.20", "23");
  } catch (const std::exception & ex) {
    std::cout << "Error: " << ex.what() << std::endl;
    return 1;
  }
  std::cout << "Successfully connected to server" << std::endl;

  // the server returns "Welcome\r\n" upon a successful connection
  std::cout << "Response: [" << clientPtr->response() << "]" << std::endl;

  // the second recv is needed to capture the empty new line
  std::cout << "Response: [" << clientPtr->response() << "]" << std::endl;

  // define our commands
  const std::vector<std::string> commands = {
    "INIT\r\n", 
    "READ PT3\r\n", 
    "READ PT4\r\n", 
    "REM PT3\r\n"};

  // execute commands and print the response in the terminal
  for (const auto & cmd : commands) {
    clientPtr->execute(cmd);
    std::cout << "Response: [" << clientPtr->response() << "]" << std::endl;
  }

  return 0;
}
