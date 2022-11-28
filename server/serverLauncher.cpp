#include "serverLauncher.h"

void launchServer(struct parameters params)
{
    switch (params.threads)
    {
    case SINGLE_THREAD_SERVER:
        launchSingleThreadServer(params.protocol);
        break;
    case MULTI_THREAD_SERVER:
        launchMultiThreadServer(params.protocol);
        break;
    default:
        std::cout << "ERROR: confusing situation (i don't know which server to launch)" << std::endl;
        break;
    }
}