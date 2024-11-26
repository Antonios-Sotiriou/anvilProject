#ifndef VEC_MATH_H
#define VEC_MATH_H 1

#if !defined(_INCLUDED_SMM) || !defined(_SMMINTRIN_H_INCLUDED)
    #include <smmintrin.h>
#endif // !_INCLUDED_SMM _SMMINTRIN_H_INCLUDED

#if !defined(_INC_MATH) || !defined(_MATH_H)
    #include <math.h>
#endif //!_INC_MATH _MATH_H

// Local created headers.
#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

vec4 setvec4(const float x, const float y, const float z, const float w);
float dotProduct(const vec4 v1, const vec4 v2);
vec4 crossProduct(const vec4 v1, const vec4 v2);
float vecLength(const vec4 v);
vec4 vecNormalize(const vec4 v);
vec4 vecMulf32(const vec4 v1, const float num);
vec4 vecMulvec(const vec4 v1, const vec4 v2);
vec4 vecDivf32(const vec4 v1, const float num);
vec4 vecDivvec(const vec4 v1, const vec4 v2);
vec4 vecAddf32(const vec4 v1, const float num);
vec4 vecAddvec(const vec4 v1, const vec4 v2);
vec4 vecSubf32(const vec4 v1, const float num);
vec4 vecSubvec(const vec4 v1, const vec4 v2);
const int checkAllZeros(const vec4 v);
const int vecEqualvec(const vec4 v1, const vec4 v2);
vec4 roundvec4(const vec4 v);
vec4 floorvec4(const vec4 v);

#endif // !VEC_MATH_H


