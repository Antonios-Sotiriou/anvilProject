#include "headers/components/scene.h"

scene SCENE = { 0 };
int TERRAIN_INDEX = 0;

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    int model_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM model;");

    SCENE.model = calloc(model_entries + 1, sizeof(model)); // +1 here for model entries to save some space at the end of the array for the terrain.
    SCENE.model_indexes = model_entries;

    /* Loads from database the default model values and populates GLOBAL SCENE with those. Models are indexed according to their position in database. */
    dbloadTable(GITANA_DB, TABLE_MODEL, "SELECT * FROM model;");

    for (int i = 0; i < SCENE.model_indexes; i++) {
        createModel(&SCENE.model[i]);
    }

    /* Loads the main Terrain from the database and increases the Scene model indexes value by one. Terrain is appended on the end to be rendered last.
       At this point we could calculate also the player position to load the appropriate terrain in the future when we will enter more terrains. */
    dbloadTable(GITANA_DB, TABLE_TERRAIN, "SELECT * FROM terrain WHERE pk=1;");
    createModel(&SCENE.model[SCENE.model_indexes]);
    // Init the global variable which is the index in SCENE models array of the terrain.
    TERRAIN_INDEX = SCENE.model_indexes;
    SCENE.model_indexes += 1;
}
/* Releases allocated ressources of the GLOBAL SCENE. */
void releaseScene(void) {
    for (int i = 0; i < SCENE.model_indexes; i++) {
        releaseModel(&SCENE.model[i]);
    }
    free(SCENE.model);
    for (int i = 0; i < SCENE.t.quad_indexes; i++) {

        if (SCENE.t.quad[i].mpks)
            free(SCENE.t.quad[i].mpks);
    }
    free(SCENE.t.quad);
}


