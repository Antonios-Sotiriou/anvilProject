#include "headers/components/animations.h"

static int COUNT = 0;
static int f_index = 0;
float rot = 1.f;

void loadModelAnimations(model* m) {
    int path_length = (strlen(m->cname) * 2) + strlen(anvil_SOURCE_DIR) + 36; // Plus 1 here for the null termination \0.
    char* dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "char *dynamic_path = malloc(path_length)");
        debug_log_info(stdout, "%s\n", m->cname);
        return;
    }
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sprintf_s(dynamic_path, path_length, "%s/models/%s/animations/%s_animations.txt", anvil_SOURCE_DIR, m->cname, m->cname);
#elif defined(LINUX) || defined(__linux__)
    snprintf(dynamic_path, path_length, "%s/models/%s/animations/%s_animations.txt", anvil_SOURCE_DIR, m->cname, m->cname);
#endif

    animTextData ad = { 0 };
    readAnimText(&ad, dynamic_path);
    free(dynamic_path);

    const int gm_size = 16 * ad.number_of_frames;

    /* Load Model animations. May will change in the future to load model and meshes, which belong to the model in one iteration. */
    m->anim.frames = ad.number_of_frames;
    for (int i = 0; i < ad.number_of_objects; i++) {
        if (strncmp(m->cname, ad.object[i].cname, strlen(m->cname)) == 0) {

            m->anim.lc = malloc(gm_size);
            m->anim.rq = malloc(gm_size);
            m->anim.sc = malloc(gm_size);
            m->anim.anim_matrix = identityMatrix();

            memcpy(m->anim.lc, ad.object[i].location, gm_size);
            memcpy(m->anim.rq, ad.object[i].rotation_quaternion, gm_size);
            memcpy(m->anim.sc, ad.object[i].scale, gm_size);
        }
    }
    /* Load model's meshes animations and assign children parent relations. 
    For relations we could also use the relations.c file but with assigning, we save us the function calls. */
    for (int x = 0; x < m->mesh_indexes; x++) {

        m->mesh[x].anim.lc = malloc(gm_size);
        m->mesh[x].anim.rq = malloc(gm_size);
        m->mesh[x].anim.sc = malloc(gm_size);
        m->anim.anim_matrix = identityMatrix();

        for (int y = 0; y < ad.number_of_objects; y++) {

            if (strncmp(m->mesh[x].cname, ad.object[y].cname, strlen(m->mesh[x].cname)) == 0) {
                memcpy(m->mesh[x].anim.lc, ad.object[y].location, gm_size);
                memcpy(m->mesh[x].anim.rq, ad.object[y].rotation_quaternion, gm_size);
                memcpy(m->mesh[x].anim.sc, ad.object[y].scale, gm_size);

                if (ad.object[y].number_of_children) {
                    m->mesh[x].number_of_children = ad.object[y].number_of_children;
                    m->mesh[x].children = malloc(8 * ad.object[y].number_of_children);

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
	releaseAnimText(&ad);
}
void applyRecursiveTranformation(mesh *m, mat4x4 *mat) {
    vec4 lc, sc;
    quat rq;
    if (m->number_of_children) {
        for (int i = 0; i < m->number_of_children; i++) {
            lc = m->children[i]->anim.lc[f_index];
            rq = m->children[i]->anim.rq[f_index];
            sc = m->children[i]->anim.sc[f_index];
            m->children[i]->anim.anim_matrix = modelMatfromQST(rq, sc, lc);
            m->anim.anim_matrix = matMulmat(*mat, m->children[i]->anim.anim_matrix);
            applyRecursiveTranformation(m->children[i], mat);
        }
    }
}
int applyReverseTranformation(mesh *m, mat4x4 *mat) {
    if (m->parent) {
        //printf("%s ", m->parent->cname);
        vec4 lc = m->parent->anim.lc[f_index];
        quat rq = m->parent->anim.rq[f_index];
        vec4 sc = m->parent->anim.sc[f_index];
        //m->parent->anim.anim_matrix = modelMatfromQST(rq, sc, lc);
        *mat = matMulmat(*mat, modelMatfromQST(rq, sc, lc));
        //m->anim.anim_matrix = *mat;
        applyReverseTranformation(m->parent, mat);
        //m->anim.anim_matrix = *mat;
    }
}
void animateModels(void) {
    if ((COUNT % 500) == 0) {
        f_index += 1;

        if (f_index > 24)
            f_index = 0;
        rot += 0.5f;
    }
    COUNT++;

    for (int i = 0; i < SCENE.model_indexes; i++) {
        if (SCENE.model[i].visible) {
            if (SCENE.model[i].owns_anim) {
                vec4 lc = SCENE.model[i].anim.lc[f_index];
                quat rq = SCENE.model[i].anim.rq[f_index];
                vec4 sc = SCENE.model[i].anim.sc[f_index];
                SCENE.model[i].anim.anim_matrix = modelMatfromQST(rq, sc, lc);
                //printf("model: %s", SCENE.model[i].cname);
                for (int x = 0; x < SCENE.model[i].mesh_indexes; x++) {
                    //printf("\nmesh: %s\n chain: ", SCENE.model[i].mesh[x].cname);
                    //if (SCENE.model[i].mesh[x].parent) {
                    //if (strncmp(SCENE.model[i].mesh[x].cname, "hand.l", 6) == 0) {
                        vec4 lc = SCENE.model[i].mesh[x].anim.lc[f_index];
                        quat rq = SCENE.model[i].mesh[x].anim.rq[f_index];
                        vec4 sc = SCENE.model[i].mesh[x].anim.sc[f_index];
                        SCENE.model[i].mesh[x].anim.anim_matrix = modelMatfromQST(rq, sc, lc);
                        applyReverseTranformation(&SCENE.model[i].mesh[x], &SCENE.model[i].mesh[x].anim.anim_matrix);
                    //}
                }
                //printf("\n");
            }
        }
    }
    //exit(0);
    //for (int i = 0; i < SCENE.model_indexes; i++) {
    //    if (SCENE.model[i].visible) {
    //        if (SCENE.model[i].owns_anim) {
    //            vec4 lc = SCENE.model[i].anim.lc[f_index];
    //            quat rq = SCENE.model[i].anim.rq[f_index];
    //            vec4 sc = SCENE.model[i].anim.sc[f_index];

    //            for (int x = 0; x < SCENE.model[i].mesh_indexes; x++) {
    //                vec4 lc = SCENE.model[i].mesh[x].anim.lc[f_index];
    //                quat rq = SCENE.model[i].mesh[x].anim.rq[f_index];
    //                vec4 sc = SCENE.model[i].mesh[x].anim.sc[f_index];
    //                SCENE.model[i].mesh[x].anim.anim_matrix = modelMatfromQST(rq, sc, lc);
    //                applyRecursiveTranformation(&SCENE.model[i].mesh[x], &SCENE.model[i].mesh[x].anim.anim_matrix);
    //            }
    //        }
    //    }
    //}

    //for (int i = 0; i < SCENE.model_indexes; i++) {
    //    if (SCENE.model[i].visible) {
    //        if (SCENE.model[i].owns_anim) {
    //            for (int x = 0; x < SCENE.model[i].mesh_indexes; x++) {
    //                if (strncmp(SCENE.model[i].mesh[x].cname, "hand.l", 6) == 0) {

    //                    vec4 lc = SCENE.model[i].mesh[x].anim.lc[f_index];
    //                    quat rq = SCENE.model[i].mesh[x].anim.rq[f_index];
    //                    vec4 sc = SCENE.model[i].mesh[x].anim.sc[f_index];
    //                    SCENE.model[i].mesh[x].anim.anim_matrix = modelMatfromQST(rq, sc, lc);

    //                    if (SCENE.model[i].mesh[x].parent) {
    //                        lc = SCENE.model[i].mesh[x].parent->anim.lc[f_index];
    //                        rq = SCENE.model[i].mesh[x].parent->anim.rq[f_index];
    //                        sc = SCENE.model[i].mesh[x].parent->anim.sc[f_index];
    //                        SCENE.model[i].mesh[x].anim.anim_matrix = matMulmat(SCENE.model[i].mesh[x].anim.anim_matrix, modelMatfromQST(rq, sc, lc));

    //                        if (SCENE.model[i].mesh[x].parent->parent) {
    //                            lc = SCENE.model[i].mesh[x].parent->parent->anim.lc[f_index];
    //                            rq = SCENE.model[i].mesh[x].parent->parent->anim.rq[f_index];
    //                            sc = SCENE.model[i].mesh[x].parent->parent->anim.sc[f_index];
    //                            SCENE.model[i].mesh[x].anim.anim_matrix = matMulmat(SCENE.model[i].mesh[x].anim.anim_matrix, modelMatfromQST(rq, sc, lc));

    //                            if (SCENE.model[i].mesh[x].parent->parent->parent) {

    //                                lc = SCENE.model[i].mesh[x].parent->parent->parent->anim.lc[f_index];
    //                                rq = SCENE.model[i].mesh[x].parent->parent->parent->anim.rq[f_index];
    //                                sc = SCENE.model[i].mesh[x].parent->parent->parent->anim.sc[f_index];
    //                                SCENE.model[i].mesh[x].anim.anim_matrix = matMulmat(SCENE.model[i].mesh[x].anim.anim_matrix, modelMatfromQST(rq, sc, lc));
    //                            }
    //                        }
    //                    }
    //                }
    //            }

    //        }
    //    }
    //}

    for (int i = 0; i < SCENE.model_indexes; i++) {
        if (SCENE.model[i].visible) {
            if (SCENE.model[i].owns_anim) {

                SCENE.model[i].model_matrix = matMulmat(SCENE.model[i].anim.anim_matrix, modelMatfromQST(SCENE.model[i].q, SCENE.model[i].scale, SCENE.model[i].coords.v[0]));
                for (int x = 0; x < SCENE.model[i].mesh_indexes; x++) {

                    SCENE.model[i].mesh[x].model_matrix = matMulmat(SCENE.model[i].mesh[x].anim.anim_matrix, modelMatfromQST(SCENE.model[i].mesh[x].q, SCENE.model[i].mesh[x].scale, SCENE.model[i].mesh[x].coords.v[0]));
                }
            } else {
                SCENE.model[i].model_matrix = modelMatfromQST(SCENE.model[i].q, SCENE.model[i].scale, SCENE.model[i].coords.v[0]);
            }
        }
    }
}
void releaseAnimations(animation *an) {
    free(an->lc);
    free(an->rq);
    free(an->sc);
}


