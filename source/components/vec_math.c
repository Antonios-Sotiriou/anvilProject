#include "headers/components/vec_math.h"

#ifdef VECTORIZED_CODE // #######################################################################################
/* Computes the Dot Product of two given Vectors. */
float dotProduct(const vec4 v1, const vec4 v2) {
    vec4 r = _mm_mul_ps(v1, v2);
    return _mm_cvtss_f32(
               _mm_add_ps(
                   _mm_add_ps(r, _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 1))),
                   _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 2))
               )
           );
}
/* Computes the Cross Product of two given Vectors.Returns a new Vector. */
vec4 crossProduct(const vec4 v1, const vec4 v2) {
    return _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 0, 2, 1)), _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(0, 1, 0, 2))), 
        _mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 1, 0, 2)), _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(0, 0, 2, 1)))
    );
}
/* Computes the length of a vector with the use of Pythagorean Theorem. */
float vecLength(const vec4 v) {
    return _mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ps1(dotProduct(v, v))));
}
/* Normalizes a Vector so he ranges from 0 to 1.Returns a new Vector. */
vec4 vecNormalize(const vec4 v) {
    float len = vecLength(v);
    return _mm_div_ps(v, _mm_load_ps1(&len));
}
/* Multiplies a Vector by the given num.Returns a new Vector. */
vec4 vecMulf32(const vec4 v1, const float num) {
    return _mm_mul_ps(v1, _mm_load_ps1(&num));
}
/* Multiplies 2 Vectors with each other.Returns a new Vector. */
vec4 vecMulvec(const vec4 v1, const vec4 v2) {
    return _mm_mul_ps(v1, v2);
}
/* Divides a Vector by the given num.Returns a new Vector. */
vec4 vecDivf32(const vec4 v1, const float num) {
    return _mm_div_ps(v1, _mm_load_ps1(&num));
}
/* Divides 2 Vectors with each other.Returns a new Vector. */
vec4 vecDivvec(const vec4 v1, const vec4 v2) {
    return _mm_div_ps(v1, v2);
}
/* Adds a num to the given Vector.Returns a new Vector. */
vec4 vecAddf32(const vec4 v1, const float num) {
    return _mm_add_ps(v1, _mm_load_ps1(&num));
}
/* Adds two Vectors to each other.Returns a new Vector. */
vec4 vecAddvec(const vec4 v1, const vec4 v2) {
    return _mm_add_ps(v1, v2);
}
/* Substructs a num from the given Vector.Returns a new Vector. */
vec4 vecSubf32(const vec4 v1, const float num) {
    return _mm_sub_ps(v1, _mm_load_ps1(&num));
}
/* Substructs two Vectors from each other.Returns a new Vector. */
vec4 vecSubvec(const vec4 v1, const vec4 v2) {
    return _mm_sub_ps(v1, v2);
}
#else // ITERATIVE_CODE #########################################################################################
/* Computes the Dot Product of two given Vectors. */
float dotProduct(const vec4 v1, const vec4 v2) {
    return v1.f32[0] * v2.f32[0] + v1.f32[1] * v2.f32[1] + v1.f32[2] * v2.f32[2];
}
/* Computes the Cross Product of two given Vectors.Returns a new Vector. */
vec4 crossProduct(const vec4 v1, const vec4 v2) {
    vec4 cp;
    cp.f32[0] = v1.f32[1] * v2.f32[2] - v1.f32[2] * v2.f32[1];
    cp.f32[1] = v1.f32[2] * v2.f32[0] - v1.f32[0] * v2.f32[2];
    cp.f32[2] = v1.f32[0] * v2.f32[1] - v1.f32[1] * v2.f32[0];
    cp.f32[3] = 0.f;
    return cp;
}
/* Computes the length of a vector with the use of Pythagorean Theorem. */
float vecLength(const vec4 v) {
    return sqrtf(dotProduct(v, v));
}
/* Normalizes a Vector so he ranges from 0 to 1.Returns a new Vector. */
vec4 vecNormalize(const vec4 v) {
    float len = vecLength(v);
    return (vec4) { v.f32[0] / len, v.f32[1] / len, v.f32[2] / len, v.f32[3] / len };
}
/* Multiplies a Vector by the given num.Returns a new Vector. */
vec4 vecMulf32(const vec4 v1, const float num) {
    return (vec4) { v1.f32[0] * num, v1.f32[1] * num, v1.f32[2] * num, v1.f32[3] * num };
}
/* Multiplies 2 Vectors with each other.Returns a new Vector. */
vec4 vecMulvec(const vec4 v1, const vec4 v2) {
    return (vec4) { v1.f32[0] * v2.f32[0], v1.f32[1] * v2.f32[1], v1.f32[2] * v2.f32[2], v1.f32[3] * v2.f32[3] };
}
/* Divides a Vector by the given num.Returns a new Vector. */
vec4 vecDivf32(const vec4 v1, const float num) {
    return (vec4) { v1.f32[0] / num, v1.f32[1] / num, v1.f32[2] / num, v1.f32[3] / num };
}
/* Divides 2 Vectors with each other.Returns a new Vector. */
vec4 vecDivvec(const vec4 v1, const vec4 v2) {
    return (vec4) { v1.f32[0] / v2.f32[0], v1.f32[1] / v2.f32[1], v1.f32[2] / v2.f32[2], v1.f32[3] / v2.f32[3] };
}
/* Adds a num to the given Vector.Returns a new Vector. */
vec4 vecAddf32(const vec4 v1, const float num) {
    return (vec4) { v1.f32[0] + num, v1.f32[1] + num, v1.f32[2] + num, v1.f32[3] + num };
}
/* Adds two Vectors to each other.Returns a new Vector. */
vec4 vecAddvec(const vec4 v1, const vec4 v2) {
    return (vec4) { v1.f32[0] + v2.f32[0], v1.f32[1] + v2.f32[1], v1.f32[2] + v2.f32[2], v1.f32[3] + v2.f32[3] };
}
/* Substructs a num from the given Vector.Returns a new Vector. */
vec4 vecSubf32(const vec4 v1, const float num) {
    return (vec4) { v1.f32[0] - num, v1.f32[1] - num, v1.f32[2] - num, v1.f32[3] - num };
}
/* Substructs two Vectors from each other.Returns a new Vector. */
vec4 vecSubvec(const vec4 v1, const vec4 v2) {
    return (vec4) { v1.f32[0] - v2.f32[0], v1.f32[1] - v2.f32[1], v1.f32[2] - v2.f32[2], v1.f32[3] - v2.f32[3] };
}
#endif // VECTORIZED_CODE #######################################################################################

