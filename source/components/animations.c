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

    m->anim.frames = ad.number_of_frames;
    for (int i = 0; i < ad.number_of_objects; i++) {
        if (strncmp(m->cname, ad.object[i].cname, strlen(m->cname)) == 0) {
            printf("Main Mesh %s\n", m->cname);
            printf("Anim Mesh %s\n", ad.object[i].cname);

            if (ad.object[i].number_of_children) {

                for (int c = 0; c < ad.object[i].number_of_children; c++) {
                    for (int z = 0; z < m->mesh_indexes; z++) {

                        if (strncmp(m->mesh[z].cname, ad.object[i].children[c], strlen(m->mesh[z].cname)) == 0) {
                            printf("Child Mesh %s\n", m->mesh[z].cname);
                        }
                    }
                }
            }
        }
    }

    for (int x = 0; x < m->mesh_indexes; x++) {

        for (int y = 0; y < ad.number_of_objects; y++) {

            if (strncmp(m->mesh[x].cname, ad.object[y].cname, strlen(m->mesh[x].cname)) == 0) {
                printf("Model Mesh %s\n", m->mesh[x].cname);
                printf("Anim Mesh %s\n", ad.object[y].cname);

                if (ad.object[y].number_of_children) {

                    for (int c = 0; c < ad.object[y].number_of_children; c++) {
                        for (int z = 0; z < m->mesh_indexes; z++) {

                            if (strncmp(m->mesh[z].cname, ad.object[y].children[c], strlen(m->mesh[z].cname)) == 0) {
                                printf("Child Mesh %s\n", m->mesh[z].cname);
                            }
                        }
                    }
                }
            }

        }
        printf("\n");

    }

	releaseAnimText(&ad);
}


