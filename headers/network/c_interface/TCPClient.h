#ifndef TCPCLIENT_H
#define TCPCLIENT_H 1

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCKAPI_
        #include <WinSock2.h>
        #ifndef _STDINT
            #include <stdint.h>
        #endif
    #endif // !_WINSOCK2API_

DWORD WINAPI startTCPClient(void* arg);
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <pthread.h>

void *startTCPClient(void* arg);
#endif

#endif // !TCPCLIENT_H


