#ifndef COLLISIONS_H
#define COLLISIONS_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

//// Local created headers
//#ifndef TERRAIN_H
//#include "headers/components/terrain.h"
//#endif // !TERRAIN_H

// Local created headers
#ifndef VEC_MATH_H
    #include "headers/components/vec_math.h"
#endif // !VEC_MATH_H

#ifndef QUATERNIONS_H
    #include "headers/components/quaternions.h"
#endif // !QUATERNIONS_H

#ifndef MATRICES_H
    #include "headers/components/matrices.h"
#endif // !MATRICES_H

const int checkOBBCollision(mesh* m);

#endif // !COLLISIONS_H


