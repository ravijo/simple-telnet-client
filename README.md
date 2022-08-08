# simple-telnet-client
Simplest Telnet Client Ever Made in C/C++ 


## Dependencies
1. [libtelnet](https://github.com/seanmiddleditch/libtelnet): Install it easily using the folllwing command:
    ```console
    $ sudo apt install libtelnet
    ```

## Installation
1. Make sure to download the repository. Use `git clone https://github.com/ravijo/simple-telnet-client.git` or download zip as per your convenience.
2. Create build directory and go inside it using `mkdir build && cd build`
3. Invoke `cmake` as `cmake ..`
3. Build files using `make`

## Debug
In order to see the request and response from the server, please set `DEBUG_MODE` as shown below:
https://github.com/ravijo/simple-telnet-client/blob/84a05652415afc40d7171fbea5a5a33f98f60a80/include/simple_telnet_client/telnet_client.hpp#L17-L18

## Run
1. Please set the IP address and port number of the telnet server as shown below:
https://github.com/ravijo/simple-telnet-client/blob/84a05652415afc40d7171fbea5a5a33f98f60a80/src/main.cpp#L14
2. Do not forget to compile after changes
3. Invoke the client using `./client`
