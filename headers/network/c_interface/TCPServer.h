#ifndef TCPSERVER_H
#define TCPSERVER_H 1

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCKAPI_
        #include <WinSock2.h>
    #endif // !_WINSOCK2API_

DWORD WINAPI startTCPServer(void* arg);
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <pthread.h>

void *startTCPServer(void* arg);
#endif

void enableNetworkInterface(void);
void disableNetworkInterface(void);

#endif // !TCPSERVER_H


