#include "singleThreadServer.h"

void launchSingleThreadServer(char protocol)
{

#ifdef __linux__
    int serverSocket;
#elif _WIN32
    SOCKET serverSocket;
#else
#endif

    serverSocket = createSocket(protocol);
    if (serverSocket == -1)
    {
        exit(-1);
    }
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        std::cout << "ERROR: can not make server socket reusable" << std::endl;
        closeSocket(serverSocket);
        exit(-1);
    }
    if (ioctl(serverSocket, FIONBIO, (u_long *)(char *)&opt) < 0)
    {
        std::cout << "ERROR: can not make server socket unblocking" << std::endl;
        closeSocket(serverSocket);
        exit(-1);
    }

    if (!attachAddressAndPort(serverSocket, SERVER_ADDRESS, SERVER_PORT))
    {
        exit(-1);
    }

    if (!startListen(serverSocket))
    {
        exit(-1);
    }

    int numberOfSockets = 1;
    struct pollfd sds[MAX_CONNECTIONS + 1];
    memset(sds, 0, sizeof(sds));
    sds[0].fd = serverSocket;
    sds[0].events = POLLIN;
    while (true)
    {
        switch (poll(sds, numberOfSockets, POLL_TIMEOUT))
        {
        case 0:
            std::cout << "INFO: poll call timed out" << std::endl;
            break;

        case -1:
            std::cout << "ERROR: can not perform poll call" << std::endl;
            closeSocket(serverSocket);
            exit(-1);
        default:
            handleEvents(sds, numberOfSockets);
            break;
        }
    }
}

void handleEvents(struct pollfd sds[], int numberOfSockets)
{
    bool performCompression = false;
    for (int i = 0; i < numberOfSockets; i++)
    {
        if (sds[i].revents == 0)
        {
            continue;
        }
        // TODO: maybe we must close connections here :)
        if (sds[i].revents != POLLIN)
        {
            std::cout << "ERROR: something is happening with socket after poll" << std::endl;
            exit(-1);
        }
        if (i == 0)
        {
#ifdef __linux__
            int conn;
#elif _WIN32
            SOCKET conn;
#else
#endif

            if (numberOfSockets < MAX_CONNECTIONS + 1)
            {

                do
                {
                    conn = acceptConnection(sds[0].fd);
                    if (conn != -1)
                    {
                        sds[numberOfSockets].fd = conn;
                        sds[numberOfSockets].events = POLLIN;
                        numberOfSockets++;
                    }
                    else
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            std::cout << "ERROR: accept failed" << std::endl;
                            // TODO: close all sockets
                            exit(-1);
                        }
                    }

                } while (conn != -1);
            }
            else
            {
                // TODO:decline connection somehow
            }
        }
        else
        {
            bool connectionClosedByClient = false;
            std::vector<char> buffer = receiveData(sds[i].fd, connectionClosedByClient);
            if (connectionClosedByClient)
            {
                closeSocket(sds[i].fd);
                sds[i].fd = -1;
                performCompression = true;
            }
            std::string line(buffer.data());
            std::cout << "Obtained info: " << line << std::endl;
        }
    }
    if (performCompression)
    {
        for (int i = 0; i < numberOfSockets; i++)
        {
            if (sds[i].fd == -1)
            {
                for (int j = i; j < numberOfSockets; j++)
                {
                    sds[j].fd = sds[j + 1].fd;
                }
            }
            i--;
            numberOfSockets--;
        }
    }
}