#ifndef REQUESTS_H
#define REQUESTS_H 1

#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCKAPI_
        #include <WinSock2.h>
    #endif // !_WINSOCK2API_
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <pthread.h>
#endif

int sendRequest(const char ip_address[], const uint16_t port, const char req[]);

#endif // !REQUESTS_H


