#include "headers/components/collisions.h"
#include <stdio.h>

const int checkOBBCollision(mesh *m) {
    /* Implement Oriented bounding boxes collision detection. */
    mat4x4 qm = modelMatfromQST(m->rigid.q, m->scale, m->coords.v[0]);
    face *temp1 = facearrayMulmat(m->rigid.f, m->rigid.f_indexes, qm);

    int pk;
    for (int i = 0; i < SCENE.t.quad[m->quadIndex].mpks_indexes; i++) {

        int pk = SCENE.t.quad[m->quadIndex].mpks[i];
        if (pk != m->pk) {
            for (int j = 0; j < m->rigid.f_indexes; j++) {

                for (int x = 0; x < SCENE.mesh[pk].rigid.f_indexes; x++) {

                    qm = modelMatfromQST(SCENE.mesh[pk].rigid.q, SCENE.mesh[pk].scale, SCENE.mesh[pk].coords.v[0]);
                    face *temp2 = facearrayMulmat(SCENE.mesh[pk].rigid.f, SCENE.mesh[pk].rigid.f_indexes, qm);

                    printf("%.3f  ", dotProduct(vecNormalize(temp1[j].vn[0]), vecNormalize(temp2[x].vn[0])));
                    free(temp2);
                }
            }
            printf("\n");
        }
    }
    free(temp1);
}


