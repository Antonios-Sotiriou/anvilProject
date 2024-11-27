#include "headers/components/general.h"

/* Swaping two variables a and b of any type with size. */
void swap(void *a, void *b, unsigned long size) {
    void *temp = malloc(size);
    if (!temp)
        return;
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}
/* Returns the given value in radians. */
const float radians(const float value) {
    return value * (3.14159 / 180.0);
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Find how much in each direction the vectors array extends. Populate with values the (min) and (max) vec4 values */
void getvec4ArrayLimits(vec4 v[], const int array_len, vec4* min, vec4* max) {
    *min = v[0];
    *max = v[0];
    for (int i = 0; i < array_len; i++) {
        *min = _mm_min_ps(*min, v[i]);
        *max = _mm_max_ps(*max, v[i]);
    }
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the vectors array extends. Populate with values the (min) and (max) vec4 values */
void getvec4ArrayLimits(vec4 v[], const int array_len, vec4 *min, vec4 *max) {
    *min = v[0];
    *max = v[0];
    for (int i = 0; i < array_len; i++) {
        if (min->m128_f32[0] > v[i].m128_f32[0])
            min->m128_f32[0] = v[i].m128_f32[0];

        if (max->m128_f32[0] < v[i].m128_f32[0])
            max->m128_f32[0] = v[i].m128_f32[0];

        if (min->m128_f32[1] > v[i].m128_f32[1])
            min->m128_f32[1] = v[i].m128_f32[1];

        if (max->m128_f32[1] < v[i].m128_f32[1])
            max->m128_f32[1] = v[i].m128_f32[1];

        if (min->m128_f32[2] > v[i].m128_f32[2])
            min->m128_f32[2] = v[i].m128_f32[2];

        if (max->m128_f32[2] < v[i].m128_f32[2])
            max->m128_f32[2] = v[i].m128_f32[2];
    }
}
#endif // VECTORIZED_CODE #######################################################################################


