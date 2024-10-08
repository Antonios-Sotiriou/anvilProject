#ifndef _DISPLAYSHADER_H
#define _DISPLAYSHADER_H 1

#ifndef STDIO_H
    #include <stdio.h>
#endif

/* OpenGL headers. */
#if !define(__glew_h__) || !define(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Global variables. Defined in main.c. */
extern int displayShaderProgram, width, height;

const int initDisplayShader(void);
const void displayTexture(const int textureIndex);

#endif /* _DISPLAYSHADER_H */


