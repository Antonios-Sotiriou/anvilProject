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

typedef struct meshPy {
    PyObject_HEAD
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    vec4 scale;                          // Vector to store the scale of the model.
    mat4x4 model_matrix;
    PyObject *cname;                     // The name to identify a mesh. Thats a dynamically size adoptaable null terminating string.
    float *vbo,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        outer_radius;                    // Value to hold the radius of the circle which surounding the mesh. aka( sqrtf(scale * scale) + (scale * scale)). Pythagorean Theorem.
    int length_cname,                    // Length of the cname char array. SOS !! (not included the NULL terminated char).
        vbo_indexes,                     // Number of vbo indexes as individual floats.
        faces_indexes,                   // Number of faces in vbo. ( vbo_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vbo. ( vbo_indexes / 8 or faces_indexes * 3).
        vbo_size,                        // The size of the vbo in bytes.( vbo_indexes * 4 ).
        pk,                              // Primary key of the mesh, representing its position in the database. That is also the mesh index in the SCENE meshes array.
        type,                            // The type of the mesh.
        visible,                         // Wether the mesh should be drawn on screen. Can be visible 1 to be drawn, or visible 0 not to.
        owns_anim;                       // Wether or not the mesh has an animation.
    GLuint VAO,                          // VAO id or name represented by an unsigned integer.
        VBO;                             // VBO id or name represented by an unsigned integer.
    rigid rigid;                         // Rigid body struct, which holds all usefull variables, for Physics and Collision Detection.
    struct meshPy **children;                 // The children mesh array of the mesh
    struct meshPy *parent;                    // The parent at which the mesh belongs.
    animation anim;
    int number_of_children,              // The number of the children that the mesh owns
        owns_rigid;                      // Wether or not the model has a rigid body attached to it.
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


