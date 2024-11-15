#include "headers/components/collisions.h"
#include <stdio.h>
#include <string.h>

const int checkOBBCollision(mesh *m) {
    /* Implement Oriented bounding boxes collision detection. */
    //printf("\x1b[H\x1b[J");
    //system("cls\n");
    mat4x4 tm = translationMatrix(m->rigid.velocity.m128_f32[0], m->rigid.velocity.m128_f32[1], m->rigid.velocity.m128_f32[2]);
    face *temp1 = facearrayMulmat(m->rigid.f, m->rigid.f_indexes, tm);

    int pk;
    for (int i = 0; i < SCENE.t.quad[m->quadIndex].mpks_indexes; i++) {

        pk = SCENE.t.quad[m->quadIndex].mpks[i];
        if (pk != m->pk) {

            for (int j = 0; j < m->rigid.f_indexes; j++) {
                for (int x = 0; x < SCENE.mesh[pk].rigid.f_indexes; x++) {
                    //printf("%.3f  ", dotProduct(temp1[0].vn[0], SCENE.mesh[pk].rigid.f[x].vn[0]));
                    //logvec4(m->rigid.f[0].vn[0]);
                }

            }
            //printf("\n");
        }
    }
    //if (m->rigid.rot_angle)
        //memcpy(m->rigid.f, temp1, m->rigid.f_indexes * sizeof(face));
    free(temp1);
    return 0;
}


