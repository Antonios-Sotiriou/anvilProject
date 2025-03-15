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

/* External Libraries. */
#ifndef _glfw3_h_
    #include "libraries/glfw-3.4/include/GLFW/glfw3.h"
#endif

movementDispatch(const int key, const int action);

#endif // !CAMERA_H


