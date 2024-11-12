#ifndef OBJ_H
#define OBJ_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef _INC_STDLIB
    #include <stdlib.h>
#endif //_INC_STDLIB

#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef anvil_VERSION_MAJOR
    #include "headers/cmake_variables.h"
#endif // !anvil_VERSION_MAJOR

/* OBG file reading structs. */
typedef struct {
    float *v, *n, *t;
    int *f;
    int v_inx, n_inx, t_inx, f_inx;
} OBJ;

void readOBJ(OBJ *o, const char path[]);
void releaseOBJ(OBJ* o);

#endif // !OBJ_H