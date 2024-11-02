#ifndef QUATERNIONS_H
#define QUATERNIONS_H 1

//#include <string.h>

#ifndef INC_MATH
    #include <math.h>
#endif // !INC_MATH

// Local created headers.
#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifdef VECTORIZED_CODE // #######################################################################################
/* Quaternion's internal format is W X Y Z. */
typedef union __m128 quat;
#else // ITERATIVE_CODE #########################################################################################
/* Quaternion's internal format is W X Y Z. */
typedef vec4 quat;
#endif // VECTORIZED_CODE #######################################################################################

quat unitQuat(void);
quat setQuat(const float angle, const float x, const float y, const float z);
float magnitudeQuat(const quat q);
void normalizeQuat(quat* q);
quat conjugateQuat(const quat q);
quat rotationQuat(const float angle, const float x, const float y, const float z);
vec4 vec4RotateQuat(const quat q, const vec4 v);
void setvec4RotateQuat(const quat q, vec4* v);
quat addQuats(const quat q1, const quat q2);
quat eulertoQuat(const float roll, const float yaw, const float pitch);
quat multiplyQuats(const quat q1, const quat q2);
mat4x4 matfromQuat(const quat q, const vec4 t);
mat4x4 modelMatfromQST(const quat q, const float s, const vec4 t);
quat slerp(const quat q1, const quat q2, const float t);
quat lerp(const quat q1, const quat q2, const float t);

#endif // !QUATERNIONS_H


