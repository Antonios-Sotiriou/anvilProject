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

    m->mesh = malloc(sizeof(mesh) * obj.e_indexes);
    m->mesh_indexes = obj.e_indexes;
    for (int i = 0; i < m->mesh_indexes; i++) {
        createMesh(&m->mesh[i], obj.e[i]);
    }


    releaseOBJ(&obj);
}
void releaseModel(model *m) {
    for (int i = 0; i < m->mesh_indexes; i++) {
        releaseMesh(&m->mesh[i]);
    }
    free(m->mesh);
    releaseRigid(&m->rigid);
}


