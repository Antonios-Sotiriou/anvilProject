#include "headers/components/camera.h"

#define FORCE      0.3f    // Force to apply on rigid body when its state is enabled.
#define SCALE_NR   100.f   // Scalar to scale the force of not enables rigid bodies.
static vec4 velocity;      // Velocity vector for when rigid body state is disabled. Probably better to move in function for better readabillity.

/* Moves camera forward along the N axis. */
void moveForward(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[3], FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[3], (FORCE * SCALE_NR)), action);
    SCENE.model[camera].mesh[0].coords.v[0] = vecAddvec(SCENE.model[camera].mesh[0].coords.v[0], velocity);
}
/* Moves camera Backward along the N axis. */
void moveBackward(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[3], -FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[3], (-FORCE * SCALE_NR)), action);
    SCENE.model[camera].mesh[0].coords.v[0] = vecAddvec(SCENE.model[camera].mesh[0].coords.v[0], velocity);
}
/* Moves camera right along the U axis. */
void moveRight(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[1], FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[1], (FORCE * SCALE_NR)), action);
    SCENE.model[camera].mesh[0].coords.v[0] = vecAddvec(SCENE.model[camera].mesh[0].coords.v[0], velocity);
}
/* Moves camera Left along the U axis. */
void moveLeft(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[1], -FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[1], (-FORCE * SCALE_NR)), action);
    SCENE.model[camera].mesh[0].coords.v[0] = vecAddvec(SCENE.model[camera].mesh[0].coords.v[0], velocity);
}
/* Moves camera Up along the V axis. */
void moveUp(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[2], FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[2], (FORCE * SCALE_NR)), action);
    SCENE.model[camera].mesh[0].coords.v[0] = vecAddvec(SCENE.model[camera].mesh[0].coords.v[0], velocity);
}
/* Moves camera Down along the V axis. */
void moveDown(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[2], -FORCE), action);
        return;
    }
    velocity = vecMulf32(vecMulf32(SCENE.model[camera].mesh[0].coords.v[2], (-FORCE * SCALE_NR)), action);
    SCENE.model[camera].mesh[0].coords.v[0] = vecAddvec(SCENE.model[camera].mesh[0].coords.v[0], velocity);
}
/* Rotates camera right at the V axis. */
void lookRight(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.rot_angle = action;
        SCENE.model[camera].mesh[0].rigid.q = rotationQuat(-0.01f, 0.f, 1.f, 0.f);
        return;
    }

    mat4x4 tr = matfromQuat(rotationQuat(-10.f * action, 0.f, 1.f, 0.f), SCENE.model[camera].mesh[0].coords.v[0]);
    setvec4arrayMulmat(SCENE.model[camera].mesh[0].coords.v, 4, tr);
}
/* Rotates camera Left at the V axis. */
void lookLeft(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.rot_angle = action;
        SCENE.model[camera].mesh[0].rigid.q = rotationQuat(0.01f, 0.f, 1.f, 0.f);
        return;
    }

    mat4x4 tr = matfromQuat(rotationQuat(10.f * action, 0.f, 1.f, 0.f), SCENE.model[camera].mesh[0].coords.v[0]);
    setvec4arrayMulmat(SCENE.model[camera].mesh[0].coords.v, 4, tr);
}
/* Rotates camera Up at the U axis. */
void lookUp(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.rot_angle = action;
        SCENE.model[camera].mesh[0].rigid.q = rotationQuat(0.01f, vec4ExtractX(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractY(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractZ(SCENE.model[camera].mesh[0].coords.v[1]));
        return;
    }

    mat4x4 tr = matfromQuat(rotationQuat(10.f * action, vec4ExtractX(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractY(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractZ(SCENE.model[camera].mesh[0].coords.v[1])), SCENE.model[camera].mesh[0].coords.v[0]);
    setvec4arrayMulmat(SCENE.model[camera].mesh[0].coords.v, 4, tr);
}
/* Rotates camera Down at the U axis. */
void lookDown(const int action) {
    if (SCENE.model[camera].mesh[0].rigid.state) {
        SCENE.model[camera].mesh[0].rigid.rot_angle = action;
        SCENE.model[camera].mesh[0].rigid.q = rotationQuat(-0.01f, vec4ExtractX(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractY(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractZ(SCENE.model[camera].mesh[0].coords.v[1]));
        return;
    }

    mat4x4 tr = matfromQuat(rotationQuat(-10.f * action, vec4ExtractX(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractY(SCENE.model[camera].mesh[0].coords.v[1]), vec4ExtractZ(SCENE.model[camera].mesh[0].coords.v[1])), SCENE.model[camera].mesh[0].coords.v[0]);
    setvec4arrayMulmat(SCENE.model[camera].mesh[0].coords.v, 4, tr);
}


