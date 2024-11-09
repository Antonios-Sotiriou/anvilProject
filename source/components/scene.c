#include "headers/components/scene.h"

scene SCENE = { 0 };

//static void initScene(void);

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    int mesh_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM mesh;");
    SCENE.mesh = calloc(mesh_entries, sizeof(mesh));
    SCENE.mesh_indexes = mesh_entries;

    dbloadTable(GITANA_DB, MESH_TABLE, "SELECT * FROM mesh;");

    //initScene();
}
///* Initializes the models and meshes that compose the GLOBAL SCENE. */
//static void initScene(void) {
//    /* Terrain mesh from Heightmap. Always the first mesh in the scene. */
//    createTerrain(&SCENE.mesh[terrain], GITANA100x100);
//    SCENE.mesh[terrain].rigid.state = ENABLE;
//    memcpy(&SCENE.mesh[terrain].coords, &initCoords, 64);
//    SCENE.mesh[terrain].q = unitQuat();
//    SCENE.mesh[terrain].scale = 1000.f;
//
//    /* Camera mesh. */
//    createMesh(&SCENE.mesh[camera], CUBE_FLAT);
//    SCENE.mesh[camera].rigid.state = ENABLE;
//    memcpy(&SCENE.mesh[camera], &initCoords, 64);
//    SCENE.mesh[camera].q = unitQuat();
//    SCENE.mesh[camera].scale = 100.f;
//
//    /* Camera mesh. */
//    createMesh(&SCENE.mesh[light], CUBE_FLAT);
//    SCENE.mesh[light].rigid.state = ENABLE;
//    memcpy(&SCENE.mesh[light], &initCoords, 64);
//    SCENE.mesh[light].q = unitQuat();
//    SCENE.mesh[light].scale = 10.f;
//}
/* Releases allocated ressources of the GLOBAL SCENE. */
void releaseScene(void) {
    for (int i = 0; i < SCENE.mesh_indexes; i++) {
        releaseMesh(&SCENE.mesh[i]);
    }
    free(SCENE.mesh);
}


