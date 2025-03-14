#include "headers/components/rasterization.h"

/* Rasterizing Meshes with appropriate shaders. */
const void rasterize(void) {
    LOOKAT_M = lookatMatrix(
        SCENE.model[EYEPOINT].mesh[0].coords.v[0],
        SCENE.model[EYEPOINT].mesh[0].coords.v[1],
        SCENE.model[EYEPOINT].mesh[0].coords.v[2],
        SCENE.model[EYEPOINT].mesh[0].coords.v[3]
    );
    VIEW_M = inverseMatrix(LOOKAT_M);
    PROJECTION_M = matMulmat(VIEW_M, PERSPECTIVE_M);

    //mainShader();
    testShader();
    if (DISPLAY_RIGID)
        rigidShader();
    displayTexture(0);
}


