#ifndef _DISPLAYSHADER_H
#define _DISPLAYSHADER_H 1

#ifndef STDIO_H
    #include <stdio.h>
#endif

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Global variables. Defined in main.c. */
extern GLint displayShaderProgram, width, height;

const int initDisplayShader(void);
void displayTexture(const int textureIndex);

#endif /* _DISPLAYSHADER_H */


