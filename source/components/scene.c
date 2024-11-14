#include "headers/components/scene.h"

scene SCENE = { 0 };

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    int mesh_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM mesh;");

    SCENE.mesh = calloc(mesh_entries, sizeof(mesh));
    SCENE.mesh_indexes = mesh_entries;

    dbloadTable(GITANA_DB, MESH_TABLE, "SELECT * FROM mesh;");

    loadRigid(&SCENE.mesh[camera], "meshes/cube/cube_flat.obj");
    loadRigid(&SCENE.mesh[3], "meshes/cube/cube_flat.obj");
}
/* Releases allocated ressources of the GLOBAL SCENE. */
void releaseScene(void) {
    for (int i = 0; i < SCENE.mesh_indexes; i++) {
        releaseMesh(&SCENE.mesh[i]);
    }
    free(SCENE.mesh);
    for (int i = 0; i < SCENE.t.quad_indexes; i++) {
        if (SCENE.t.quad[i].mpks)
            free(SCENE.t.quad[i].mpks);
    }
    free(SCENE.t.quad);
}


