#ifndef OPENGL_H
#define OPENGL_H 1

#include "headers/structs.h"

void initOpenGLComponents(void);
void createSceneFrameBuffers(scene *s);
void releaseSceneFrameBuffers(scene *s);

// For Debugging purposes.That way we make errors more user friendly.
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);

#endif // !OPENGL_H


