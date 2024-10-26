#ifndef VEC_MATH_H
#define VEC_MATH_H 1

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

#endif // !VEC_MATH_H


