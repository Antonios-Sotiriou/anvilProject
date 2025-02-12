#ifndef MATRICES_H
#define MATRICES_H 1

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

#ifndef VEC_MATH_H
    #include "headers/components/vec_math.h"
#endif // !VEC_MATH_H

mat4x4 identityMatrix(void);
mat4x4 scaleMatrix(const float scale);
mat4x4 translationMatrix(const float x, const float y, const float z);
mat4x4 rotateXMatrix(const float angle);
mat4x4 rotateYMatrix(const float angle);
mat4x4 rotateZMatrix(const float angle);
mat4x4 orthographicMatrix(const float l, const float r, const float t, const float b, const float n, const float f);
mat4x4 perspectiveMatrix(const float fov, const float ar, const float zn, const float zf);
mat4x4 reperspectiveMatrix(const float fov, const float ar);
mat4x4 lookatMatrix(const vec4 P, const vec4 U, const vec4 V, const vec4 N);
mat4x4 pointatMatrix(const vec4 P, const vec4 T, const vec4 Up);
vec4 vec4Mulmat(const vec4 v, const mat4x4 m);
void setvec4Mulmat(vec4* v, const mat4x4 m);
vec4* vec4arrayMulmat(vec4 vecs[], const int len, const mat4x4 m);
void setvec4arrayMulmat(vec4 vecs[], const int len, const mat4x4 m);
face* facearrayMulmat(face f[], const int len, const mat4x4 m);
void setfacearrayMulmat(face f[], const int len, const mat4x4 m);
mat4x4 matMulmat(const mat4x4 m1, const mat4x4 m2);
mat4x4 transposeMatrix(const mat4x4 m);
mat4x4 inverseMatrix(const mat4x4 m);

#endif // !MATRICES_H


