#ifndef COLLISIONS_H
#define COLLISIONS_H 1

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

const int outerRadiusCollision(mesh* m);
const int checkAABBCollision(mesh* m, const int pks[]);
const int checkOBBCollision(mesh* m);

#endif // !COLLISIONS_H


