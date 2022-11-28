#include "package.h"

std::vector<std::vector<char>> composePackages(std::vector<char> message, int packageSize)
{
    std::vector<std::vector<char>> packages;
    int messagePointer = 0;
    while (messagePointer != message.size())
    {
        std::vector<char> package;
        while (messagePointer != message.size() && package.size() != packageSize)
        {
            package.push_back(message[messagePointer++]);
        }
        completePackage(package, packageSize);
        packages.push_back(package);
    }
    return packages;
}

void completePackage(std::vector<char> &package, int packageSize)
{
    while (package.size() != packageSize)
    {
        package.push_back('\0');
    }
}