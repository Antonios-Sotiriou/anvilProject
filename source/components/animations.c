#include "headers/components/animations.h"

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

        for (int y = 0; y < ad.number_of_objects; y++) {

            if (strncmp(m->mesh[x].cname, ad.object[y].cname, strlen(m->mesh[x].cname)) == 0) {
                memcpy(m->mesh[x].anim.lc, ad.object[y].location, gm_size);
                memcpy(m->mesh[x].anim.rq, ad.object[y].rotation_quaternion, gm_size);
                memcpy(m->mesh[x].anim.sc, ad.object[y].scale, gm_size);

                if (ad.object[y].number_of_children) {
                    m->mesh[x].number_of_children = ad.object[y].number_of_children;
                    m->mesh[x].children = malloc(sizeof(mesh) * ad.object[y].number_of_children);

                    for (int c = 0; c < ad.object[y].number_of_children; c++) {
                        for (int z = 0; z < m->mesh_indexes; z++) {

                            if (strncmp(m->mesh[z].cname, ad.object[y].children[c], strlen(m->mesh[z].cname)) == 0) {
                                m->mesh[x].children[c] = m->mesh[z];
                                m->mesh[z].parent = &m->mesh[x];
                            }
                        }
                    }
                }
            }
        }
    }
	releaseAnimText(&ad);
}
void releaseAnimations(animation *an) {
    free(an->lc);
    free(an->rq);
    free(an->sc);
}


