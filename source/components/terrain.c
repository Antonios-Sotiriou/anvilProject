#include "headers/components/terrain.h"

void createTerrain(mesh *m, const char type[]) {
	BMP bmp;
	readBMP(&bmp, type);
    const int emvadon = (bmp.info.Width > 0) && (bmp.info.Height > 0) ? bmp.info.Width * bmp.info.Height : 0;
    if (!emvadon) {
        fprintf(stderr, "Zero value for emvadon: %d. createTerrain() --> ERROR 1\n", emvadon);
        exit(1);
    }

    /* Populate the gloabal TerrainInfo struct with infos. */
    //tf->vecWidth = bmp.info.Width;
    //tf->vecHeight = bmp.info.Height;
    //tf->quadCols = tf->vecWidth - 1;
    //tf->quadRows = tf->vecHeight - 1;
    //tf->quadsArea = tf->quadRows * tf->quadCols;
    //tf->quads = calloc(tf->quadsArea, sizeof(Quad));

    /* Quads. */
    const int quad_vcols = bmp.info.Width - 1;
    const int quad_vrows = bmp.info.Height - 1;
    const int quads = quad_vrows * quad_vcols;
    if (!quads) {
        fprintf(stderr, "Zero value for Quads: %d. createTerrain() --> ERROR 1\n", emvadon);
        exit(1);
    }

    /* Faces. */
    const int faces_per_row = quad_vrows * 2;
    const int num_of_faces = quads * 2 * 9;

    float *v = calloc(emvadon, 12);
    if (!v) {
        printf("Failed to allocate memory for *v -- createTerrain()\n");
        return;
    }
    float* t = calloc(emvadon, 8);
    if (!v) {
        printf("Failed to allocate memory for *t -- createTerrain()\n");
        return;
    }
    float *n = calloc(emvadon, 12);
    if (!v) {
        printf("Failed to allocate memory for *n -- createTerrain()\n"); 
        return;
    }
    int *f = calloc(num_of_faces, 4);
    if (!v) {
        printf("Failed to allocate memory for *f -- createTerrain()\n"); 
        return;
    }

    /* Vectors initialization. ############################## */
    float step_x = 2.f / bmp.info.Width;
    float step_z = 2.f / bmp.info.Height;
    float start_x = -1.f;//((step_x * (bmp.info.Width * 0.5)) - 1);
    float start_z = -1.f;//((step_z * (bmp.info.Height * 0.5)) - 1);

    float x_step_cache = start_x;
    float z_step_cache = start_z;

    int vcols_count = bmp.info.Width;
    int v_index = 0;

    for (int x = 0; x < emvadon; x++) {

        if (x == vcols_count) {
            x_step_cache = start_x;
            z_step_cache += step_z;

            vcols_count += bmp.info.Width;
        }

        v[v_index] += x_step_cache;
        // v[x][1] = (float)rand() / (float)(RAND_MAX / 0.09f);
        // v[x][1] = 0.f;
        v[v_index + 1] = bmp.data[x] / 255.f;
        v[v_index + 2] = z_step_cache;

        x_step_cache += step_x;
        v_index += 3;
    }
    free(bmp.data);

    /* Textors initialization. ############################## */
    float step_tu = 1.f / quad_vrows;
    float step_tv = 1.f / quad_vcols;
    float start_tu = 0.f;
    float start_tv = 0.f;
    float tu_step_cache = start_tu;
    float tv_step_cache = start_tv;

    int tx_count = bmp.info.Height;
    for (int x = 0; x < emvadon; x += 2) {

        if (x == tx_count) {
            tu_step_cache = start_tu;
            tv_step_cache += step_tv;

            tx_count += bmp.info.Height;
        }
        t[x] = tu_step_cache;
        t[x + 1] = tv_step_cache;

        tu_step_cache += step_tu;
    }

    /* Normals initialization. ############################## */
    for (int x = 0; x < emvadon; x += 3) {
        n[x] = 0.f;
        n[x + 1] = 1.f;
        n[x + 2] = 0.f;
    }

    /* faces initialization. ############################## */
    int face_1_0 = 0;
    int face_1_1 = bmp.info.Height;
    int face_1_2 = bmp.info.Height + 1;
    int face_counter = 0;

    for (int x = 0; x < num_of_faces; x += 18) {

        if (face_counter == faces_per_row) {
            face_1_0 += 1;
            face_1_1 += 1;
            face_1_2 += 1;

            face_counter = 0;
        }

        /* Face 1st Up. */
        f[x] = face_1_0;
        f[x + 1] = face_1_0;

        f[x + 3] = face_1_1;
        f[x + 4] = face_1_1;

        f[x + 6] = face_1_2;
        f[x + 7] = face_1_2;

        /* Face 2nd Down. */
        f[x + 9] = face_1_0;
        f[x + 10] = face_1_0;

        f[x + 12] = face_1_2;
        f[x + 13] = face_1_2;

        f[x + 15] = face_1_0 + 1;
        f[x + 16] = face_1_0 + 1;

        face_1_0++;
        face_1_1++;
        face_1_2++;

        face_counter += 2;
    }

    m->vbo_indexes = (num_of_faces / 9) * 24;
    m->faces_indexes = m->vbo_indexes / 24;
    m->vecs_indexes = m->faces_indexes * 3;
    m->vbo_size = m->vbo_indexes * 4;

    m->vbo = malloc(m->vbo_size);
    if (!m->vbo) {
        printf("Could not create mesh vbo. loadMesh() - malloc()\n");
        return;
    }

    int index = 0, vpad, tpad;
    for (int i = 0; i < num_of_faces; i++) {
        vpad = f[i] * 3;
        m->vbo[index] = v[vpad];
        m->vbo[index + 1] = v[vpad + 1];
        m->vbo[index + 2] = v[vpad + 2];
        i++;
        tpad = f[i] * 2;
        m->vbo[index + 3] = t[tpad];
        m->vbo[index + 4] = t[tpad + 1];
        i++;
        vpad = f[i] * 3;
        m->vbo[index + 5] = n[vpad];
        m->vbo[index + 6] = n[vpad + 1];
        m->vbo[index + 7] = n[vpad + 2];
        index += 8;
    }

    createMeshVAO(m);

    free(v);
    free(t);
    free(n);
    free(f);
}


