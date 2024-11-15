#ifndef COLLISIONS_H
#define COLLISIONS_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef MATRICES_H
    #include "headers/components/matrices.h"
#endif // !MATRICES_H

const int checkOBBCollision(mesh* m);

#endif // !COLLISIONS_H


