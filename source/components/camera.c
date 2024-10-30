#include "headers/components/camera.h"

#define FORCE 10.f
static vec4 velocity;

/* Moves camera forward along the N axis. */
void moveForward(const int action) {
    int enable = action > 0 ? 1 : 0;
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[3], FORCE), enable);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
    printf("action: %d\n", action);
}
void moveBackward(const int action) {
    int enable = action > 0 ? 1 : 0;
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[3], FORCE), enable);
    SCENE.mesh[camera].coords.v[0] = vecSubvec(SCENE.mesh[camera].coords.v[0], velocity);
}


