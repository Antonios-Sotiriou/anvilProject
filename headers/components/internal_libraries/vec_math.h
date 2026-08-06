#ifndef VEC_MATH_H
#define VEC_MATH_H 1

#include "headers/structs.h"

vec4 setvec4(const float x, const float y, const float z, const float w);
vec4 setvec4Zero(void);
void vec4SetX(vec4 *v, const float val);
void vec4SetY(vec4 *v, const float val);
void vec4SetZ(vec4 *v, const float val);
void vec4SetW(vec4 *v, const float val);
float vec4ExtractX(const vec4 v);
float vec4ExtractY(const vec4 v);
float vec4ExtractZ(const vec4 v);
float vec4ExtractW(const vec4 v);
float dotProduct(const vec4 v1, const vec4 v2);
vec4 crossProduct(const vec4 v1, const vec4 v2);
float vec4Length(const vec4 v);
vec4 vec4Normalize(const vec4 v);
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


