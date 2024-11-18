#include "headers/components/rasterization.h"

/* Rasterizing Meshes with appropriate shaders. */
const void rasterize(void) {
    //project();
    testShader();
    if (DISPLAY_RIGID)
        rigidShader();
    displayTexture(0);
}


