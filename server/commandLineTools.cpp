#include "commandLineTools.h"
#include "includes.h"

// TODO: add help
struct parameters parseCommandLineParams(int argc, char *argv[])
{
    struct parameters params = {'t', 's'};
    int i = 1;
    bool isValid = false;
    while (i < argc)
    {
        isValid = false;
        if (!strcmp(argv[i], DATA_PROTOCOL_ARGUMENT))
        {
            i++;
            if (i < argc && (!strcmp(argv[i], TCP_PROTOCOL) || !strcmp(argv[i], UDP_PROTOCOL)))
            {
                params.protocol = argv[i][0];
                i++;
                isValid = true;
                continue;
            }
            else
            {
                displayErrorAndExit();
            }
        }
        if (!strcmp(argv[i], THREAD_ARGUMENT))
        {
            i++;
            if (i < argc &&
                (!strcmp(argv[i], SINGLE_THREAD_MODE) || !strcmp(argv[i], MULTI_THREAD_MODE)))
            {
                params.protocol = argv[i][0];
                i++;
                isValid = true;
                continue;
            }
            else
            {
                displayErrorAndExit();
            }
        }
        if (!isValid)
        {
            displayErrorAndExit();
        }
    }
    return params;
}

void displayErrorAndExit()
{
    std::cout << "ERROR: can not parse command line arguments" << std::endl;
    exit(-1);
}