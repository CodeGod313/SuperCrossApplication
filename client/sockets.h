#pragma once
#define MAX_CONNECTIONS 15
#define BUFF_SIZE 50
#include "includes.h"
#include "package.h"

// CREATE
#ifdef __linux__
int createSocket(char protocol);
#elif _WIN32
SOCKET createSocket(char protocol);
#else

#endif

// BIND
#ifdef __linux__
bool attachAddressAndPort(int sd, std::string ipv4Address, unsigned short int port);
#elif _WIN32
bool attachAddressAndPort(SOCKET sd, std::string ipv4Address, unsigned short int port);
#else

#endif

#ifdef __linux__
bool connectToServer(int socket, std::string ipv4Address, unsigned short int port);
#elif _WIN32
bool connectToServer(SOCKET sd, std::string ipv4Address, unsigned short int port);
#else

#endif

#ifdef __linux__
bool startListen(int sd);
#elif _WIN32
bool startListen(SOCKET sd);
#else

#endif

// returns number of connection in special container
#ifdef __linux__
int acceptConnection(int sd);
#elif _WIN32
SOCKET acceptConnection(SOCKET sd);
#else

#endif

#ifdef __linux__
std::vector<char> receiveData(int sd, bool &connectionClosedByClient);
#elif _WIN32
std::vector<char> receiveData(SOCKET sd, bool &connectionClosedByClient);
#else

#endif

#ifdef __linux__
bool sendData(int sd, std::vector<char> data);
#elif _WIN32
bool sendData(SOCKET sd, std::vector<char> data);
#else
#endif

#ifdef __linux__
void closeSocket(int sd);
#elif _WIN32
void closeSocket(SOCKET sd);
#else
#endif

#ifdef _WIN32
int poll(struct pollfd *pfd, int nfds, int timeout);
#endif

#ifdef _WIN32
int ioctl(SOCKET s, long cmd, u_long *argp);
#endif
