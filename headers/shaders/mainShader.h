#ifndef _MAINSHADER_H
#define _MAINSHADER_H 1

#ifndef STDIO_H
    #include <stdio.h>
#endif

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Global variables. Defined in main.c. */
extern GLint mainShaderProgram, width, height;
extern GLint mainFBO;
extern GLenum drawBuffers[2];

const int initMainShader(void);
void project(void);

#endif /* _MAINSHADER_H */


