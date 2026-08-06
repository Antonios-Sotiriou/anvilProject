#include "headers/components/animations.h"
#include "headers/cmake_variables.h"
#include "headers/global.h"
#include "headers/flags.h"
#include "headers/components/animText.h"
#include "headers/components/internal_libraries/matrices.h"
#include "headers/components/internal_libraries/quaternions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static float getAnimationTime(animation *an, const float FPS);

void loadModelAnimations(model* m) {
    int path_length = (strlen(m->cname) * 2) + strlen(anvil_SOURCE_DIR) + 36; // Plus 1 here for the null termination \0.
    char* dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "char *dynamic_path = malloc(path_length)");
        debug_log_info(stdout, "%s\n", m->cname);
        return;
    }

    anvil_snprintf(dynamic_path, path_length, "%s/models/%s/animations/%s_animations.txt", anvil_SOURCE_DIR, m->cname, m->cname);

    animTextData ad = { 0 };
    readAnimText(&ad, dynamic_path);
    free(dynamic_path);

    const int vec_size = 16 * ad.number_of_frames;
    const int mat_size = 64 * ad.number_of_frames;

    /* Load Model animations. May will change in the future to load model and meshes, which belong to the model in one iteration. */
    m->anim.frames = ad.number_of_frames - 1;  // Minus 1 here because frames start from zero when we iterate them.
 
    for (int i = 0; i < ad.number_of_objects; i++) {
        if (strncmp(m->cname, ad.object[i].cname, strlen(m->cname)) == 0) {

            m->anim.lc = malloc(vec_size);
            m->anim.rq = malloc(vec_size);
            m->anim.sc = malloc(vec_size);
            m->anim.bm = malloc(mat_size);
            m->anim.anim_matrix = identityMatrix();

            memcpy(m->anim.lc, ad.object[i].location, vec_size);
            memcpy(m->anim.rq, ad.object[i].rotation_quaternion, vec_size);
            memcpy(m->anim.sc, ad.object[i].scale, vec_size);
            memcpy(m->anim.bm, ad.object[i].bone_matrix, mat_size);
        }
    }
    if (m->mesh_indexes == 1) {
        m->mesh[0].anim.anim_matrix = identityMatrix();
    } else {
        /* Load model's meshes animations and assign children parent relations. 
        For relations we could also use the relations.c file but with assigning, we save us the function calls. */
        for (int x = 0; x < m->mesh_indexes; x++) {

            m->mesh[x].anim.lc = malloc(vec_size);
            m->mesh[x].anim.rq = malloc(vec_size);
            m->mesh[x].anim.sc = malloc(vec_size);
            m->mesh[x].anim.bm = malloc(mat_size);
            m->mesh[x].anim.anim_matrix = identityMatrix();

            for (int y = 0; y < ad.number_of_objects; y++) {

                if (strncmp(m->mesh[x].cname, ad.object[y].cname, strlen(m->mesh[x].cname)) == 0) {
                    memcpy(m->mesh[x].anim.lc, ad.object[y].location, vec_size);
                    memcpy(m->mesh[x].anim.rq, ad.object[y].rotation_quaternion, vec_size);
                    memcpy(m->mesh[x].anim.sc, ad.object[y].scale, vec_size);
                    memcpy(m->mesh[x].anim.bm, ad.object[y].bone_matrix, mat_size);

                    if (ad.object[y].number_of_children) {
                        m->mesh[x].number_of_children = ad.object[y].number_of_children;
                        m->mesh[x].children = malloc(sizeof(double) * ad.object[y].number_of_children);

                        for (int c = 0; c < ad.object[y].number_of_children; c++) {
                            for (int z = 0; z < m->mesh_indexes; z++) {

                                if (strncmp(m->mesh[z].cname, ad.object[y].children[c], strlen(m->mesh[z].cname)) == 0) {
                                    m->mesh[x].children[c] = &m->mesh[z];
                                    m->mesh[z].parent = &m->mesh[x];
                                    //addChild(m->mesh[x], m->mesh[z]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
	releaseAnimText(&ad);
}
void animateModels(scene *s) {

    for (int i = 0; i < s->model_indexes; i++) {
        //if (s->model[i].visible) {
            if (s->model[i].owns_anim) {

                if (s->model[i].anim.curr_frame == (s->model[i].anim.frames - 1)) {
                    s->model[i].anim.next_frame = 0;
                }

                if (s->model[i].anim.curr_frame >= s->model[i].anim.frames) {
                    s->model[i].anim.curr_frame = 0;
                    s->model[i].anim.next_frame = 1;
                }

                vec4 lc = lerp(s->model[i].anim.lc[s->model[i].anim.curr_frame], s->model[i].anim.lc[s->model[i].anim.next_frame], s->model[i].anim.frame_t);
                quat rq = slerp(s->model[i].anim.rq[s->model[i].anim.curr_frame], s->model[i].anim.rq[s->model[i].anim.next_frame], s->model[i].anim.frame_t);
                vec4 sc = lerp(s->model[i].anim.sc[s->model[i].anim.curr_frame], s->model[i].anim.sc[s->model[i].anim.next_frame], s->model[i].anim.frame_t);

                s->model[i].anim.anim_matrix = modelMatFromQST(rq, sc, lc);
                // s->model[i].anim.anim_matrix = s->model[i].anim.bm[curr_frame];
                //s->model[i].model_matrix = matMulMat(s->model[i].anim.anim_matrix, modelMatFromQST(s->model[i].q, s->model[i].scale, s->model[i].coords.v[0]));  // to be removed!

                if (s->model[i].mesh_indexes > 1) {
                    for (int x = 0; x < s->model[i].mesh_indexes; x++) {

                        vec4 lc = lerp(s->model[i].mesh[x].anim.lc[s->model[i].anim.curr_frame], s->model[i].mesh[x].anim.lc[s->model[i].anim.next_frame], s->model[i].anim.frame_t);
                        quat rq = slerp(s->model[i].mesh[x].anim.rq[s->model[i].anim.curr_frame], s->model[i].mesh[x].anim.rq[s->model[i].anim.next_frame], s->model[i].anim.frame_t);
                        vec4 sc = lerp(s->model[i].mesh[x].anim.sc[s->model[i].anim.curr_frame], s->model[i].mesh[x].anim.sc[s->model[i].anim.next_frame], s->model[i].anim.frame_t);

                        s->model[i].mesh[x].anim.anim_matrix = modelMatFromQST(rq, sc, lc);
                        // s->model[i].mesh[x].anim.anim_matrix = s->model[i].mesh[x].anim.bm[curr_frame];

                        s->model[i].mesh[x].model_matrix = matMulMat(s->model[i].mesh[x].anim.anim_matrix, modelMatFromQST(s->model[i].mesh[x].q, s->model[i].mesh[x].scale, s->model[i].mesh[x].coords.v[0]));
                    }
                } else {
                    s->model[i].model_matrix = matMulMat(s->model[i].anim.anim_matrix, modelMatFromQST(s->model[i].q, s->model[i].scale, s->model[i].coords.v[0]));
                }

                if (s->model[i].anim.frame_t >= 1) {
                    s->model[i].anim.curr_frame++;
                    s->model[i].anim.next_frame = s->model[i].anim.curr_frame + 1;
                    s->model[i].anim.frame_t = 0.f;
                }

                s->model[i].anim.frame_t += getAnimationTime(&s->model[i].anim, s->mtr.FPS);
            }
    }
}
float getAnimationTime(animation *an, const float FPS) {
    return 0.05f; // 1.f / (FPS / an->frames);
}
void releaseAnimations(animation *an) {
    free(an->lc);
    free(an->rq);
    free(an->sc);
    free(an->bm);
}
