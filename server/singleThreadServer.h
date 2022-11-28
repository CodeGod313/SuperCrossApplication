#pragma once

#include "sockets.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 25565

// timeout in milliseconds
#define POLL_TIMEOUT 3000

void launchSingleThreadServer(char protocol);


void handleEvents(struct pollfd sds[], int numberOfSockets);