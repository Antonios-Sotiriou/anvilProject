#ifndef GLOBAL_H
#define GLOBAL_H 1

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

#ifndef STRUCTS_H
    #include "structs.h"
#endif // !STRUCTS_H

/* defined in rasterization.c. */
extern GLint mainShaderProgram, displayShaderProgram, testShaderProgram, rigidShaderProgram, shadowShaderProgram;

#endif // !GLOBAL_H


