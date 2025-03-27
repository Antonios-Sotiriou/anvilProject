#ifndef TCPSERVER_H
#define TCPSERVER_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#ifndef _INC_STDLIB
    #include <stdlib.h>
#endif

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCKAPI_
        #include <WinSock2.h>
    #endif // !_WINSOCK2API_

DWORD WINAPI startTCPServer(void* arg);
#else
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <pthread.h>

void *startTCPServer(void* arg);
#endif

void enableNetworkInterface(void);
void disableNetworkInterface(void);

#endif // !TCPSERVER_H


