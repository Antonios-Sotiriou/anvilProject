#ifndef COLLISIONS_H
#define COLLISIONS_H 1

#if !defined(_INC_LIMITS) || !defined(_GCC_LIMITS_H_ )
    #include <limits.h>
#endif // !_INC_LIMITS !_GCC_LIMITS_H_ 

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef RIGID_H
    #include "headers/components/rigid.h"
#endif // !RIGID_H

#ifndef MATRICES_H
    #include "headers/components/matrices.h"
#endif // !MATRICES_H

#ifndef QUATERNIONS_H
    #include "headers/components/quaternions.h"
#endif // !QUATERNIONS_H

void meshTerrainCollision(mesh *m);
const int staticOuterRadiusCollision(mesh *m);
const int sweptAABBCollision(mesh *m, const int pks[]);
const int staticOBBCollision(mesh *m, const int pk);
void sortCollisions(mesh* m);

#endif // !COLLISIONS_H


