#ifndef RIGID_H
#define RIGID_H 1

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#if !defined(_INC_LIMITS) || !defined(_GCC_LIMITS_H_ )
    #include <limits.h>
#endif // !_INC_LIMITS !_GCC_LIMITS_H_ 

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

#ifndef TERRAIN_H
    #include "headers/components/terrain.h"
#endif // !TERRAIN_H

#ifndef COLLISIONS_H
    #include "headers/components/collisions.h"
#endif // !COLLISIONS_H

void loadmeshRigid(mesh *m, const char name[]);
void getmeshRigidLimits(mesh *m);
void releaseRigid(rigid *r);

#endif // !RIGID_H


