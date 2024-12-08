#include "headers/components/rasterization.h"

/* Rasterizing Meshes with appropriate shaders. */
const void rasterize(void) {
    LOOKAT_M = lookatMatrix(SCENE.mesh[EYEPOINT].coords.v[0], SCENE.mesh[EYEPOINT].coords.v[1], SCENE.mesh[EYEPOINT].coords.v[2], SCENE.mesh[EYEPOINT].coords.v[3]);
    VIEW_M = inverseMatrix(LOOKAT_M);
    PROJECTION_M = matMulmat(VIEW_M, PERSPECTIVE_M);

    //mainShader();
    testShader();
    if (DISPLAY_RIGID)
        rigidShader();
    displayTexture(0);
}


