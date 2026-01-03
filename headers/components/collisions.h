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
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

#ifndef QUATERNIONS_H
    #include "headers/components/internal_libraries/quaternions.h"
#endif // !QUATERNIONS_H

void modelTerrainCollision(model *m);
const int staticOuterRadiusCollision(model *m);
const int sweptAABBCollision(model *m, const int pks[]);
const int staticOBBCollision(model *m, const int pk);
void sortCollisions(model *m);

#endif // !COLLISIONS_H


