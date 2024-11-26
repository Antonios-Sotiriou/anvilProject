#ifndef CAMERA_H
#define CAMERA_H 1

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

// Local created headers
#ifndef VEC_MATH_H
    #include "headers/components/vec_math.h"
#endif // !VEC_MATH_H

#ifndef MATRICES_H
    #include "headers/components/matrices.h"
#endif // !MATRICES_H

#ifndef QUATERNIONS_H
    #include "headers/components/quaternions.h"
#endif // !QUATERNIONS_H

void moveForward(const int action);
void moveBackward(const int action);
void moveRight(const int action);
void moveLeft(const int action);
void moveUp(const int action);
void moveDown(const int action);
void lookRight(const int action);
void lookLeft(const int action);

#endif // !CAMERA_H


