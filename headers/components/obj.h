#ifndef OBJ_H
#define OBJ_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#if !defined(_INC_STDLIB) || !defined(_STDLIB_H)
    #include <stdlib.h>
#endif //!_INC_STDLIB _STDLIB_H

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef anvil_VERSION_MAJOR
    #include "headers/cmake_variables.h"
#endif // !anvil_VERSION_MAJOR

/* OBJ file reading structs. */
typedef struct {
    char *cname;          // The name of the mesh entry in the obj file.Usefull to identify meshes from blender.
    float *v,             // Floats array to store the vectors RAW data from an .obj file.
        *n,               // Floats array to store the notmals RAW data from an .obj file.
        *t;               // Floats array to store the texels RAW data from an .obj file.
    int *f,               // Integers array to store the faces RAW data from an .obj file.
        v_indexes,        // Number of items stored in the v array aka(The length of the array).
        n_indexes,        // Number of items stored in the n array aka(The length of the array). 
        t_indexes,        // Number of items stored in the t array aka(The length of the array). 
        f_indexes,        // Number of items stored in the f array aka(The length of the array).
        c_indexes;        // Number of items stored in the cname char array aka(The length of the array).
    int e_indexes;        // Number of items stored in the entries array aka(The length of the array).
} OBJ;

OBJ *readOBJ(const char path[]);
void releaseOBJ(OBJ *obj);

#endif // !OBJ_H


