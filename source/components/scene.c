#include "headers/components/scene.h"

scene SCENE = { 0 };

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    int model_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM model;");

    SCENE.model = calloc(model_entries, sizeof(model));
    SCENE.model_indexes = model_entries;

    /* Loads from database the default model values and populates GLOBAL SCENE with those. Models are indexed according to their position in database. */
    dbloadTable(GITANA_DB, MODEL_TABLE, "SELECT * FROM model;");

    for (int i = 0; i < SCENE.model_indexes; i++) {
        createModel(&SCENE.model[i]);
        //if (model.animations)      // Here we must load the animations probably.
    }
    loadModelAnimations(&SCENE.model[0]);
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


