#include "headers/components/collisions.h"

extern void swap(void *a, void *b, unsigned long size);
/* For testing purposes. Return t time of collision. */
float planeIntersect(vec4 plane, vec4 point, vec4 line_start, vec4 line_end) {
    float plane_d = -dotProduct(plane, point);
    float ad = dotProduct(line_start, plane);
    float bd = dotProduct(line_end, plane);
    float t = ((-plane_d - ad) / (bd - ad));
    return t;
}
/* Return signed shortest distance from point to plane, plane normal must be normalised. */
const float planeDistance(vec4 plane, vec4 v) {
    vec4 r = vecMulvec(plane, v);
    return ((vec4ExtractX(r) + vec4ExtractY(r) + vec4ExtractZ(r)) - dotProduct(plane, vecNormalize(plane)));
}

void meshTerrainCollision(mesh *m) {
    mat4x4 tm;
    vec4 pos, normal;
    getmeshPositionData(m, &pos, &normal);

    float height_diff = -vec4ExtractY(pos) - (vec4ExtractY(m->coords.v[0]) - m->scale);
    if (height_diff >= 0) {
        m->rigid.grounded = 1;
        m->rigid.falling_time = 0;
    }
    if (m->rigid.grounded) {
        tm = translationMatrix(0, height_diff, 0);

        setvec4arrayMulmat(m->coords.v, 4, tm);
        setvec4arrayMulmat(m->rigid.v, m->rigid.v_indexes, tm);
        setvec4arrayMulmat(m->rigid.n, m->rigid.n_indexes, tm);
    }
}
//const void terrainHeightDifference(Mesh* terrain, Mesh* obj) {
//    vec4f next_pos = obj->cd.v[P] + obj->velocity + (obj->mvdir * obj->scale);
//    // displayPoint(next_pos, worldMatrix, 0xffccaa);
//    TerrainPointInfo tp_0 = getTerrainPointData(terrain, obj->cd.v[0], obj);
//    TerrainPointInfo tp_1 = getTerrainPointData(terrain, next_pos, obj);
//
//    float height_0 = tp_0.pos[1] - (obj->cd.v[P][1] - obj->scale);
//    float height_1 = tp_1.pos[1] - (next_pos[1] - obj->scale);
//
//    if ((obj->grounded) && (height_1 - height_0 > 50)) {
//        obj->velocity -= obj->velocity;
//    }
//}
/* Checks for collisions whithin a radius sourounding the mesh. */
const int staticOuterRadiusCollision(mesh *m) {
    //vec4 newPos = vecAddvec(m->coords.v[0], m->rigid.velocity);
    int pk;
    for (int i = 0; i < SCENE.t.quad[m->quad_index].mpks_indexes; i++) {
        pk = SCENE.t.quad[m->quad_index].mpks[i];
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

    if (m->quad_index < 0) {
        fprintf(stderr, "obj->quadIndex : %d. Out of Terrain. ObjectEnvironmentCollision().\n", m->quad_index);
        return 0;
    }

    getmeshRigidLimits(m);
    vec4 tnear, tfar, min, max;

    const int num_of_members = SCENE.t.quad[m->quad_index].mpks_indexes;

    for (int i = 0; i < num_of_members; i++) {

        int pk = SCENE.t.quad[m->quad_index].mpks[i];

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
                //printf("COLLISION  ######################### : %f    mesh.id: %d\n", t_near, pk);

                m->rigid.velocity = vecMulf32(m->rigid.velocity, t_near);

                mat4x4 trans = translationMatrix(vec4ExtractX(m->rigid.velocity), vec4ExtractY(m->rigid.velocity), vec4ExtractZ(m->rigid.velocity));

                setvec4arrayMulmat(m->coords.v, 4, trans);
                setvec4arrayMulmat(m->rigid.v, m->rigid.v_indexes, trans);
                setvec4arrayMulmat(m->rigid.n, m->rigid.n_indexes, trans);

                //obj->momentum *= cache->fr_coef;
                //float dot = dot_product(normal, obj->mvdir);
                //obj->mvdir = obj->mvdir - (dot * normal);
                //obj->velocity = (obj->mvdir * obj->momentum);
                float col_dot = dotProduct(m->rigid.velocity, normal);
                m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, col_dot));
                printf("Collision Detected...depth: %f    col_dot: %f\n", t_near, col_dot);
                return 1;
            }
        }
    }
    return 0;
}
#else // ITERATIVE_CODE #########################################################################################
/* Check swept Axis Aligned Bounding Boxes collisions between, given mesh (*m) and a Primary keys array of possible colliders (pks). */
const int sweptAABBCollision(mesh *m, const int pks[]) {

    if (m->quad_index < 0) {
        fprintf(stderr, "obj->quadIndex : %d. Out of Terrain. ObjectEnvironmentCollision().\n", m->quad_index);
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
                setvec4arrayMulmat(m->rigid.v, m->rigid.v_indexes, trans);
                setvec4arrayMulmat(m->rigid.n, m->rigid.n_indexes, trans);

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
const int staticOBBCollision(mesh *m, const int pk) {

    if (m->quad_index < 0) {
        fprintf(stderr, "obj->quadIndex : %d. Out of Terrain. staticOBBCollisions().\n", m->quad_index);
        return 0;
    }

    const int sum_norms = m->rigid.n_indexes + SCENE.mesh[pk].rigid.n_indexes;
    /* Implement Oriented bounding boxes collision detection. */
    mat4x4 tm = translationMatrix(vec4ExtractX(m->rigid.velocity), vec4ExtractY(m->rigid.velocity), vec4ExtractZ(m->rigid.velocity));
    vec4 *vec4s = vec4arrayMulmat(m->rigid.v, m->rigid.v_indexes, tm);
    vec4 *norms = vec4arrayMulmat(m->rigid.n, m->rigid.n_indexes, tm);
    vec4 *temp = realloc(norms, 16 * sum_norms);;
    if (!temp) {
        debug_log_error(stdout, "malloc()");
        free(vec4s);
        free(norms);
        return 0;
    }
    norms = temp;
    memcpy(&norms[m->rigid.n_indexes], SCENE.mesh[pk].rigid.n, 16 * SCENE.mesh[pk].rigid.n_indexes);

    float depth = (float)INT_MAX;
    vec4 normal = { 0 };
    float dot = 0.f, min_outer,  min_inner, max_outer, max_inner;

    //printf("\x1b[H\x1b[J");

    for (int i = 0; i < sum_norms; i++) {
        min_outer = (float)INT_MAX, min_inner = (float)INT_MAX, max_outer = (float)-INT_MAX, max_inner = (float)-INT_MAX;
        for (int x = 0; x < m->rigid.v_indexes; x++) {
            min_outer = (dot = dotProduct(norms[i], vec4s[x])) < min_outer ? dot : min_outer;
            max_outer = dot > max_outer ? dot : max_outer;
        }

        for (int y = 0; y < SCENE.mesh[pk].rigid.v_indexes; y++) {
            min_inner = (dot = dotProduct(norms[i], SCENE.mesh[pk].rigid.v[y])) < min_inner ? dot : min_inner;
            max_inner = dot > max_inner ? dot : max_inner;
        }

        if ((min_outer > max_inner) || (max_outer < min_inner)) {
            free(vec4s);
            free(norms);
            return 0;
        }

        float da = (max_outer - min_inner);
        float db = (max_inner - min_outer);
        float axisDepth = da < db ? da : db;
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = norms[i];
        }
    }

    depth = vecLength(m->rigid.velocity);
    //if (dotProduct(vecSubvec(m->coords.v[0], SCENE.mesh[pk].coords.v[0]), normal) < 0) {
    //    normal = vecMulf32(normal, -1.f);
    //}
    //normal = vecNormalize(normal);

    //printf("depth_normal: %f\n", depth / vecLength(normal));
    //printf("depth_velocity: %f\n", depth / vecLength(m->rigid.velocity));
    if (depth == 0)
        printf("Sliding... depth_velocity: %f\n", depth);
    if (depth > 0 && depth <= 1.f) {
        vec4 velocity = vecMulf32(m->rigid.velocity, -depth);
        logvec4(m->rigid.velocity);
        logvec4(velocity);
        mat4x4 trans = translationMatrix(vec4ExtractX(velocity), vec4ExtractY(velocity), vec4ExtractZ(velocity));
        setvec4arrayMulmat(m->coords.v, 4, trans);
        setvec4arrayMulmat(m->rigid.v, m->rigid.v_indexes, trans);
        setvec4arrayMulmat(m->rigid.n, m->rigid.n_indexes, trans);
    }
    float col_dot = dotProduct(m->rigid.velocity, normal);
    m->rigid.velocity = vecSubvec(m->rigid.velocity, vecMulf32(normal, col_dot));

    //printf("t_min: %f    t_max: %f    t_near: %f\n", t_min, t_max, t_near);
    //printf("Collision Detected...depth: %f    col_dot: %f\n", depth, col_dot);
    printf("depth_velocity: %f\n", depth);
    //printf("Collision Detected...\n");

    free(vec4s);
    free(norms);
    //getc(stdin);
    return 1;
}
/* Swept Sorting the collisions for AABB. */
void sortCollisions(mesh *m) {

    if (m->quad_index < 0) {
        fprintf(stderr, "obj->quadIndex : %d. Out of Terrain. sortCollisions().\n", m->quad_index);
        return;
    }

    getmeshRigidLimits(m);
    vec4 tnear, tfar, min, max;

    const int num_of_members = SCENE.t.quad[m->quad_index].mpks_indexes;

    for (int i = 0; i < num_of_members; i++) {

        int pk = SCENE.t.quad[m->quad_index].mpks[i];

        if (pk != m->pk) {

            min = roundvec4(vecSubvec(SCENE.mesh[pk].rigid.min, vecSubvec(m->coords.v[0], m->rigid.min)));
            max = roundvec4(vecSubvec(SCENE.mesh[pk].rigid.max, vecSubvec(m->coords.v[0], m->rigid.max)));

            tnear = vecDivvec(vecSubvec(min, m->coords.v[0]), m->rigid.velocity);
            tfar = vecDivvec(vecSubvec(max, m->coords.v[0]), m->rigid.velocity);

            if (vecEqualvec(tnear, tnear) || vecEqualvec(tfar, tfar))
                continue;

            min = _mm_min_ps(tnear, tfar);
            max = _mm_max_ps(tnear, tfar);

            int check_xz = _mm_movemask_ps(_mm_cmpgt_ps(min, _mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 0, 1, 2))));
            if (check_xz == 1 || check_xz == 4) {
                continue;
            }

            float t_near = 0.f, t_far = 0.f;
            if (_mm_movemask_ps(_mm_cmpgt_ps(min, _mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 2)))) == 1) {
                t_near = _mm_cvtss_f32(min);
            }
            else {
                t_near = _mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 2)));
            }

            if (_mm_movemask_ps(_mm_cmplt_ps(max, _mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 2)))) == 1) {
                t_far = _mm_cvtss_f32(max);
            }
            else {
                t_far = _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 2)));
            }

            /* ##################### Y ############################ */
            if ((t_near > _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 1)))) || (_mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 1))) > t_far))
                continue;

            if (_mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 1))) > t_near)
                t_near = _mm_cvtss_f32(_mm_shuffle_ps(min, min, _MM_SHUFFLE(3, 2, 1, 1)));
            if (_mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 1))) > t_far)
                t_far = _mm_cvtss_f32(_mm_shuffle_ps(max, max, _MM_SHUFFLE(3, 2, 1, 1)));
            /* ##################### Y ############################ */

            if (((t_far < 0) || (t_near < 0)) || (t_near > 1.f))
                continue;

            SCENE.mesh[pk].rigid.collision_t = t_near;
        }
    }
    mesh cache_1, cache_2;
    for (int i = 0; i < num_of_members; i++) {
        int pk1 = SCENE.t.quad[m->quad_index].mpks[i];
        cache_1 = SCENE.mesh[pk1];
        for (int j = 0; j < num_of_members; j++) {
            int pk2 = SCENE.t.quad[m->quad_index].mpks[j];
            cache_2 = SCENE.mesh[pk2];
            if ( cache_1.pk != m->pk ) {
                if ( cache_1.rigid.collision_t < cache_2.rigid.collision_t ) {
                    swap(&SCENE.t.quad[m->quad_index].mpks[i], &SCENE.t.quad[m->quad_index].mpks[j], 4);
                }
            }
        }
    }
}


