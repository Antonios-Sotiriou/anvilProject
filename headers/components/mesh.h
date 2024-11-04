#ifndef MESH_H
#define MESH_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void createMesh(mesh *m, const char type[]);
void releaseMesh(mesh *m);

#endif // !MESH_H


