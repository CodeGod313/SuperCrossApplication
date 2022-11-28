#include "includes.h"
#include "sockets.h"
#include "commandLineTools.h"
#include "serverLauncher.h"

int main(int argc, char *argv[])
{
    struct parameters params = parseCommandLineParams(argc, argv);
    launchServer(params);
    return 0;
}
