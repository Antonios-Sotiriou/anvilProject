#ifndef PHYSICS_H
#define PHYSICS_H 1

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

// Local created headers
#ifndef TERRAIN_H
    #include "headers/components/terrain.h"
#endif // !TERRAIN_H

// Local created headers
#ifndef VEC_MATH_H
    #include "headers/components/internal_libraries/vec_math.h"
#endif // !VEC_MATH_H

#ifndef QUATERNIONS_H
    #include "headers/components/internal_libraries/quaternions.h"
#endif // !QUATERNIONS_H

#ifndef MATRICES_H
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

#ifndef COLLISIONS_H
    #include "headers/components/collisions.h"
#endif // !COLLISIONS_H

void applyPhysics(scene *s);

#endif // !PHYSICS_H


