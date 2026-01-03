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
    #include "headers/components/internal_libraries/obj.h"
#endif // !OBJ_H

#ifndef ANIMATIONS_H
    #include "headers/components/animations.h"
#endif // !ANIMATIONS_H

#ifndef MATRICES_H
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

void createModel(model *m);
void releaseModel(model *m);

#endif // !MODEL_H


