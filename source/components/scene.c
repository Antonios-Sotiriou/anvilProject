#include "headers/components/scene.h"

scene SCENE = { 0 };

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    //int mesh_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM mesh;");

    //SCENE.mesh = calloc(mesh_entries, sizeof(mesh));
    //SCENE.mesh_indexes = mesh_entries;

    //dbloadTable(GITANA_DB, MESH_TABLE, "SELECT * FROM mesh;");

    /*  TERRAIN  */
    //SCENE.model = calloc(1, sizeof(model));
    //SCENE.model[0].mesh = calloc(1, sizeof(mesh));
    //SCENE.model_indexes = 1;
    //SCENE.model[0].mesh_indexes = 1;
    //createTerrain(&SCENE.model[0].mesh[0], "gitana");
    //SCENE.model[0].scale = 1000;
    //SCENE.model[0].q = unitQuat();
    //SCENE.model[0].mesh[0].scale = SCENE.model[0].scale;
    //SCENE.model[0].mesh[0].q = SCENE.model[0].q;
    //SCENE.model[0].mesh[0].coords.v[0].m128_f32[3] = 1.f;

    /* ROBOT */
    //SCENE.model = calloc(1, sizeof(model));
    //SCENE.model_indexes = 1;
    //createModel(&SCENE.model[0], "robot");
    //SCENE.model[0].scale = 100;
    //SCENE.model[0].q = rotationQuat(90, 0.f, 1.f, 0.f);
    //for (int i = 0; i < SCENE.model[0].mesh_indexes; i++) {
    //    SCENE.model[0].mesh[i].scale = SCENE.model[0].scale;
    //    SCENE.model[0].mesh[i].q = SCENE.model[0].q;
    //    SCENE.model[0].mesh[i].coords.v[0].m128_f32[3] = 1.f;
    //}

    int model_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM model;");

    SCENE.model = calloc(model_entries, sizeof(model));
    SCENE.model_indexes = model_entries;

    dbloadTable(GITANA_DB, MODEL_TABLE, "SELECT * FROM model;");

    for (int i = 0; i < SCENE.model_indexes; i++) {
        createModel(&SCENE.model[i], SCENE.model[i].cname);

        for (int x = 0; x < SCENE.model[i].mesh_indexes; x++) {
            SCENE.model[i].mesh[x].scale = SCENE.model[i].scale;
            SCENE.model[i].mesh[x].q = SCENE.model[i].q;
            memcpy(&SCENE.model[i].mesh[x].coords, &SCENE.model[i].coords, 64);
        }
    }
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


