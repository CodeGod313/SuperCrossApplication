#include "includes.h"
#include "sockets.h"
#include "commandLineTools.h"
#include "serverLauncher.h"

std::vector<char> lineToChars(std::string line)
{
    std::vector<char> data;
    for (int i = 0; i < line.size(); i++)
    {
        data.push_back(line[i]);
    }
    return data;
}

int main(int argc, char *argv[])
{
    int s = createSocket('t');
    connectToServer(s, "127.0.0.1", 25565);
    std::vector<char> data;
    std::string line;
    std::getline(std::cin, line);

    sendData(s, lineToChars(line));
    return 0;
}
