#include "headers/components/animations.h"

static int COUNT = 0;
static int f_index = 0;
float rot = 0.f;

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
    }
	releaseAnimText(&ad);
}
//void applyRecursiveTranformation(mesh *m, mat4x4 *mat) {
//    vec4 lc, sc;
//    quat rq;
//    if (m->number_of_children) {
//        for (int i = 0; i < m->number_of_children; i++) {
//            lc = m->children[i]->anim.lc[f_index];
//            rq = m->children[i]->anim.rq[f_index];
//            sc = m->children[i]->anim.sc[f_index];
//            *mat = matMulMat(m->children[i]->anim.bm[f_index], *mat);
//            //*mat = matMulMat(modelMatFromQST(rq, sc, lc), *mat);
//            applyRecursiveTranformation(m->children[i], mat);
//        }
//    }
//}
//int applyReverseTranformation(mesh *m, mat4x4 *mat) {
//    if (m->parent) {
//        vec4 lc = m->parent->anim.lc[f_index];
//        quat rq = m->parent->anim.rq[f_index];
//        vec4 sc = m->parent->anim.sc[f_index];
//        *mat = matMulMat(m->parent->anim.bm[f_index], *mat);
//        //*mat = matMulMat(modelMatFromQST(rq, sc, lc), *mat);
//        applyReverseTranformation(m->parent, mat);
//    }
//}
void animateModels(scene *s) {

    for (int i = 0; i < s->model_indexes; i++) {
        if (s->model[i].visible) {
            if (s->model[i].owns_anim) {

                if ((COUNT % 100) == 0) {
                    f_index += 1;

                    if (f_index > s->model[i].anim.frames)
                        f_index = 0;
                    rot += 10.0f;
                }
                COUNT++;

                vec4 lc = s->model[i].anim.lc[f_index];
                quat rq = s->model[i].anim.rq[f_index];
                vec4 sc = s->model[i].anim.sc[f_index];

                s->model[i].anim.anim_matrix = modelMatFromQST(rq, sc, lc);
                // s->model[i].anim.anim_matrix = s->model[i].anim.bm[f_index];

                if (s->model[i].mesh_indexes > 1) {
                    for (int x = 0; x < s->model[i].mesh_indexes; x++) {

                        vec4 lc = s->model[i].mesh[x].anim.lc[f_index];
                        quat rq = s->model[i].mesh[x].anim.rq[f_index];
                        vec4 sc = s->model[i].mesh[x].anim.sc[f_index];

                        s->model[i].mesh[x].anim.anim_matrix = modelMatFromQST(rq, sc, lc);
                        // s->model[i].mesh[x].anim.anim_matrix = s->model[i].mesh[x].anim.bm[f_index];
                    }
                }
            }
        }
    }

    for (int i = 0; i < s->model_indexes; i++) {
        if (s->model[i].visible) {
            if (s->model[i].owns_anim) {

                s->model[i].model_matrix = matMulMat(s->model[i].anim.anim_matrix, modelMatFromQST(s->model[i].q, s->model[i].scale, s->model[i].coords.v[0]));
                for (int x = 0; x < s->model[i].mesh_indexes; x++) {

                    s->model[i].mesh[x].model_matrix = matMulMat(s->model[i].mesh[x].anim.anim_matrix, modelMatFromQST(s->model[i].mesh[x].q, s->model[i].mesh[x].scale, s->model[i].mesh[x].coords.v[0]));
                }
            } else {
                s->model[i].model_matrix = modelMatFromQST(s->model[i].q, s->model[i].scale, s->model[i].coords.v[0]);
            }
        }
    }
}
void releaseAnimations(animation *an) {
    free(an->lc);
    free(an->rq);
    free(an->sc);
    free(an->bm);
}


