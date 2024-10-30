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

void moveForward(const int action);
void moveBackward(const int action);

#endif // !CAMERA_H