#ifndef OPENGL_H
#define OPENGL_H 1

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

/* Shaders. */
#ifndef MAINSHADER_H
    #include "headers/shaders/mainShader.h"
#endif // !MAINSHADER_H

#ifndef DISPLAYSHADER_H
    #include "headers/shaders/displayShader.h"
#endif // !DISPLAYSHADER_H

#ifndef TESTSHADER_H
    #include "headers/shaders/testShader.h"
#endif // !TESTSHADER_H

#ifndef RIGIDSHADER_H
    #include "headers/shaders/rigidShader.h"
#endif // !RIGIDSHADER_H

const int initOpenGLComponents(void);
void createMeshVAO(mesh* m);
void releaseOpenGLComponents(void);
void glErrorReport(void);

#endif // !OPENGL_H


