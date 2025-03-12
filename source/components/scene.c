#include "headers/components/scene.h"

scene SCENE = { 0 };

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    int mesh_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM mesh;");

    SCENE.mesh = calloc(mesh_entries, sizeof(mesh));
    SCENE.mesh_indexes = mesh_entries;

    dbloadTable(GITANA_DB, MESH_TABLE, "SELECT * FROM mesh;");


    SCENE.model = calloc(1, sizeof(model));
    SCENE.model_indexes = 1;
    createModel(&SCENE.model[0], "robot");
}
/* Releases allocated ressources of the GLOBAL SCENE. */
void releaseScene(void) {
    for (int i = 0; i < SCENE.model_indexes; i++) {
        releaseModel(&SCENE.model[i]);
    }
    free(SCENE.model);
    for (int i = 0; i < SCENE.t.quad_indexes; i++) {
        if (SCENE.t.quad[i].mpks)
            free(&SCENE.t.quad[i].mpks);
    }
    free(SCENE.t.quad);
}


