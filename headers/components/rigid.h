#ifndef RIGID_H
#define RIGID_H 1

#ifndef _INC_STRING
    #include <string.h>
#endif // !_INC_STRING

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef OBJ_H
    #include "headers/components/obj.h"
#endif // !OBJ_H

#ifndef QUATERNIONS_H
    #include "headers/components/quaternions.h"
#endif // !QUATERNIONS_H

#ifndef MATRICES_H
    #include "headers/components/matrices.h"
#endif // !MATRICES_H

void loadmeshRigid(mesh* m, const char name[]);
void getmeshRigidLimits(mesh* m);
void releasemeshRigid(mesh* m);

#endif // !RIGID_H


