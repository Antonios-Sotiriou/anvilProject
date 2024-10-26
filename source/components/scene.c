#include "headers/components/scene.h"

scene SCENE = { 0 };
enum asset { terrain, player1, initAssets };

static void initScene(void);

/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(void) {
    SCENE.mesh = calloc(1, sizeof(mesh));
    initScene();
}
/* Initializes the models and meshes than compose the GLOBAL SCENE. */
static void initScene(void) {
    createMesh(&SCENE.mesh[terrain], CUBE_FLAT);
}
/* Releases allocated ressources of the GLOBAL SCENE. */
void releaseScene(void) {
    for (int i = 0; i < SCENE.mesh_indexes; i++) {
        releaseMesh(&SCENE.mesh[i]);
    }
    free(SCENE.mesh);
}


