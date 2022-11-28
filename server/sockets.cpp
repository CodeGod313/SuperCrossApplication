#include "sockets.h"

std::vector<struct sockaddr_in> clientsInfo;

#ifdef __linux__
std::vector<int> clients;
#elif _WIN32
std::vector<SOCKET> clients;
#else

#endif

#ifdef _WIN32
static bool isConfigured = false;
bool configure()
{
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) == -1)
    {
        std::cout << "ERROR: Sockets configuration failed. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}
#endif

// CREATE
#ifdef __linux__
int createSocket(char protocol)
{
    int type;
    switch (protocol)
    {
    case 't':
        type = SOCK_STREAM;
        break;
    case 'u':
        type = SOCK_DGRAM;
        break;

    default:
        std::cout << "WARNING: socket protocol setted to TCP" << std::endl;
        type = SOCK_STREAM;
        break;
    }
    int sd = socket(AF_INET, type, 0);
    if (sd == -1)
    {
        close(sd);
        std::cout << "ERROR: Socket creation failed" << std::endl;
    }
    return sd;
}
#elif _WIN32
SOCKET createSocket(char protocol)
{
    int type;
    switch (protocol)
    {
    case 't':
        type = SOCK_STREAM;
        break;
    case 'u':
        type = SOCK_DGRAM;
        break;

    default:
        std::cout << "WARNING: socket protocol setted to TCP" << std::endl;
        type = SOCK_STREAM;
        break;
    }
    if (!isConfigured && !configure())
    {
        return -1;
    }
    SOCKET sd = socket(AF_INET, type, 0);
    if (sd == INVALID_SOCKET)
    {
        std::cout << "ERROR: Socket creation failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(sd);
        WSACleanup();
        return -1;
    }
    return sd;
}
#else

#endif

// BIND
#ifdef __linux__
bool attachAddressAndPort(int sd, std::string ipv4Address, unsigned short int port)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ipv4Address.c_str());
    address.sin_port = htons(port);
    int errStat = bind(sd, (struct sockaddr *)&address, sizeof(address));
    if (errStat == -1)
    {
        close(sd);
        std::cout << "ERROR: Socket bind failed" << std::endl;
        return false;
    }
    return true;
}
#elif _WIN32
bool attachAddressAndPort(SOCKET sd, std::string ipv4Address, unsigned short int port)
{
    in_addr convertedIp;
    int errStat = inet_pton(AF_INET, ipv4Address.data(), &convertedIp);
    if (errStat <= 0)
    {
        std::cout << "ERROR: Can not translate ip";
        return false;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr = convertedIp;
    address.sin_port = htons(port);
    errStat = bind(sd, (struct sockaddr *)&address, sizeof(address));
    if (errStat != 0)
    {
        std::cout << "ERROR: Socket bind failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(sd);
        WSACleanup();
        return false;
    }
    return true;
}
#else

#endif

#ifdef __linux__
bool connectToServer(int sd, std::string ipv4Address, unsigned short int port)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ipv4Address.data());
    address.sin_port = htons(port);
    if (connect(sd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "ERROR: can not connect to server" << std::endl;
        close(sd);
        return false;
    }
    return true;
}
#elif _WIN32
bool connectToServer(SOCKET sd, std::string ipv4Address, unsigned short int port)
{

    in_addr convertedIp;
    int errStat;
    errStat = inet_pton(AF_INET, ipv4Address.data(), &convertedIp);
    if (errStat <= 0)
    {
        closesocket(sd);
        WSACleanup();
        std::cout << "ERROR: Can not translate ip" << std::endl;
        return false;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr = convertedIp;
    errStat = connect(sd, (struct sockaddr *)&address, sizeof(address));
    if (errStat != 0)
    {
        closesocket(sd);
        WSACleanup();
        std::cout << "ERROR: can not connect to server" << std::endl;
        return false;
    }
    return true;
}
#else

#endif

#ifdef __linux__
bool startListen(int sd)
{
    if (listen(sd, MAX_CONNECTIONS) < 0)
    {
        std::cout << "ERROR: can not listen to socket" << std::endl;
        close(sd);
        return false;
    }
    return true;
}
#elif _WIN32
bool startListen(SOCKET sd)
{
    int errStat = listen(sd, MAX_CONNECTIONS);
    if (errStat != 0)
    {
        std::cout << "ERROR: can not listen to socket" << std::endl;
        closesocket(sd);
        WSACleanup();
        return false;
    }
    return true;
}
#else

#endif

// returns number of connection in special container
#ifdef __linux__
int acceptConnection(int sd)
{
    struct sockaddr_in address;
    int client = accept(sd, (struct sockaddr *)&address, NULL);
    if (client < 0)
    {
        std::cout << "ERROR: can not accept the client" << std::endl;
        close(client);
        return -1;
    }
    clients.push_back(client);
    clientsInfo.push_back(address);
    return client;
}
#elif _WIN32
SOCKET acceptConnection(SOCKET sd)
{
    struct sockaddr_in address;
    SOCKET client = accept(sd, (struct sockaddr *)&address, NULL);
    if (client == INVALID_SOCKET)
    {
        std::cout << "ERROR: can not accept the client" << std::endl;
        closesocket(client);
        WSACleanup();
        return -1;
    }
    clients.push_back(client);
    clientsInfo.push_back(address);
    return client;
}
#else

#endif

#ifdef __linux__
std::vector<char> receiveData(int sd, bool &connectionClosedByClient)
{
    std::vector<char> buff(BUFF_SIZE);
    int rc;
    if (rc = recv(sd, buff.data(), buff.size(), 0) < 0)
    {
        std::cout << "ERROR: can not receive data" << std::endl;
        close(sd);
        return std::vector<char>();
    }
    connectionClosedByClient = (rc == 0) ? true : false;
    return buff;
}
#elif _WIN32
std::vector<char> receiveData(SOCKET sd, bool &connectionClosedByClient)
{
    std::vector<char> buff(BUFF_SIZE);
    int rc;
    if (rc = recv(sd, buff.data(), buff.size(), 0) < 0)
    {
        std::cout << "ERROR: can not receive data" << std::endl;
        closesocket(sd);
        WSACleanup();
        return std::vector<char>();
    }
    connectionClosedByClient = (rc == 0) ? true : false;
    return buff;
}
#else

#endif

#ifdef __linux__
bool sendData(int sd, std::vector<char> data)
{
    std::vector<std::vector<char>> packages;
    packages = composePackages(data, BUFF_SIZE);
    for (auto &package : packages)
    {
        if (send(sd, package.data(), package.size(), 0) < 0)
        {
            std::cout << "ERROR: can not send data" << std::endl;
            return false;
        }
    }
    return true;
}
#elif _WIN32
bool sendData(SOCKET sd, std::vector<char> data)
{
    std::vector<std::vector<char>> packages;
    packages = composePackages(data, BUFF_SIZE);
    for (auto &package : packages)
    {
        if (send(sd, package.data(), package.size(), 0) < 0)
        {
            std::cout << "ERROR: can not send data" << std::endl;
            return false;
        }
    }
    return true;
}
#else
#endif

#ifdef __linux__
void closeSocket(int sd)
{
    close(sd);
}
#elif _WIN32
void closeSocket(SOCKET sd)
{
    closesocket(sd);
}
#else
#endif

#ifdef _WIN32
int poll(struct pollfd *pfd, int nfds, int timeout)
{
    return WSAPoll(pfd, nfds, timeout);
}
#endif

#ifdef _WIN32
int ioctl(SOCKET s, long cmd, u_long *argp)
{
    return ioctlsocket(s, cmd, argp);
}
#endif
