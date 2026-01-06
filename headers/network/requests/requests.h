#ifndef REQUESTS_H
#define REQUESTS_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#ifndef _INC_STDLIB
    #include <stdlib.h>
#endif

#ifndef _STDINT
    #include <stdint.h>
#endif // !_STDINT

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCKAPI_
        #include <WinSock2.h>
        #ifndef _STDINT
            #include <stdint.h>
        #endif
    #endif // !_WINSOCK2API_
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <pthread.h>
#endif

int sendRequest(const char ip_address[], const uint16_t port, const char req[]);

#endif // !REQUESTS_H


