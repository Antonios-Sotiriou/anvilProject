#ifndef QUATERNIONS_H
#define QUATERNIONS_H 1

#include "headers/structs.h"

#ifdef VECTORIZED_CODE // #######################################################################################
    #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        /* Quaternion's internal format is W X Y Z. */
        typedef union __m128 quat;
    #elif defined(LINUX) || defined(__linux__)
        /* Quaternion's internal format is W X Y Z. */
        typedef __m128 quat;
    #endif
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
quat eulerToQuat(const float roll, const float yaw, const float pitch);
quat multiplyQuats(const quat q1, const quat q2);
mat4x4 matFromQuat(const quat q, const vec4 t);
mat4x4 modelMatFromQST(const quat q, const vec4 s, const vec4 t);
quat quatFromMat(mat4x4 m);
quat slerp(const quat q1, const quat q2, const float t);
quat lerp(const quat q1, const quat q2, const float t);

#endif // !QUATERNIONS_H


