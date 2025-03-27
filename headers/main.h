#ifndef MAIN_H
#define MAIN_H 1

/* Platform Specific libraries. */
/* Operating system checking. ######################################################### */
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #define  CLIENT_OS  "Windows Operating System!"
    #include <Windows.h>
    #include <time.h>
#elif defined(__APPLE__) || defined(__MACH__)
    #define  CLIENT_OS  "MacOs Operating System!"
#elif defined(LINUX) || defined(__linux__)
    #define  CLIENT_OS  "Linux Operating System!"
    #include <sys/time.h>
#else
    #define  CLIENT_OS  "Unkown Operating System!"
#endif

/* Cmake variables */
#include "cmake_variables.h"

/* Build in Libraries. */
#include <stdio.h>
#include <xmmintrin.h>
#include <limits.h>

/* Internal Libraries. */
#include "headers/flags.h"
#include "headers/components/openGL.h"
#include "headers/components/scene.h"
#include "headers/components/rasterization.h"
#include "headers/components/camera.h"
#include "headers/components/physics.h"

#if (NETWORK_ENABLED)
    #ifndef TCPSERVER_H
        #include "headers/network/TCPServer.h"
    #endif // !TCPSERVER_H
#endif // 1(NETWORK_INTERFACE)


/* External Libraries. */
#ifndef _glfw3_h_
    #include "libraries/glfw-3.4/include/GLFW/glfw3.h"
#endif

/* General Macro definitions */
#define _GNU_SOURCE /* Importand to counter sigaction struct < incomplete type error >. */

#endif // !MAIN_H


