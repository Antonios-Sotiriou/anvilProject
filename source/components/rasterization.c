#include "headers/components/rasterization.h"

/* Rasterizing Meshes with appropriate shaders. */
const void rasterize(void) {
    LOOKAT_M = lookatMatrix((vec4){ 0.0, 0.0, -1000.0, 1.0 }, (vec4){ 1.0, 0.0, 0.0, 0.0 }, (vec4){ 0.0, 1.0, 0.0, 0.0 }, (vec4){ 0.0, 0.0, 1.0, 0.0 });
    VIEW_M = inverseMatrix(LOOKAT_M);
    PROJECTION_M = matMulmat(VIEW_M, PERSPECTIVE_M);

    //mainShader();
    testShader();
    if (DISPLAY_RIGID)
        rigidShader();
    displayTexture(0);
}


