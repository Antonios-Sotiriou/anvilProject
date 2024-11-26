#ifndef DISPLAYSHADER_H
#define DISPLAYSHADER_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

/* Defined in openGL.c */
extern void glErrorReport(void);

const int initDisplayShader(void);
void displayTexture(const int textureIndex);

#endif // !DISPLAYSHADER_H


