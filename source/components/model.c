#include "headers/components/model.h"

void createModel(model *m, const char name[]) {
    int path_length = (strlen(name) * 2) + 13; // Plus 1 here for the null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "char *dynamic_path = malloc(path_length)");
        debug_log_info(stdout, "%s\n", name);
        return;
    }
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sprintf_s(dynamic_path, path_length, "models/%s/%s.obj", name, name);
#elif defined(LINUX) || defined(__linux__)
    snprintf(dynamic_path, path_length, "models/%s/%s.obj", name, name);
#endif

    OBJ obj = { 0 };
    readOBJ(&obj, dynamic_path);
    free(dynamic_path);

    m->mesh = calloc(obj.e_indexes, sizeof(mesh));
    m->mesh_indexes = obj.e_indexes;
    for (int i = 0; i < m->mesh_indexes; i++) {
        /* Creating the meshes from ENTRIES of obj file. */
        createMesh(&m->mesh[i], obj.e[i]);

        /* Inherit model translations to meshes. */
        m->mesh[i].scale = m->scale;
        m->mesh[i].q = m->q;
        memcpy(&m->mesh[i].coords, &m->coords, 64);
    }

    releaseOBJ(&obj);
}
void releaseModel(model *m) {
    free(m->cname);
    for (int i = 0; i < m->mesh_indexes; i++) {
        releaseMesh(&m->mesh[i]);
    }
    free(m->mesh);
    if (m->rigid.state == ENABLED)
        releaseRigid(&m->rigid);
}


