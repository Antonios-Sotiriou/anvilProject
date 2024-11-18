#include "headers/components/collisions.h"
#include <stdio.h>
#include <string.h>

const int outerRadiusCollision(mesh* m) {
    //vec4 newPos = vecAddvec(m->coords.v[0], m->rigid.velocity);
    int pk;
    for (int i = 0; i < SCENE.t.quad[m->quadIndex].mpks_indexes; i++) {
        pk = SCENE.t.quad[m->quadIndex].mpks[i];
        if (pk != m->pk) {

            vec4 dis = vecSubvec(m->coords.v[0], SCENE.mesh[pk].coords.v[0]);
            if (vecLength(dis) <= (SCENE.mesh[pk].outer_radius + m->outer_radius))
                logvec4(dis);
                //printf("checking for collision: %d --> %d\n", m->pk, pk);
        }
    }
    return 0;
}
const int checkOBBCollision(mesh *m) {
    /* Implement Oriented bounding boxes collision detection. */
    //printf("\x1b[H\x1b[J");
    //system("cls\n");
    //mat4x4 tm = modelMatfromQST(m->rigid.q, 1.f, m->rigid.velocity);
    //mat4x4 tm = matfromQuat(m->rigid.q, m->coords.v[0]);
    mat4x4 tm = translationMatrix(m->rigid.velocity.m128_f32[0], m->rigid.velocity.m128_f32[1], m->rigid.velocity.m128_f32[2]);
    face *temp1 = facearrayMulmat(m->rigid.f, m->rigid.f_indexes, tm);
    //setfacearrayMulmat(m->rigid.f, m->rigid.f_indexes, tm);

    int collision = 0;
    float dot = 0.f, min_outer,  min_inner, max_outer, max_inner;
    for (int i = 0; i < m->rigid.f_indexes; i++) {
        min_outer = (float)_CRT_INT_MAX, min_inner = (float)_CRT_INT_MAX, max_outer = (float)-_CRT_INT_MAX, max_inner = (float)-_CRT_INT_MAX;
        for (int x = 0; x < m->rigid.f_indexes; x++) {
            for (int d = 0; d < 3; d++) {
                min_outer = (dot = dotProduct(temp1[i].vn[0], temp1[x].v[d])) < min_outer ? dot : min_outer;
                max_outer = dot > max_outer ? dot : max_outer;
            }
        }

        for (int y = 0; y < SCENE.mesh[3].rigid.f_indexes; y++) {
            for (int d = 0; d < 3; d++) {
                min_inner = (dot = dotProduct(temp1[i].vn[0], SCENE.mesh[3].rigid.f[y].v[d])) < min_inner ? dot : min_inner;
                max_inner = dot > max_inner ? dot : max_inner;
            }
        }

        if ( (min_outer > max_inner) || (max_outer < min_inner) ) {
            memcpy(m->rigid.f, temp1, m->rigid.f_indexes * sizeof(face));
            free(temp1);
            return 0;
        }
    }

    for (int i = 0; i < SCENE.mesh[3].rigid.f_indexes; i++) {
        min_outer = (float)_CRT_INT_MAX, min_inner = (float)_CRT_INT_MAX, max_outer = (float)-_CRT_INT_MAX, max_inner = (float)-_CRT_INT_MAX;
        for (int x = 0; x < SCENE.mesh[3].rigid.f_indexes; x++) {
            for (int d = 0; d < 3; d++) {
                min_outer = (dot = dotProduct(SCENE.mesh[3].rigid.f[i].vn[0], SCENE.mesh[3].rigid.f[x].v[d])) < min_outer ? dot : min_outer;
                max_outer = dot > max_outer ? dot : max_outer;
            }
        }

        for (int y = 0; y < m->rigid.f_indexes; y++) {
            for (int d = 0; d < 3; d++) {
                min_inner = (dot = dotProduct(SCENE.mesh[3].rigid.f[i].vn[0], temp1[y].v[d])) < min_inner ? dot : min_inner;
                max_inner = dot > max_inner ? dot : max_inner;
            }
        }

        if ( (min_outer > max_inner) || (max_outer < min_inner) ) {
            memcpy(m->rigid.f, temp1, m->rigid.f_indexes * sizeof(face));
            free(temp1);
            return 0;
        }
    }
    printf("Collision Detected... %d\n", collision);

    memcpy(m->rigid.f, temp1, m->rigid.f_indexes * sizeof(face));
    free(temp1);
    return 1;
}


