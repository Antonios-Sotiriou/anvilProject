#ifndef TESTSHADER_H
#define TESTSHADER_H 1

#ifndef STDIO_H
    #include <stdio.h>
#endif

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

const int initTestShader(void);
void testShader(void);

#endif // !TESTSHADER_H


