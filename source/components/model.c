#include "headers/components/model.h"

void createModel(model *m) {
    int path_length = (strlen(m->cname) * 2) + strlen(anvil_SOURCE_DIR) + 14; // Plus 1 here for the null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "char *dynamic_path = malloc(path_length)");
        debug_log_info(stdout, "%s\n", m->cname);
        return;
    }

    anvil_snprintf(dynamic_path, path_length, "%s/models/%s/%s.obj", anvil_SOURCE_DIR, m->cname, m->cname);

    OBJ obj = { 0 };
    readOBJ(&obj, dynamic_path);
    free(dynamic_path);

    m->mesh = calloc(obj.e_indexes, sizeof(mesh));
    m->mesh_indexes = obj.e_indexes;
    m->model_matrix = identityMatrix();
    for (int i = 0; i < m->mesh_indexes; i++) {
        /* Creating the meshes from ENTRIES of obj file. */
        createMesh(&m->mesh[i], obj.e[i]);
    }

    if (m->owns_rigid)
        loadModelRigid(m);

    if (m->owns_anim)
        loadModelAnimations(m);

    releaseOBJ(&obj);
}
void releaseModel(model *m) {
    free(m->cname);
    for (int i = 0; i < m->mesh_indexes; i++) {
        releaseMesh(&m->mesh[i]);
    }
    free(m->mesh);

    if (m->owns_rigid == ENABLED)
        releaseRigid(&m->rigid);

    if (m->owns_anim == ENABLED)
        releaseAnimations(&m->anim);

    free(m->surroundingQuads);
}


