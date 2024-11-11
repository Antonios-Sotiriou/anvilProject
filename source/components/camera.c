#include "headers/components/camera.h"

#define FORCE      0.01f    // Force to apply on rigid body when its state is enabled.
#define SCALE_NR   100.f   // Scalar to scale the force of not enables rigid bodies.
static vec4 velocity;      // Velocity vector for when rigid body state is disabled. Probably better to move in function for better readabillity.

/* Moves camera forward along the N axis. */
void moveForward(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[3], FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[3], (FORCE * SCALE_NR)), action);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
}
/* Moves camera Backward along the N axis. */
void moveBackward(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[3], -FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[3], (-FORCE * SCALE_NR)), action);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
}
/* Moves camera right along the U axis. */
void moveRight(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[1], FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[1], (FORCE * SCALE_NR)), action);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
}
/* Moves camera Left along the U axis. */
void moveLeft(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[1], -FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[1], (-FORCE * SCALE_NR)), action);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
}
/* Moves camera Up along the V axis. */
void moveUp(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[2], FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[2], (FORCE * SCALE_NR)), action);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
}
/* Moves camera Down along the V axis. */
void moveDown(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[2], -FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.mesh[camera].coords.v[2], (-FORCE * SCALE_NR)), action);
    SCENE.mesh[camera].coords.v[0] = vecAddvec(SCENE.mesh[camera].coords.v[0], velocity);
}
/* Rotates camera right at the V axis. */
void lookRight(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.rot_angle = action;
        SCENE.mesh[camera].rigid.q = rotationQuat(-0.01f, 0.f, 1.f, 0.f);
            return;
    }

    mat4x4 tr = matfromQuat(rotationQuat(-10.f * action, 0.f, 1.f, 0.f), SCENE.mesh[camera].coords.v[0]);
    setvec4arrayMulmat(SCENE.mesh[camera].coords.v, 4, tr);
}
/* Rotates camera Left at the V axis. */
void lookLeft(const int action) {
    if (SCENE.mesh[camera].rigid.state) {
        SCENE.mesh[camera].rigid.rot_angle = action;
        SCENE.mesh[camera].rigid.q = rotationQuat(0.01f, 0.f, 1.f, 0.f);
        return;
    }

    mat4x4 tr = matfromQuat(rotationQuat(10.f * action, 0.f, 1.f, 0.f), SCENE.mesh[camera].coords.v[0]);
    setvec4arrayMulmat(SCENE.mesh[camera].coords.v, 4, tr);
}


