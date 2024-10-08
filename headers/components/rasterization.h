#ifndef _RASTERIZATION_H
#define _RASTERIZATION_H 1

/* OpenGL headers. */
#if !define(__glew_h__) || !define(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Shaders. */
#include "headers/shaders/mainShader.h"
#include "headers/shaders/displayShader.h"
#include "headers/shaders/testShader.h"

/* Global variables. Defined in main.c. */
extern int Debug;

const void initRasterComponents(void);
const void rasterize();

#endif /* _RASTERIZATION_H */


