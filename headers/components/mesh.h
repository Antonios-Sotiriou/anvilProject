#ifndef MESH_H
#define MESH_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef _INC_STRING
    #include <string.h>
#endif // !_INC_STRING

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // STRUCTS_H

#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // GLOBAL_H

void createMesh(mesh *m, const char type[]);
void releaseMesh(mesh *m);

#endif // MESH_H


