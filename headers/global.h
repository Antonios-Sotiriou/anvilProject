#ifndef GLOBAL_H
#define GLOBAL_H 1

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

#ifndef STRUCTS_H
    #include "structs.h"
#endif // !STRUCTS_H

/* Defined in main.c */
extern int WIDTH, HEIGHT, DISPLAY_RIGID;
/* The global matrices, defined also in main.c. */
extern mat4x4 LOOKAT_M, VIEW_M, PERSPECTIVE_M, PROJECTION_M;

/* defined in scene.c. */
extern scene SCENE;
extern int TERRAIN_INDEX;

/* defined in rasterization.c. */
extern GLint mainShaderProgram, displayShaderProgram, testShaderProgram, rigidShaderProgram;
extern GLuint mainFBO, shadowMapFBO;
extern const GLenum drawBuffers[2];

#endif // !GLOBAL_H


