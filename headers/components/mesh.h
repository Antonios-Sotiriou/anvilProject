#ifndef MESH_H
#define MESH_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef OBJ_H
    #include "headers/components/internal_libraries/obj.h"
#endif // !OBJ_H

#ifndef OPENGL_H
    #include "headers/components/openGL.h"
#endif // !OPENGL_H

#ifndef RIGID_H
    #include "headers/components/rigid.h"
#endif // !RIGID_H

#ifndef ANIMATIONS_H
    #include "headers/components/animations.h"
#endif // !ANIMATIONS_H

void createMesh(mesh *m, ENTRY obj);
void releaseMesh(mesh *m);

#endif // !MESH_H


