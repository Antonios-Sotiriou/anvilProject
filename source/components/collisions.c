#include "headers/components/collisions.h"

extern void swap(void *a, void *b, unsigned long size);
/* Checks for collisions whithin a radius sourounding the mesh. */
const int staticOuterRadiusCollision(mesh *m) {
    //vec4 newPos = vecAddvec(m->coords.v[0], m->rigid.velocity);
    int pk;
    for (int i = 0; i < SCENE.t.quad[m->quadIndex].mpks_indexes; i++) {
        pk = SCENE.t.quad[m->quadIndex].mpks[i];
        if (pk != m->pk) {

            vec4 dis = vecSubvec(m->coords.v[0], SCENE.mesh[pk].coords.v[0]);
            if (vecLength(dis) <= (SCENE.mesh[pk].outer_radius + m->outer_radius))
                // logvec4(dis);
                printf("checking for collision: %d --> %d\n", m->pk, pk);
        }
    }
    return 0;
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Check swept Axis Aligned Bounding Boxes collisions between, given mesh (*m) and a Primary keys array of possible colliders (pks). */
const int sweptAABBCollision(mesh *m, const int pks[]) {

    if (m->quadIndex < 0) {
        fprintf(stderr, "obj->quadIndex : %d. Out of Terrain. ObjectEnvironmentCollision().\n", m->quadIndex);
        return 0;
    }

    getmeshRigidLimits(m);
    vec4 tnear, tfar, min, max;

    const int num_of_members = 1;

    for (int i = 0; i < num_of_members; i++) {

        int pk = pks[i];

        if (pk != m->pk) {

            min = roundvec4(vecSubvec(SCENE.mesh[pk].rigid.min, vecSubvec(m->coords.v[0], m->rigid.min)));
            max = roundvec4(vecSubvec(SCENE.mesh[pk].rigid.max, vecSubvec(m->coords.v[0], m->rigid.max)));

            tnear = vecDivvec(vecSubvec(min, m->coords.v[0]), m->rigid.velocity);
            tfar = vecDivvec(vecSubvec(max, m->coords.v[0]), m->rigid.velocity);

            if ( vecEqualvec(tnear, tnear) || vecEqualvec(tfar, tfar) )
                continue;

            min = _mm_min_ps(tnear, tfar);
            max = _mm_max_ps(tnear, tfar);

            int check_xz = _mm_movemask_ps(_mm_cmpgt_ps(min, _mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 0, 1, 2))));
            if ( check_xz == 1 || check_xz == 4 ) {
                continue;
            }

            float t_near = 0.f, t_far = 0.f;
            if (_mm_movemask_ps(_mm_cmpgt_ps(min, _mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 2)))) == 1) {
                t_near = _mm_cvtss_f32(min);
            } else {
                t_near = _mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 2)));
            }

            if (_mm_movemask_ps(_mm_cmplt_ps(max, _mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 2)))) == 1) {
                t_far = _mm_cvtss_f32(max);
            } else {
                t_far = _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 2)));
            }

            /* ##################### Y ############################ */
            if ( (t_near > _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 1)))) || (_mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 1))) > t_far) )
                continue;

            if ( _mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 1))) > t_near )
                t_near = _mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 1)));
            if ( _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 1))) > t_far )
                t_far = _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 1)));
            /* ##################### Y ############################ */

            if (((t_far < 0) || (t_near < 0)) || (t_near > 1.f))
                continue;

            vec4 normal = { 0.f };
            if (vec4ExtractX(min) >= vec4ExtractY(min) && vec4ExtractX(min) >= vec4ExtractZ(min)) {
                if (vec4ExtractX(m->rigid.velocity) < 0) {
                    vec4SetX(&normal, 1.f);
                } else if (vec4ExtractX(m->rigid.velocity) > 0) {
                    vec4SetX(&normal, -1.f);
                }
            } else if (vec4ExtractY(min) >= vec4ExtractX(min) && vec4ExtractY(min) >= vec4ExtractZ(min)) {
                if (vec4ExtractY(m->rigid.velocity) < 0) {
                    vec4SetY(&normal, 1.f);
                } else if (vec4ExtractY(m->rigid.velocity) > 0) {
                    vec4SetY(&normal, -1.f);
                }
            } else if (vec4ExtractZ(min) >= vec4ExtractX(min) && vec4ExtractZ(min) >= vec4ExtractY(min)) {
                if (vec4ExtractZ(m->rigid.velocity) < 0) {
                    vec4SetZ(&normal, 1.f);
                } else if (vec4ExtractZ(m->rigid.velocity) > 0) {
                    vec4SetZ(&normal, -1.f);
                }
            }

            if (t_near == 0.f) {
                printf("Sliding.... %f\n", t_near);

                float dot = dotProduct(m->rigid.velocity, normal);
                m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, dot));

                //if (tnear.m128_f32[1] == 0) {
                //    //obj->falling_time = 0.f;
                //    //m->rigid.velocity = vecMulvec(m->rigid.velocity,  m->rigid.momentum);
                //}
                //else {
                //    m->rigid.velocity = (gravity_epicenter * (98.1f * (m->rigid.falling_time * 2.f))) + (m->rigid.velocity * m->rigid.momentum);
                //}
                return 2;

            } else if (((t_near > 0.f) && (t_near <= 1.f))) {
                printf("COLLISION  ######################### : %f    mesh.id: %d\n", t_near, pk);

                m->rigid.velocity = vecMulf32(m->rigid.velocity, t_near);

                mat4x4 trans = translationMatrix(vec4ExtractX(m->rigid.velocity), vec4ExtractY(m->rigid.velocity), vec4ExtractZ(m->rigid.velocity));

                setvec4arrayMulmat(m->coords.v, 4, trans);
                setfacearrayMulmat(m->rigid.f, m->rigid.f_indexes, trans);

                //obj->momentum *= cache->fr_coef;
                //float dot = dot_product(normal, obj->mvdir);
                //obj->mvdir = obj->mvdir - (dot * normal);
                //obj->velocity = (obj->mvdir * obj->momentum);
                float col_dot = dotProduct(m->rigid.velocity, normal);
                m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, col_dot));

                return 1;
            }
        }
    }
    return 0;
}
#else // ITERATIVE_CODE #########################################################################################
/* Check swept Axis Aligned Bounding Boxes collisions between, given mesh (*m) and a Primary keys array of possible colliders (pks). */
const int sweptAABBCollision(mesh *m, const int pks[]) {

    if (m->quadIndex < 0) {
        fprintf(stderr, "obj->quadIndex : %d. Out of Terrain. ObjectEnvironmentCollision().\n", m->quadIndex);
        return 0;
    }

    getmeshRigidLimits(m);
    vec4 tnear, tfar, min, max;

    const int num_of_members = 1;

    for (int i = 0; i < num_of_members; i++) {

        int pk = pks[i];

        if (pk != m->pk) {

            min = roundvec4(vecSubvec(SCENE.mesh[pk].rigid.min, vecSubvec(m->coords.v[0], m->rigid.min)));
            max = roundvec4(vecSubvec(SCENE.mesh[pk].rigid.max, vecSubvec(m->coords.v[0], m->rigid.max)));

            tnear = vecDivvec(vecSubvec(min, m->coords.v[0]), m->rigid.velocity);
            tfar = vecDivvec(vecSubvec(max, m->coords.v[0]), m->rigid.velocity);

            if ( vecEqualvec(tnear, tnear) || vecEqualvec(tfar, tfar) )
                continue;

            if (tnear.m128_f32[0] > tfar.m128_f32[0]) swap(&tnear.m128_f32[0], &tfar.m128_f32[0], 4);
            if (tnear.m128_f32[1] > tfar.m128_f32[1]) swap(&tnear.m128_f32[1], &tfar.m128_f32[1], 4);
            if (tnear.m128_f32[2] > tfar.m128_f32[2]) swap(&tnear.m128_f32[2], &tfar.m128_f32[2], 4);

            if (tnear.m128_f32[0] > tfar.m128_f32[2] || tnear.m128_f32[2] > tfar.m128_f32[0])
                continue;

            float t_near = tnear.m128_f32[0] > tnear.m128_f32[2] ? tnear.m128_f32[0] : tnear.m128_f32[2];
            float t_far = tfar.m128_f32[0] < tfar.m128_f32[2] ? tfar.m128_f32[0] : tfar.m128_f32[2];

            /* ##################### Y ############################ */
            if (t_near > tfar.m128_f32[1] || tnear.m128_f32[1] > t_far)
                continue;

            if (tnear.m128_f32[1] > t_near)
                t_near = tnear.m128_f32[1];
            if (tfar.m128_f32[1] < t_far)
                t_far = tfar.m128_f32[1];
            /* ##################### Y ############################ */

            if (((t_far < 0) || (t_near < 0)) || (t_near > 1.f))
                continue;

            vec4 normal = { 0.f };
            if (tnear.m128_f32[0] >= tnear.m128_f32[1] && tnear.m128_f32[0] >= tnear.m128_f32[2]) {
                if (m->rigid.velocity.m128_f32[0] < 0) {
                    normal.m128_f32[0] = 1.f;
                } else if (m->rigid.velocity.m128_f32[0] > 0) {
                    normal.m128_f32[0] = -1.f;
                }
            } else if (tnear.m128_f32[1] >= tnear.m128_f32[0] && tnear.m128_f32[1] >= tnear.m128_f32[2]) {
                if (m->rigid.velocity.m128_f32[1] < 0) {
                    normal.m128_f32[1] = 1.f;
                } else if (m->rigid.velocity.m128_f32[1] > 0) {
                    normal.m128_f32[1] = -1.f;
                }
            } else if (tnear.m128_f32[2] >= tnear.m128_f32[0] && tnear.m128_f32[2] >= tnear.m128_f32[1]) {
                if (m->rigid.velocity.m128_f32[2] < 0) {
                    normal.m128_f32[2] = 1.f;
                } else if (m->rigid.velocity.m128_f32[2] > 0) {
                    normal.m128_f32[2] = -1.f;
                }
            }

            if (t_near == 0.f) {
                printf("Sliding.... %f\n", t_near);

                float dot = dotProduct(m->rigid.velocity, normal);
                m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, dot));

                //if (tnear.m128_f32[1] == 0) {
                //    //obj->falling_time = 0.f;
                //    //m->rigid.velocity = vecMulvec(m->rigid.velocity,  m->rigid.momentum);
                //}
                //else {
                //    m->rigid.velocity = (gravity_epicenter * (98.1f * (m->rigid.falling_time * 2.f))) + (m->rigid.velocity * m->rigid.momentum);
                //}
                return 2;

            } else if (((t_near > 0.f) && (t_near <= 1.f))) {
                printf("COLLISION  ######################### : %f    mesh.id: %d\n", t_near, pk);

                m->rigid.velocity = vecMulf32(m->rigid.velocity, t_near);

                mat4x4 trans = translationMatrix(m->rigid.velocity.m128_f32[0], m->rigid.velocity.m128_f32[1], m->rigid.velocity.m128_f32[2]);

                setvec4arrayMulmat(m->coords.v, 4, trans);
                setfacearrayMulmat(m->rigid.f, m->rigid.f_indexes, trans);

                //obj->momentum *= cache->fr_coef;
                //float dot = dot_product(normal, obj->mvdir);
                //obj->mvdir = obj->mvdir - (dot * normal);
                //obj->velocity = (obj->mvdir * obj->momentum);
                float col_dot = dotProduct(m->rigid.velocity, normal);
                m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, col_dot));

                return 1;
            }
        }
    }
    return 0;
}
#endif // VECTORIZED_CODE #######################################################################################
/* Check for collisions between rotated meshes. */
const int staticOBBCollision(mesh *m) {
    /* Implement Oriented bounding boxes collision detection. */
    mat4x4 tm = translationMatrix(vec4ExtractX(m->rigid.velocity), vec4ExtractY(m->rigid.velocity), vec4ExtractZ(m->rigid.velocity));
    face *temp1 = facearrayMulmat(m->rigid.f, m->rigid.f_indexes, tm);

    float depth = (float)INT_MAX;
    vec4 normal = { 0 };
    float dot = 0.f, min_outer,  min_inner, max_outer, max_inner;
    for (int i = 0; i < m->rigid.f_indexes; i++) {
        min_outer = (float)INT_MAX, min_inner = (float)INT_MAX, max_outer = (float)-INT_MAX, max_inner = (float)-INT_MAX;
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
            free(temp1);
            return 0;
        }

        float da = (max_outer - min_inner);
        float db = (max_inner - min_outer);
        float axisDepth = da < db ? da : db;
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = temp1[i].vn[0];
        }
    }

    for (int i = 0; i < SCENE.mesh[3].rigid.f_indexes; i++) {
        min_outer = (float)INT_MAX, min_inner = (float)INT_MAX, max_outer = (float)-INT_MAX, max_inner = (float)-INT_MAX;
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
            free(temp1);
            return 0;
        }

        float da = (max_outer - min_inner);
        float db = (max_inner - min_outer);
        float axisDepth = da < db ? da : db;
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = SCENE.mesh[3].rigid.f[i].vn[0];
        }
    }

    depth /= vecLength(normal);
    if (dotProduct(vecSubvec(m->coords.v[0], SCENE.mesh[3].coords.v[0]), normal) < 0) {
        normal = vecMulf32(normal, -1.f);
    }
    normal = vecNormalize(normal);

    float col_dot = dotProduct(m->rigid.velocity, normal);
    m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, col_dot));
    //logvec4(normal);
    //printf("Collision Detected...depth: %f\n", depth);

    //memcpy(m->rigid.f, temp1, m->rigid.f_indexes * sizeof(face));
    free(temp1);
    return 1;
}


