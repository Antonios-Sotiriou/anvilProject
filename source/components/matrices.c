#include "headers/components/matrices.h"

#ifdef VECTORIZED_CODE // #######################################################################################
/* Usefull to inverse some values in inverseMatrix function. */
const static vec4 invmor = { -0.f, -0.f, -0.f, 0.f };

/* Scale Matrix. */
mat4x4 scaleMatrix(const float scale) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = scale;
    m.m[1].m128_f32[1] = scale;
    m.m[2].m128_f32[2] = scale;
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Translation Matrix. */
mat4x4 translationMatrix(const float x, const float y, const float z) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = 1.0f;
    m.m[1].m128_f32[1] = 1.0f;
    m.m[2].m128_f32[2] = 1.0f;
    m.m[3].m128_f32[3] = 1.0f;
    m.m[3].m128_f32[0] = x;
    m.m[3].m128_f32[1] = y;
    m.m[3].m128_f32[2] = z;
    return m;
}
/* Rotate Object on X axis according the world origin */
mat4x4 rotateXMatrix(const float angle) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = 1.0f;
    m.m[1].m128_f32[1] = cosf(angle);
    m.m[1].m128_f32[2] = -sinf(angle);
    m.m[2].m128_f32[1] = sinf(angle);
    m.m[2].m128_f32[2] = cosf(angle);
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Rotate Object on Y axis according the world origin. */
mat4x4 rotateYMatrix(const float angle) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = cosf(angle);
    m.m[1].m128_f32[1] = 1.0f;
    m.m[0].m128_f32[2] = -sinf(angle);
    m.m[2].m128_f32[0] = sinf(angle);
    m.m[2].m128_f32[2] = cosf(angle);
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Rotate Object on Z axis according the world origin */
mat4x4 rotateZMatrix(const float angle) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = cosf(angle);
    m.m[0].m128_f32[1] = sinf(angle);
    m.m[1].m128_f32[0] = -sinf(angle);
    m.m[1].m128_f32[1] = cosf(angle);
    m.m[2].m128_f32[2] = 1.0f;
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Orthographic Projection Matrix. l: left, r: right, t: top, b: bottom, n: near, f: far.*/
mat4x4 orthographicMatrix(const float l, const float r, const float t, const float b, const float n, const float f) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = 2.f / (r - l);
    m.m[1].m128_f32[1] = 2.f / (b - t);
    m.m[2].m128_f32[2] = (1.0f / (n - f));
    m.m[3].m128_f32[0] = ((r + l) / (r - l));
    m.m[3].m128_f32[1] = ((b + t) / (b - t));
    m.m[3].m128_f32[2] = ((f + n) / (n - f));
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Perspective Projection Matrix. */
mat4x4 perspectiveMatrix(const float fov, const float aspectratio, const float zn, const float zf) {
    mat4x4 m = { 0 };
    float fovRadius = 1.f / tanf(fov * 0.5f / 180.0f * 3.14159f);
    m.m[0].m128_f32[0] = fovRadius;
    m.m[1].m128_f32[1] = aspectratio * fovRadius;
    m.m[2].m128_f32[2] = zf / (zf - zn);
    m.m[2].m128_f32[3] = 1.0f;
    m.m[3].m128_f32[2] = (zf * zn) / (zn - zf);
    return m;
}
/* Reverse Perspective Projection Matrix. fov: field of view, ar: aspect ratio. */
mat4x4 reperspectiveMatrix(const float fov, const float ar) {
    mat4x4 m = { 0 };
    const float fovRadius = 1.f / tanf(fov * 0.5f / 180.0f * 3.14159f);
    m.m[0].m128_f32[0] = ar / fovRadius;
    m.m[1].m128_f32[1] = ar / fovRadius;
    m.m[3].m128_f32[2] = 1.0f;
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* The Camera Matrix or as used to called the View Matrix.Returns a new 4x4 Matrix. */
mat4x4 lookatMatrix(const vec4 P, const vec4 U, const vec4 V, const vec4 N) {
    return (mat4x4) {
        U,
        V,
        N,
        P
    };
}
/* The Point at Matrix.Takes a position P, a target point vector T, and an up vector Up and returns a matrix to point at location T. */
mat4x4 pointatMatrix(const vec4 P, const vec4 T, const vec4 Up) {
    const vec4 N = vecNormalize(_mm_sub_ps(P, T));
    const vec4 U = vecNormalize(crossProduct(Up, N));
    const vec4 V = crossProduct(N, U);

    return (mat4x4) {
        U,
        V,
        N,
        P
    };
}
/* Multiplies a vec4 with the given Matrix and returns a new vec4, leaving the original unmodified. */
vec4 vec4Mulmat(const vec4 v, const mat4x4 m) {
    const vec4 x = _mm_mul_ps(_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
    const vec4 y = _mm_mul_ps(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
    const vec4 z = _mm_mul_ps(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
    const vec4 w = _mm_mul_ps(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
    return _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
}
/* Multiplies a Vector with the given Matrix and change its values. */
void setvec4Mulmat(vec4 *v, const mat4x4 m) {
    const vec4 x = _mm_mul_ps(_mm_shuffle_ps(*v, *v, _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
    const vec4 y = _mm_mul_ps(_mm_shuffle_ps(*v, *v, _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
    const vec4 z = _mm_mul_ps(_mm_shuffle_ps(*v, *v, _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
    const vec4 w = _mm_mul_ps(_mm_shuffle_ps(*v, *v, _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
    *v = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
}
/* Multiplies a vec4 array with the given Matrix and returns a new array, leaving the original unmodified. New array must be freed when not needed anymore. */
vec4 *vec4arrayMulmat(vec4 vecs[], const int len, const mat4x4 m) {
    vec4 *r = malloc(16 * len);
    if (!r)
        return 0;
    vec4 x, y, z, w;
    for (int i = 0; i < len; i++) {
        x = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
        y = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
        z = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
        w = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
        r[i] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
    }
    return r;
}
/* Multiplies a vec4 array with the given Matrix updating the array. */
void setvec4arrayMulmat(vec4 vecs[], const int len, const mat4x4 m) {
    vec4 x, y, z, w;
    for (int i = 0; i < len; i++) {
        x = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
        y = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
        z = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
        w = _mm_mul_ps(_mm_shuffle_ps(vecs[i], vecs[i], _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
        vecs[i] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
    }
}
/* Multiplies a face array with the given Matrix and returns a new array, leaving the original unmodified. New array must be freed when not needed anymore. */
face *facearrayMulmat(face f[], const int len, const mat4x4 m) {
    face *r = malloc(sizeof(face) * len);
    if (!r)
        return 0;
    vec4 x, y, z, w;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 3; j++) {
            x = _mm_mul_ps(_mm_shuffle_ps(f[i].v[j], f[i].v[j], _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
            y = _mm_mul_ps(_mm_shuffle_ps(f[i].v[j], f[i].v[j], _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
            z = _mm_mul_ps(_mm_shuffle_ps(f[i].v[j], f[i].v[j], _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
            w = _mm_mul_ps(_mm_shuffle_ps(f[i].v[j], f[i].v[j], _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
            r[i].v[j] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));

            x = _mm_mul_ps(_mm_shuffle_ps(f[i].vn[j], f[i].vn[j], _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
            y = _mm_mul_ps(_mm_shuffle_ps(f[i].vn[j], f[i].vn[j], _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
            z = _mm_mul_ps(_mm_shuffle_ps(f[i].vn[j], f[i].vn[j], _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
            w = _mm_mul_ps(_mm_shuffle_ps(f[i].vn[j], f[i].vn[j], _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
            r[i].vn[j] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
        }
    }
    return r;
}
/* Multiplies a face array with the given Matrix and returns a new array, leaving the original unmodified. New array must be freed when not needed anymore. */
void setfacearrayMulmat(face f[], const int len, const mat4x4 m) {
    face r;
    vec4 x, y, z, w;
    for (int i = 0; i < len; i++) {
        r = f[i];
        for (int j = 0; j < 3; j++) {
            x = _mm_mul_ps(_mm_shuffle_ps(r.v[j], r.v[j], _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
            y = _mm_mul_ps(_mm_shuffle_ps(r.v[j], r.v[j], _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
            z = _mm_mul_ps(_mm_shuffle_ps(r.v[j], r.v[j], _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
            w = _mm_mul_ps(_mm_shuffle_ps(r.v[j], r.v[j], _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
            f[i].v[j] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));

            x = _mm_mul_ps(_mm_shuffle_ps(r.vn[j], r.vn[j], _MM_SHUFFLE(0, 0, 0, 0)), m.m[0]);
            y = _mm_mul_ps(_mm_shuffle_ps(r.vn[j], r.vn[j], _MM_SHUFFLE(1, 1, 1, 1)), m.m[1]);
            z = _mm_mul_ps(_mm_shuffle_ps(r.vn[j], r.vn[j], _MM_SHUFFLE(2, 2, 2, 2)), m.m[2]);
            w = _mm_mul_ps(_mm_shuffle_ps(r.vn[j], r.vn[j], _MM_SHUFFLE(3, 3, 3, 3)), m.m[3]);
            f[i].vn[j] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
        }
    }
}
/* Multiplies two given Matrices m1, m2.Returns a new 4x4 Matrix. */
mat4x4 matMulmat(const mat4x4 m1, const mat4x4 m2) {
    mat4x4 r;
    vec4 x, y, z, w;
    for (int i = 0; i < 4; i++) {
        x = _mm_mul_ps(_mm_shuffle_ps(m1.m[i], m1.m[i], _MM_SHUFFLE(0, 0, 0, 0)), m2.m[0]);
        y = _mm_mul_ps(_mm_shuffle_ps(m1.m[i], m1.m[i], _MM_SHUFFLE(1, 1, 1, 1)), m2.m[1]);
        z = _mm_mul_ps(_mm_shuffle_ps(m1.m[i], m1.m[i], _MM_SHUFFLE(2, 2, 2, 2)), m2.m[2]);
        w = _mm_mul_ps(_mm_shuffle_ps(m1.m[i], m1.m[i], _MM_SHUFFLE(3, 3, 3, 3)), m2.m[3]);
        r.m[i] = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
    }
    return r;
}
/* Transposes the given Matrix returning a new Matrix. */
mat4x4 transposeMatrix(const mat4x4 m) {
    const vec4 temp1 = _mm_shuffle_ps(m.m[0], m.m[1], _MM_SHUFFLE(1, 0, 1, 0));
    const vec4 temp2 = _mm_shuffle_ps(m.m[2], m.m[3], _MM_SHUFFLE(1, 0, 1, 0));
    const vec4 temp3 = _mm_shuffle_ps(m.m[0], m.m[1], _MM_SHUFFLE(3, 2, 3, 2));
    const vec4 temp4 = _mm_shuffle_ps(m.m[2], m.m[3], _MM_SHUFFLE(3, 2, 3, 2));

    return (mat4x4) {
        _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(2, 0, 2, 0)),
        _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(3, 1, 3, 1)),
        _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(2, 0, 2, 0)),
        _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(3, 1, 3, 1))
    };
}
/* Inverts the given Matrix returning a new Matrix. */
mat4x4 inverseMatrix(const mat4x4 m) {
    mat4x4 r;
    const vec4 temp1 = _mm_shuffle_ps(m.m[0], m.m[1], _MM_SHUFFLE(1, 0, 1, 0));
    const vec4 temp2 = _mm_shuffle_ps(m.m[2], m.m[3], _MM_SHUFFLE(1, 0, 1, 0));
    const vec4 temp3 = _mm_shuffle_ps(m.m[0], m.m[1], _MM_SHUFFLE(3, 2, 3, 2));
    const vec4 temp4 = _mm_shuffle_ps(m.m[2], m.m[3], _MM_SHUFFLE(3, 2, 3, 2));

    r.m[0] = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(2, 0, 2, 0));
    r.m[0].m128_f32[3] = 0.f;

    r.m[1] = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(3, 1, 3, 1));
    r.m[1].m128_f32[3] = 0.f;


    r.m[2] = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(2, 0, 2, 0));
    r.m[2].m128_f32[3] = 0.f;

    const vec4 left = _mm_mul_ps(_mm_shuffle_ps(m.m[3], m.m[3], _MM_SHUFFLE(3, 0, 0, 0)), r.m[0]);
    const vec4 middle = _mm_mul_ps(_mm_shuffle_ps(m.m[3], m.m[3], _MM_SHUFFLE(3, 1, 1, 1)), r.m[1]);
    const vec4 right = _mm_mul_ps(_mm_shuffle_ps(m.m[3], m.m[3], _MM_SHUFFLE(3, 2, 2, 2)), r.m[2]);
    r.m[3] = _mm_xor_ps(invmor, _mm_add_ps(_mm_add_ps(left, middle), right));
    r.m[3].m128_f32[3] = 1.f;

    return r;
}
#else // ITERATIVE_CODE #########################################################################################
/* Scale Matrix. */
mat4x4 scaleMatrix(const float scale) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = scale;
    m.m[1].m128_f32[1] = scale;
    m.m[2].m128_f32[2] = scale;
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Translation Matrix. */
mat4x4 translationMatrix(const float x, const float y, const float z) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = 1.0f;
    m.m[1].m128_f32[1] = 1.0f;
    m.m[2].m128_f32[2] = 1.0f;
    m.m[3].m128_f32[3] = 1.0f;
    m.m[3].m128_f32[0] = x;
    m.m[3].m128_f32[1] = y;
    m.m[3].m128_f32[2] = z;
    return m;
}
/* Rotate Object on X axis according the world origin */
mat4x4 rotateXMatrix(const float angle) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = 1.0f;
    m.m[1].m128_f32[1] = cosf(angle);
    m.m[1].m128_f32[2] = -sinf(angle);
    m.m[2].m128_f32[1] = sinf(angle);
    m.m[2].m128_f32[2] = cosf(angle);
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Rotate Object on Y axis according the world origin. */
mat4x4 rotateYMatrix(const float angle) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = cosf(angle);
    m.m[1].m128_f32[1] = 1.0f;
    m.m[0].m128_f32[2] = -sinf(angle);
    m.m[2].m128_f32[0] = sinf(angle);
    m.m[2].m128_f32[2] = cosf(angle);
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Rotate Object on Z axis according the world origin */
mat4x4 rotateZMatrix(const float angle) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = cosf(angle);
    m.m[0].m128_f32[1] = sinf(angle);
    m.m[1].m128_f32[0] = -sinf(angle);
    m.m[1].m128_f32[1] = cosf(angle);
    m.m[2].m128_f32[2] = 1.0f;
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Orthographic Projection Matrix. l: left, r: right, t: top, b: bottom, n: near, f: far.*/
mat4x4 orthographicMatrix(const float l, const float r, const float t, const float b, const float n, const float f) {
    mat4x4 m = { 0 };
    m.m[0].m128_f32[0] = 2.f / (r - l);
    m.m[1].m128_f32[1] = 2.f / (b - t);
    m.m[2].m128_f32[2] = (1.0f / (n - f));
    m.m[3].m128_f32[0] = ((r + l) / (r - l));
    m.m[3].m128_f32[1] = ((b + t) / (b - t));
    m.m[3].m128_f32[2] = ((f + n) / (n - f));
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* Perspective Projection Matrix. Input values are fov: field of view, ar: aspect ratio, zn: Z near, zf: Z far. */
mat4x4 perspectiveMatrix(const float fov, const float ar, const float zn, const float zf) {
    mat4x4 m = { 0 };
    float fovRadius = 1.f / tanf(fov * 0.5f / 180.0f * 3.14159f);
    m.m[0].m128_f32[0] = fovRadius;
    m.m[1].m128_f32[1] = ar * fovRadius;
    m.m[2].m128_f32[2] = zf / (zf - zn);
    m.m[2].m128_f32[3] = 1.0f;
    m.m[3].m128_f32[2] = (zf * zn) / (zn - zf);
    return m;
}
/* Reverse Perspective Projection Matrix. fov: field of view, ar: aspect ratio. */
mat4x4 reperspectiveMatrix(const float fov, const float ar) {
    mat4x4 m = { 0 };
    const float fovRadius = 1.f / tanf(fov * 0.5f / 180.0f * 3.14159f);
    m.m[0].m128_f32[0] = ar / fovRadius;
    m.m[1].m128_f32[1] = ar / fovRadius;
    m.m[3].m128_f32[2] = 1.0f;
    m.m[3].m128_f32[3] = 1.0f;
    return m;
}
/* The Camera Matrix or as used to called the View Matrix.Returns a new 4x4 Matrix. */
mat4x4 lookatMatrix(const vec4 P, const vec4 U, const vec4 V, const vec4 N) {
    mat4x4 m;
    m.m[0].m128_f32[0] = U.m128_f32[0];    m.m[0].m128_f32[1] = U.m128_f32[1];    m.m[0].m128_f32[2] = U.m128_f32[2];    m.m[0].m128_f32[3] = 0.0;
    m.m[1].m128_f32[0] = V.m128_f32[0];    m.m[1].m128_f32[1] = V.m128_f32[1];    m.m[1].m128_f32[2] = V.m128_f32[2];    m.m[1].m128_f32[3] = 0.0;
    m.m[2].m128_f32[0] = N.m128_f32[0];    m.m[2].m128_f32[1] = N.m128_f32[1];    m.m[2].m128_f32[2] = N.m128_f32[2];    m.m[2].m128_f32[3] = 0.0;
    m.m[3].m128_f32[0] = P.m128_f32[0];    m.m[3].m128_f32[1] = P.m128_f32[1];    m.m[3].m128_f32[2] = P.m128_f32[2];    m.m[3].m128_f32[3] = 1.0;
    return m;
}
/* The Point at Matrix.Takes a position P, a target point vector T, and an up vector Up and returns a matrix to point at location T. */
mat4x4 pointatMatrix(const vec4 P, const vec4 T, const vec4 Up) {
    mat4x4 m;
    vec4 N = vecNormalize(vecSubvec(P, T));
    vec4 U = vecNormalize(crossProduct(Up, N));
    vec4 V = crossProduct(N, U);

    m.m[0].m128_f32[0] = U.m128_f32[0];    m.m[0].m128_f32[1] = U.m128_f32[1];    m.m[0].m128_f32[2] = U.m128_f32[2];    m.m[0].m128_f32[3] = 0.0;
    m.m[1].m128_f32[0] = V.m128_f32[0];    m.m[1].m128_f32[1] = V.m128_f32[1];    m.m[1].m128_f32[2] = V.m128_f32[2];    m.m[1].m128_f32[3] = 0.0;
    m.m[2].m128_f32[0] = N.m128_f32[0];    m.m[2].m128_f32[1] = N.m128_f32[1];    m.m[2].m128_f32[2] = N.m128_f32[2];    m.m[2].m128_f32[3] = 0.0;
    m.m[3].m128_f32[0] = P.m128_f32[0];    m.m[3].m128_f32[1] = P.m128_f32[1];    m.m[3].m128_f32[2] = P.m128_f32[2];    m.m[3].m128_f32[3] = 1.0;
    return m;
}
/* Multiplies a Vector with the given Matrix and returns a new Vector, leaving the original unmodified. */
vec4 vec4Mulmat(const vec4 v, const mat4x4 m) {
    vec4 r;
    for (int i = 0; i < 4; i++) {
        r.m128_f32[i] = v.m128_f32[0] * m.m[0].m128_f32[i] + v.m128_f32[1] * m.m[1].m128_f32[i] + v.m128_f32[2] * m.m[2].m128_f32[i] + v.m128_f32[3] * m.m[3].m128_f32[i];
    }
    return r;
}
/* Multiplies a Vector with the given Matrix and change its values. */
void setvec4Mulmat(vec4 *v, const mat4x4 m) {
    vec4 r = *v;
    for (int i = 0; i < 4; i++) {
        v->m128_f32[i] = r.m128_f32[0] * m.m[0].m128_f32[i] + r.m128_f32[1] * m.m[1].m128_f32[i] + r.m128_f32[2] * m.m[2].m128_f32[i] + r.m128_f32[3] * m.m[3].m128_f32[i];
    }
}
/* Multiplies a vec4 array with the given Matrix and returns a new array, which includes the original array information, leaving the original unmodified. */
vec4 *vec4arrayMulmat(vec4 vecs[], const int len, const mat4x4 m) {
    vec4 *r = malloc(16 * len);
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 4; j++) {
            r[i].m128_f32[j] = vecs[i].m128_f32[0] * m.m[0].m128_f32[j] + vecs[i].m128_f32[1] * m.m[1].m128_f32[j] + vecs[i].m128_f32[2] * m.m[2].m128_f32[j] + vecs[i].m128_f32[3] * m.m[3].m128_f32[j];
        }
    }
    return r;
}
/* Multiplies a vec4 array with the given Matrix updating the array. */
void setvec4arrayMulmat(vec4 vecs[], const int len, const mat4x4 m) {
    vec4 r;
    for (int i = 0; i < len; i++) {
        r = vecs[i];
        for (int j = 0; j < 4; j++) {
            vecs[i].m128_f32[j] = r.m128_f32[0] * m.m[0].m128_f32[j] + r.m128_f32[1] * m.m[1].m128_f32[j] + r.m128_f32[2] * m.m[2].m128_f32[j] + r.m128_f32[3] * m.m[3].m128_f32[j];
        }
    }
}
/* Multiplies a face array with the given Matrix and returns a new array, leaving the original unmodified. New array must be freed when not needed anymore. */
face* facearrayMulmat(face f[], const int len, const mat4x4 m) {
    face* r = malloc(sizeof(face) * len);
    if (!r)
        return 0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 3; j++) {
            r[i].v[j].m128_f32[0] = f[i].v[j].m128_f32[0] * m.m[0].m128_f32[0] + f[i].v[j].m128_f32[1] * m.m[1].m128_f32[0] + f[i].v[j].m128_f32[2] * m.m[2].m128_f32[0] + f[i].v[j].m128_f32[3] * m.m[3].m128_f32[0];
            r[i].v[j].m128_f32[1] = f[i].v[j].m128_f32[0] * m.m[0].m128_f32[1] + f[i].v[j].m128_f32[1] * m.m[1].m128_f32[1] + f[i].v[j].m128_f32[2] * m.m[2].m128_f32[1] + f[i].v[j].m128_f32[3] * m.m[3].m128_f32[1];
            r[i].v[j].m128_f32[2] = f[i].v[j].m128_f32[0] * m.m[0].m128_f32[2] + f[i].v[j].m128_f32[1] * m.m[1].m128_f32[2] + f[i].v[j].m128_f32[2] * m.m[2].m128_f32[2] + f[i].v[j].m128_f32[3] * m.m[3].m128_f32[2];
            r[i].v[j].m128_f32[3] = f[i].v[j].m128_f32[0] * m.m[0].m128_f32[3] + f[i].v[j].m128_f32[1] * m.m[1].m128_f32[3] + f[i].v[j].m128_f32[2] * m.m[2].m128_f32[3] + f[i].v[j].m128_f32[3] * m.m[3].m128_f32[3];

            r[i].vn[j].m128_f32[0] = f[i].vn[j].m128_f32[0] * m.m[0].m128_f32[0] + f[i].vn[j].m128_f32[1] * m.m[1].m128_f32[0] + f[i].vn[j].m128_f32[2] * m.m[2].m128_f32[0] + f[i].vn[j].m128_f32[3] * m.m[3].m128_f32[0];
            r[i].vn[j].m128_f32[1] = f[i].vn[j].m128_f32[0] * m.m[0].m128_f32[1] + f[i].vn[j].m128_f32[1] * m.m[1].m128_f32[1] + f[i].vn[j].m128_f32[2] * m.m[2].m128_f32[1] + f[i].vn[j].m128_f32[3] * m.m[3].m128_f32[1];
            r[i].vn[j].m128_f32[2] = f[i].vn[j].m128_f32[0] * m.m[0].m128_f32[2] + f[i].vn[j].m128_f32[1] * m.m[1].m128_f32[2] + f[i].vn[j].m128_f32[2] * m.m[2].m128_f32[2] + f[i].vn[j].m128_f32[3] * m.m[3].m128_f32[2];
            r[i].vn[j].m128_f32[3] = f[i].vn[j].m128_f32[0] * m.m[0].m128_f32[3] + f[i].vn[j].m128_f32[1] * m.m[1].m128_f32[3] + f[i].vn[j].m128_f32[2] * m.m[2].m128_f32[3] + f[i].vn[j].m128_f32[3] * m.m[3].m128_f32[3];
        }
    }
    return r;
}
/* Multiplies a face array with the given Matrix and returns a new array, leaving the original unmodified. New array must be freed when not needed anymore. */
void setfacearrayMulmat(face f[], const int len, const mat4x4 m) {
    face r;
    for (int i = 0; i < len; i++) {
        r = f[i];
        for (int j = 0; j < 3; j++) {
            f[i].v[j].m128_f32[0] = r.v[j].m128_f32[0] * m.m[0].m128_f32[0] + r.v[j].m128_f32[1] * m.m[1].m128_f32[0] + r.v[j].m128_f32[2] * m.m[2].m128_f32[0] + r.v[j].m128_f32[3] * m.m[3].m128_f32[0];
            f[i].v[j].m128_f32[1] = r.v[j].m128_f32[0] * m.m[0].m128_f32[1] + r.v[j].m128_f32[1] * m.m[1].m128_f32[1] + r.v[j].m128_f32[2] * m.m[2].m128_f32[1] + r.v[j].m128_f32[3] * m.m[3].m128_f32[1];
            f[i].v[j].m128_f32[2] = r.v[j].m128_f32[0] * m.m[0].m128_f32[2] + r.v[j].m128_f32[1] * m.m[1].m128_f32[2] + r.v[j].m128_f32[2] * m.m[2].m128_f32[2] + r.v[j].m128_f32[3] * m.m[3].m128_f32[2];
            f[i].v[j].m128_f32[3] = r.v[j].m128_f32[0] * m.m[0].m128_f32[3] + r.v[j].m128_f32[1] * m.m[1].m128_f32[3] + r.v[j].m128_f32[2] * m.m[2].m128_f32[3] + r.v[j].m128_f32[3] * m.m[3].m128_f32[3];

            f[i].vn[j].m128_f32[0] = r.vn[j].m128_f32[0] * m.m[0].m128_f32[0] + r.vn[j].m128_f32[1] * m.m[1].m128_f32[0] + r.vn[j].m128_f32[2] * m.m[2].m128_f32[0] + r.vn[j].m128_f32[3] * m.m[3].m128_f32[0];
            f[i].vn[j].m128_f32[1] = r.vn[j].m128_f32[0] * m.m[0].m128_f32[1] + r.vn[j].m128_f32[1] * m.m[1].m128_f32[1] + r.vn[j].m128_f32[2] * m.m[2].m128_f32[1] + r.vn[j].m128_f32[3] * m.m[3].m128_f32[1];
            f[i].vn[j].m128_f32[2] = r.vn[j].m128_f32[0] * m.m[0].m128_f32[2] + r.vn[j].m128_f32[1] * m.m[1].m128_f32[2] + r.vn[j].m128_f32[2] * m.m[2].m128_f32[2] + r.vn[j].m128_f32[3] * m.m[3].m128_f32[2];
            f[i].vn[j].m128_f32[3] = r.vn[j].m128_f32[0] * m.m[0].m128_f32[3] + r.vn[j].m128_f32[1] * m.m[1].m128_f32[3] + r.vn[j].m128_f32[2] * m.m[2].m128_f32[3] + r.vn[j].m128_f32[3] * m.m[3].m128_f32[3];
        }
    }
}
/* Multiplies two given Matrices m1, m2.Returns a new 4x4 Matrix. */
mat4x4 matMulmat(const mat4x4 m1, const mat4x4 m2) {
    mat4x4 r;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            r.m[j].m128_f32[i] = m1.m[j].m128_f32[0] * m2.m[0].m128_f32[i] + m1.m[j].m128_f32[1] * m2.m[1].m128_f32[i] + m1.m[j].m128_f32[2] * m2.m[2].m128_f32[i] + m1.m[j].m128_f32[3] * m2.m[3].m128_f32[i];
        }
    return r;
}
/* Transposes the given Matrix returning a new Matrix. */
mat4x4 transposeMatrix(const mat4x4 m) {
    mat4x4 rm;
    rm.m[0].m128_f32[0] = m.m[0].m128_f32[0];    rm.m[0].m128_f32[1] = m.m[1].m128_f32[0];    rm.m[0].m128_f32[2] = m.m[2].m128_f32[0];    rm.m[0].m128_f32[3] = m.m[3].m128_f32[0];
    rm.m[1].m128_f32[0] = m.m[0].m128_f32[1];    rm.m[1].m128_f32[1] = m.m[1].m128_f32[1];    rm.m[1].m128_f32[2] = m.m[2].m128_f32[1];    rm.m[1].m128_f32[3] = m.m[3].m128_f32[1];
    rm.m[2].m128_f32[0] = m.m[0].m128_f32[2];    rm.m[2].m128_f32[1] = m.m[1].m128_f32[2];    rm.m[2].m128_f32[2] = m.m[2].m128_f32[2];    rm.m[2].m128_f32[3] = m.m[3].m128_f32[2];
    rm.m[3].m128_f32[0] = m.m[0].m128_f32[3];    rm.m[3].m128_f32[1] = m.m[1].m128_f32[3];    rm.m[3].m128_f32[2] = m.m[2].m128_f32[3];    rm.m[3].m128_f32[3] = m.m[3].m128_f32[3];
    return rm;
}
/* Inverts the given Matrix returning a new Matrix. */
mat4x4 inverseMatrix(const mat4x4 m) {
    mat4x4 rm;
    rm.m[0].m128_f32[0] = m.m[0].m128_f32[0];    rm.m[0].m128_f32[1] = m.m[1].m128_f32[0];    rm.m[0].m128_f32[2] = m.m[2].m128_f32[0];    rm.m[0].m128_f32[3] = 0.0;
    rm.m[1].m128_f32[0] = m.m[0].m128_f32[1];    rm.m[1].m128_f32[1] = m.m[1].m128_f32[1];    rm.m[1].m128_f32[2] = m.m[2].m128_f32[1];    rm.m[1].m128_f32[3] = 0.0;
    rm.m[2].m128_f32[0] = m.m[0].m128_f32[2];    rm.m[2].m128_f32[1] = m.m[1].m128_f32[2];    rm.m[2].m128_f32[2] = m.m[2].m128_f32[2];    rm.m[2].m128_f32[3] = 0.0;

    rm.m[3].m128_f32[0] = -(m.m[3].m128_f32[0] * rm.m[0].m128_f32[0] + m.m[3].m128_f32[1] * rm.m[1].m128_f32[0] + m.m[3].m128_f32[2] * rm.m[2].m128_f32[0]);
    rm.m[3].m128_f32[1] = -(m.m[3].m128_f32[0] * rm.m[0].m128_f32[1] + m.m[3].m128_f32[1] * rm.m[1].m128_f32[1] + m.m[3].m128_f32[2] * rm.m[2].m128_f32[1]);
    rm.m[3].m128_f32[2] = -(m.m[3].m128_f32[0] * rm.m[0].m128_f32[2] + m.m[3].m128_f32[1] * rm.m[1].m128_f32[2] + m.m[3].m128_f32[2] * rm.m[2].m128_f32[2]);
    rm.m[3].m128_f32[3] = 1.0;
    return rm;
}
#endif // VECTORIZED_CODE #######################################################################################


