#ifndef TESTSHADER_H
#define TESTSHADER_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef MATRICES_H
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

#ifndef QUATERNIONS_H
    #include "headers/components/internal_libraries/quaternions.h"
#endif // !QUATERNIONS_H

#ifndef RIGID_H
    #include "headers/components/rigid.h"
#endif // !RIGID_H

#ifndef RASTERIZATION_H
    #include "headers/components/rasterization.h"
#endif // !RASTERIZATION_H

/* Defined in openGL.c */
extern void glErrorReport(void);

const int initTestShader(void);
void testShader(scene *s);

#endif // !TESTSHADER_H


