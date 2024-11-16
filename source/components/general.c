#include "headers/components/general.h"

/* Swaping two variables a and b of any type with size. */
void swap(void* a, void* b, unsigned long size) {
    void* temp = malloc(size);
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
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getmeshRigidLimits(mesh *m) {
    m->rigid.min = m->rigid.v[0];
    m->rigid.max = m->rigid.v[0];
    for (int i = 0; i < 20; i++) {
        m->rigid.min = _mm_min_ps(m->rigid.min, m->rigid.v[i]);
        m->rigid.min = _mm_max_ps(m->rigid.min, m->rigid.v[i]);
    }
}
/* Find how much in each direction the vectors array extends. Populate with values the (min) and (max) vec4 values */
void getvec4ArrayRigidLimits(vec4 v[], vec4 *min, vec4 *max) {
    *min = _mm_load_ps(&v[0]);
    *max = _mm_load_ps(&v[0]);
    for (int i = 0; i < 20; i++) {
        *min = _mm_min_ps(*min, v[i]);
        *max = _mm_max_ps(*max, v[i]);
    }
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getmeshRigidLimits(mesh *m) {
    m->rigid.min = m->rigid.v[0];
    m->rigid.max = m->rigid.v[0];
    for (int i = 0; i < 20; i++) {
        if (m->rigid.min->m128_f32[0] > m->rigid.v[i].m128_f32[0])
            m->rigid.min->m128_f32[0] = m->rigid.v[i].m128_f32[0];

        if (m->rigid.max->m128_f32[0] < m->rigid.v[i].m128_f32[0])
            m->rigid.max->m128_f32[0] = m->rigid.v[i].m128_f32[0];

        if (m->rigid.min->m128_f32[1] > m->rigid.v[i].m128_f32[1])
            m->rigid.min->m128_f32[1] = m->rigid.v[i].m128_f32[1];

        if (m->rigid.max->m128_f32[1] < m->rigid.v[i].m128_f32[1])
            m->rigid.max->m128_f32[1] = m->rigid.v[i].m128_f32[1];

        if (m->rigid.min->m128_f32[2] > m->rigid.v[i].m128_f32[2])
            m->rigid.min->m128_f32[2] = m->rigid.v[i].m128_f32[2];

        if (m->rigid.max->m128_f32[2] < m->rigid.v[i].m128_f32[2])
            m->rigid.max->m128_f32[2] = m->rigid.v[i].m128_f32[2];
    }
}
/* Find how much in each direction the vectors array extends. Populate with values the (min) and (max) vec4 values */
void getvec4ArayRigidLimits(vec4 v[], vec4 *min, vec4 *max) {
    *min = v[0];
    *max = v[0];
    for (int i = 0; i < 20; i++) {
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


