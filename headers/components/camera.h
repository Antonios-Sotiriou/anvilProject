#ifndef CAMERA_H
#define CAMERA_H 1

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

// Local created headers
#ifndef VEC_MATH_H
    #include "headers/components/internal_libraries/vec_math.h"
#endif // !VEC_MATH_H

#ifndef MATRICES_H
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

#ifndef QUATERNIONS_H
    #include "headers/components/internal_libraries/quaternions.h"
#endif // !QUATERNIONS_H

/* External Libraries. */
#ifndef _glfw3_h_
    #include "libraries/glfw-3.4/include/GLFW/glfw3.h"
#endif

void movementDispatch(model *m, const int key, const int action);

#endif // !CAMERA_H


