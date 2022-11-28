#pragma once

// standard shit
#include <iostream>
#include <vector>
#include <string>
#include <string.h>

// sockets
#ifdef __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/ioctl.h>

#elif _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#else

#endif