# syntax=docker/dockerfile:1
# Use Ubuntu 20.04
FROM ubuntu:20.04

# Set the timezone to Tokyo. Otherwise, it asks to set the geographic area later
ENV TZ=Asia/Tokyo
RUN ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && echo $CONTAINER_TIMEZONE > /etc/timezone

# Install dependencies to build the project
RUN apt-get -y update \
    && apt-get install -y --no-install-recommends build-essential=12.8ubuntu1.1 \
    && apt-get install -y --no-install-recommends git=1:2.25.1-1ubuntu3.5 \
    && apt-get install -y --no-install-recommends cmake=3.16.3-1ubuntu1 \
    && apt-get install -y --no-install-recommends tzdata=2022a-0ubuntu0.20.04 \
    && apt-get install -y --no-install-recommends libtelnet-dev=0.21-5 \
    && apt-get install -y --no-install-recommends ca-certificates=20211016~20.04.1 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Get the project from GitHub
RUN git clone https://github.com/ravijo/simple-telnet-client.git /simple-telnet-client

# Browse to the newly created directory
WORKDIR /simple-telnet-client

# Create build directory
RUN mkdir build

# Set build directory as working directory 
WORKDIR /simple-telnet-client/build
