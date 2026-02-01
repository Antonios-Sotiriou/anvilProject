#include "headers/components/camera.h"

#define FORCE      500.3f    // Force to apply on velocity to move the object.
#define ANGLE      0.1f     // Angle by which to rotate the camera.

void movementDispatch(model *m, const int key, const int action) {

    switch (key) {

        case GLFW_KEY_W:
            m->velocity = vecMulf32(vecMulf32(m->coords.v[3], FORCE), action);
            break;
        case GLFW_KEY_A:
            m->rotate = action;
            m->rigid.q = rotationQuat(ANGLE, 0.f, 1.f, 0.f);
            break;
        case GLFW_KEY_S:
            m->velocity = vecMulf32(vecMulf32(m->coords.v[3], -FORCE), action);
            break;
        case GLFW_KEY_D:
            m->rotate = action;
            m->rigid.q = rotationQuat(-ANGLE, 0.f, 1.f, 0.f);
            break;
        case GLFW_KEY_Q:
            m->rotate = action;
            m->rigid.q = rotationQuat(ANGLE, vec4ExtractX(m->coords.v[1]), vec4ExtractY(m->coords.v[1]), vec4ExtractZ(m->coords.v[1]));
            break;
        case GLFW_KEY_E:
            m->rotate = action;
            m->rigid.q = rotationQuat(-ANGLE, vec4ExtractX(m->coords.v[1]), vec4ExtractY(m->coords.v[1]), vec4ExtractZ(m->coords.v[1]));
            break;
        case GLFW_KEY_RIGHT:
            m->velocity = vecMulf32(vecMulf32(m->coords.v[1], FORCE), action);
            break;
        case GLFW_KEY_LEFT:
            m->velocity = vecMulf32(vecMulf32(m->coords.v[1], -FORCE), action);
            break;
        case GLFW_KEY_UP:
            m->velocity = vecMulf32(vecMulf32(m->coords.v[2], FORCE), action);
            break;
        case GLFW_KEY_DOWN:
            m->velocity = vecMulf32(vecMulf32(m->coords.v[2], -FORCE), action);
            break;
        default:
            break;
    }
}


