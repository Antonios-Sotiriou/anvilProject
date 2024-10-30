#ifndef MESH_H
#define MESH_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void createMesh(mesh *m, const char type[]);
void releaseMesh(mesh *m);

#endif // !MESH_H


