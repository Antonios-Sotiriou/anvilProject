#include "headers/components/scene.h"

scene SCENE = { 0 };

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    //int mesh_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM mesh;");

    //SCENE.mesh = calloc(mesh_entries, sizeof(mesh));
    //SCENE.mesh_indexes = mesh_entries;

    //dbloadTable(GITANA_DB, MESH_TABLE, "SELECT * FROM mesh;");

    SCENE.model = calloc(2, sizeof(model));
    createTerrain(&SCENE.model[0], "gitana");
    SCENE.model[0].mesh_indexes = 1;


    SCENE.model_indexes = 2;
    createModel(&SCENE.model[1], "robot");
    SCENE.model[0].scale = 100;
    SCENE.model[0].q = unitQuat();
    //for (int i = 0; i < SCENE.model[1].mesh_indexes; i++)
        SCENE.model[0].mesh[0].scale = SCENE.model[0].scale;
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


