#ifndef VEC4PY_H
#define VEC4PY_H 1

#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef struct vec4Py {
    PyObject_HEAD
    PyObject *v;
} vec4Py;

PyMODINIT_FUNC PyInit_vec4Py(void);

#endif // !VEC4PY_H