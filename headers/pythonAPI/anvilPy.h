#ifndef ANVILPY_H
#define ANVILPY_H 1

#ifndef Py_PYTHON_H
    #define PY_SSIZE_T_CLEAN
    #include <Python.h>
#endif // !Py_PYTHON_H

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#define DEBUG_LVL_1 1
#define debug_log_INFO()\
        do { if (DEBUG_LVL_1) fprintf(stderr, "CRITICAL:%s:%d:%s()\n", __FILE__, __LINE__, __func__); } while (0)

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #ifndef _WINSOCKAPI_
        #include <WinSock2.h>
        #ifndef _STDINT
            #include <stdint.h>
        #endif // !_SDTINT
    #endif // !_WINSOCK2API_
#else
    #include <unistd.h>
    #include <pthread.h>
#endif

typedef struct {
    PyObject_HEAD
    PyObject* cname;
    int number_of_children;
    PyObject **children;
    PyObject *parent;
} meshPy;

typedef struct {
    PyObject_HEAD
    PyObject *cname;
    int number_of_children;
    struct meshPy *parent;
} scenePy;

PyMODINIT_FUNC PyInit_meshPy(void);

void enablePythonAPI(void);
void disablePythonAPI(void);

#endif // !ANVILPY_H


