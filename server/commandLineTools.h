#pragma once

#define DATA_PROTOCOL_ARGUMENT "-p"
#define THREAD_ARGUMENT "-t"

#define TCP_PROTOCOL "t"
#define UDP_PROTOCOL "u"

#define SINGLE_THREAD_MODE "s"
#define MULTI_THREAD_MODE "m"

#define SINGLE_THREAD_SERVER 's'
#define MULTI_THREAD_SERVER 'm'

struct parameters
{
    // t - TCP, u - UDP
    char protocol;
    // s - single thread mode, m - multithread
    char threads;
};

struct parameters parseCommandLineParams(int argc, char *argv[]);

void displayErrorAndExit();