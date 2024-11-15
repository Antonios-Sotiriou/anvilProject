#include "headers/components/general.h"

/* Swaping two variables a and b of any type with size. */
const void swap(void* a, void* b, unsigned long size) {
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
void getRigidLimits(mesh *m) {

    for (int i = 0; i < m->rigid.v_indexes; i++) {

        if (m->rigid.v[i].m128_f32[0] < m->rigid.v[i].m128_f32[0])
            m->rigid.min.m128_f32[0] = m->rigid.v[i].m128_f32[0];
        else
            m->rigid.max.m128_f32[0] = m->rigid.v[i].m128_f32[0];

        //if ( va[i][0] <= dl.min[0]) {
        //    dl.min[0] = va[i][0];
        //} else if ( va[i][0] > dl.max[0]) {
        //    dl.max[0] = va[i][0];
        //}
        ///* Get min and max y values. */
        //if ( va[i][1] <= dl.min[1]) {
        //    dl.min[1] = va[i][1];
        //} else if ( va[i][1] > dl.max[1]) {
        //    dl.max[1] = va[i][1];
        //}
        ///* Get min and max z values. */
        //if ( va[i][2] <= dl.min[2]) {
        //    dl.min[2] = va[i][2];
        //} else if ( va[i][2] > dl.max[2]) {
        //    dl.max[2] = va[i][2];
        //}
    }
}


