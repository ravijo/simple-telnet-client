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
3. Invoke the client using `./client` as shown below:
    ```console
    $ ./client
    response: [Welcome
    ]
    response: [
    ]
    request: [INIT
    ]
    response: [OK
    ]
    request: [READ PT3
    ]
    response: [OK
    ]
    request: [READ PT4
    ]
    response: [OK
    ]
    request: [REM PT3
    ]
    response: [OK
    ]
    ```


## Using `libtelnet` Installed on Custom Location
1. If the `libtelnet` is installed on a custom locaton, we need to provide its path in `CMakeLists.txt`.
2. First, let's find out where is our `libtelnet`
    ```console
    $ whereis libtelnet
    libtelnet: /usr/lib/x86_64-linux-gnu/libtelnet.so /usr/lib/x86_64-linux-gnu/libtelnet.a /usr/include/libtelnet.h
    ```
3. Now, using the above information, we need to modify `CMakeLists.txt` as shown below:
    ```CMakeLists.txt
    set(TELNET_INCLUDE_DIR
      /usr/include
    )
    
    set(TELNET_LIBRARY
      /usr/lib/x86_64-linux-gnu/libtelnet.so
    )
    
    include_directories(include
      ${TELNET_INCLUDE_DIR}
    )
    
    add_executable(client
      src/telnet_client.cpp
      src/main.cpp
    )
    
    target_link_libraries(client
      ${TELNET_LIBRARY}
    )
    ```
