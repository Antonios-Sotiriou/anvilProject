#include "headers/components/camera.h"

#define FORCE      500.3f    // Force to apply on velocity to move the object.
#define ANGLE      0.1f     // Angle by which to rotate the camera.

void movementDispatch(const int key, const int action) {

    switch (key) {

        case GLFW_KEY_W:
            SCENE.model[camera].velocity = vecMulf32(vecMulf32(SCENE.model[camera].coords.v[3], FORCE), action);
            break;
        case GLFW_KEY_A:
            SCENE.model[camera].rotate = action;
            SCENE.model[camera].rigid.q = rotationQuat(ANGLE, 0.f, 1.f, 0.f);
            break;
        case GLFW_KEY_S:
            SCENE.model[camera].velocity = vecMulf32(vecMulf32(SCENE.model[camera].coords.v[3], -FORCE), action);
            break;
        case GLFW_KEY_D:
            SCENE.model[camera].rotate = action;
            SCENE.model[camera].rigid.q = rotationQuat(-ANGLE, 0.f, 1.f, 0.f);
            break;
        case GLFW_KEY_Q:
            SCENE.model[camera].rotate = action;
            SCENE.model[camera].rigid.q = rotationQuat(ANGLE, vec4ExtractX(SCENE.model[camera].coords.v[1]), vec4ExtractY(SCENE.model[camera].coords.v[1]), vec4ExtractZ(SCENE.model[camera].coords.v[1]));
            break;
        case GLFW_KEY_E:
            SCENE.model[camera].rotate = action;
            SCENE.model[camera].rigid.q = rotationQuat(-ANGLE, vec4ExtractX(SCENE.model[camera].coords.v[1]), vec4ExtractY(SCENE.model[camera].coords.v[1]), vec4ExtractZ(SCENE.model[camera].coords.v[1]));
            break;
        case GLFW_KEY_RIGHT:
            SCENE.model[camera].velocity = vecMulf32(vecMulf32(SCENE.model[camera].coords.v[1], FORCE), action);
            break;
        case GLFW_KEY_LEFT:
            SCENE.model[camera].velocity = vecMulf32(vecMulf32(SCENE.model[camera].coords.v[1], -FORCE), action);
            break;
        case GLFW_KEY_UP:
            SCENE.model[camera].velocity = vecMulf32(vecMulf32(SCENE.model[camera].coords.v[2], FORCE), action);
            break;
        case GLFW_KEY_DOWN:
            SCENE.model[camera].velocity = vecMulf32(vecMulf32(SCENE.model[camera].coords.v[2], -FORCE), action);
            break;
        default:
            break;
    }
}


