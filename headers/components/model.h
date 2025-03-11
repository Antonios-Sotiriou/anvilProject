#ifndef MODEL_H
#define MODEL_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef MESH_H
    #include "headers/components/mesh.h"
#endif // !MESH_H

#ifndef RIGID_H
    #include "headers/components/rigid.h"
#endif // !RIGID_H

#ifndef OBJ_H
    #include "headers/components/obj.h"
#endif // !OBJ_H

void createModel(model *m, const char name[]);
void releaseModel(model *m);

#endif // !MODEL_H


