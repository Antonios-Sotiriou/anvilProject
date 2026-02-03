#ifndef GENERAL_H
#define GENERAL_H 1

/* Platform Specific libraries. */
/* Operating system checking. ######################################################### */
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINDOWS_
        #include <Windows.h>
    #endif // !_WINDOWS_
    #ifndef _INC_TIME
        #include <time.h>
    #endif // !_INC_TIME
#elif defined(__APPLE__) || defined(__MACH__)
#elif defined(LINUX) || defined(__linux__)
    #ifndef _INC_TIME
            #include <sys/time.h>
    #endif // !_INC_TIME
#else
#endif

#ifndef _STDINT
    #include <stdint.h>
#endif // !_STDINT

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void swap(void* a, void* b, unsigned long size);
const float radians(const float value);
void getvec4ArrayLimits(vec4 v[], const int array_len, vec4 *min, vec4 *max);
void usleep(const int usec);
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) 
    void gettimeofday(struct timeval* tv, struct timezone* tzp);
#endif
void initTimeCounter(metrics* mtr);
void updateTimeCounter(metrics* mtr);
void calculateFPS(metrics* mtr);

#endif // !GENERAL_H


