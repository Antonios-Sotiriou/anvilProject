#include "headers/components/scene.h"

void initScene(scene *s, int winWidth, int winHeight) {
    s->WIDTH = winWidth;
    s->HEIGHT = winHeight;
    s->lastMouseX = winWidth * 0.5f;
    s->lastMouseY = winHeight * 0.5f;
    s->eyePoint = camera;

    /* Create all the framebuffers and main general textures we need for the scene. */
    createSceneFrameBuffers(s);

    /* Create and initialize the GLOBAL SCENE. */
    createScene(s);

    createSceneCanvas(&s->canvas);
}
/* Allocates memory for the components of the GLOBAL SCENE. */
void createScene(scene *s) {
    int model_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM model;");

    s->model = calloc(model_entries + 1, sizeof(model)); // +1 here for model entries to save some space at the end of the array for the terrain.
    if (!s->model) {
        debug_log_critical(stdout, "Failed allocating space for scene models");
        exit(-1);
    }
    s->model_indexes = model_entries;

    /* Loads from database the default model values and populates GLOBAL SCENE with those. Models are indexed according to their position in database. */
    dbloadTable(GITANA_DB, s, TABLE_MODEL, "SELECT * FROM model;");

    for (int i = 0; i < s->model_indexes; i++) {
        createModel(&s->model[i]);
    }

    /* Loads the main Terrain from the database and increases the Scene model indexes value by one. Terrain is appended on the end to be rendered last.
       At this point we could calculate also the player position to load the appropriate terrain in the future when we will enter more terrains. */
    dbloadTable(GITANA_DB, s, TABLE_TERRAIN, "SELECT * FROM terrain WHERE pk=1;");
    createModel(&s->model[s->model_indexes]);
    s->last_model_index = s->model_indexes;
    s->model_indexes += 1;
}
void createSceneCanvas(canvas *c) {
    /* Main Vertex Buffer Object buffer initiallization. */
    glGenVertexArrays(1, &c->VAO);
    glBindVertexArray(c->VAO);
    glGenBuffers(1, &c->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, c->VBO);

    GLfloat quad[32] = {
        1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
        -1.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f
    };

    glBufferData(GL_ARRAY_BUFFER, 32 * 4, quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}
void releaseSceneCanvas(canvas *c) {
    glDeleteBuffers(1, &c->VBO);
    glDeleteVertexArrays(1, &c->VAO);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
/* Displays texture with given texture index on full screen. */
void drawOnSceneCanvas(canvas *c, const int textureIndex) {

    glUseProgram(displayShaderProgram);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(0, textureIndex);

    glBindVertexArray(c->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    debug_log_OpenGL();
}
/* Releases allocated ressources of the GLOBAL SCENE. */
void releaseScene(scene *s) {
    for (int i = 0; i < s->model_indexes; i++) {
        releaseModel(&s->model[i]);
    }
    free(s->model);
    for (int i = 0; i < s->t.quad_indexes; i++) {

        if (s->t.quad[i].mpks)
            free(s->t.quad[i].mpks);
    }
    free(s->t.quad);

    releaseSceneCanvas(&s->canvas);
}


