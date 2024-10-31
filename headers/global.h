#ifndef GLOBAL_H
#define GLOBAL_H 1

#ifndef STRUCTS_H
    #include "structs.h"
#endif // !STRUCTS_H

/* Defined in main.c */
extern int WIDTH, HEIGHT, DEBUG;
/* The global matrices, defined also in main.c. */
extern mat4x4 LOOKAT_M, VIEW_M, PERSPECTIVE_M, PROJECTION_M;

/* defined in scene.c */
extern scene SCENE;

#endif // !GLOBAL_H


