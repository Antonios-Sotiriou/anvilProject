#ifndef _TESTSHADER_H
#define _TESTSHADER_H 1

#ifndef STDIO_H
    #include <stdio.h>
#endif

/* OpenGL headers. */
#if !define(__glew_h__) || !define(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Global variables. Defined in main.c. */
extern GLint testShaderProgram, width, height;
extern GLint mainFBO;

const int initTestShader(void);
const void testShader(void);

#endif // !_TESTSHADER_H

