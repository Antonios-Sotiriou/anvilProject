#ifndef RASTERIZATION_H
#define RASTERIZATION_H 1

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Shaders. */
#include "headers/shaders/mainShader.h"
#include "headers/shaders/displayShader.h"
#include "headers/shaders/testShader.h"

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

const void initRasterComponents(void);
const void rasterize();

#endif // !RASTERIZATION_H


