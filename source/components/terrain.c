#include "headers/components/terrain.h"

void createTerrain(mesh* m, const char type[]) {
	BMP bmp;
	readBMP(&bmp, type);
    const int emvadon = bmp.info.Width * bmp.info.Height;

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

    vec4 *v = calloc(emvadon, 16);
    vec4 *n = calloc(emvadon, 16);
    vec2 *t = calloc(emvadon, 8);
    int *f = calloc(num_of_faces, 4);

    /* Vectors initialization. ############################## */
    float step_x = 2.f / bmp.info.Width;
    float step_z = 2.f / bmp.info.Height;
    float start_x = -1.f;//((step_x * (bmp.info.Width * 0.5)) - 1);
    float start_z = -1.f;//((step_z * (bmp.info.Height * 0.5)) - 1);

    float x_step_cache = start_x;
    float z_step_cache = start_z;

    int vcols_count = bmp.info.Width;

    for (int x = 0; x < emvadon; x++) {

        if (x == vcols_count) {
            x_step_cache = start_x;
            z_step_cache += step_z;

            vcols_count += bmp.info.Width;
        }

        v[x][0] += x_step_cache;
        // v[x][1] = (float)rand() / (float)(RAND_MAX / 0.09f);
        // v[x][1] = 0.f;
        v[x][1] = bmp.data[x] / 255.f;
        v[x][2] = z_step_cache;
        v[x][3] = 1.f;

        x_step_cache += step_x;
    }

    /* Textors initialization. ############################## */
    float step_tu = 1.f / quad_vrows;
    float step_tv = 1.f / quad_vcols;
    float start_tu = 0.f;
    float start_tv = 0.f;
    float tu_step_cache = start_tu;
    float tv_step_cache = start_tv;

    int tx_count = bmp.info.Height;
    for (int x = 0; x < emvadon; x++) {

        if (x == tx_count) {
            tu_step_cache = start_tu;
            tv_step_cache += step_tv;

            tx_count += bmp.info.Height;
        }
        t[x][0] = tu_step_cache;
        t[x][1] = tv_step_cache;

        tu_step_cache += step_tu;
    }

    /* Normals initialization. ############################## */
    vec4 normal = { 0.f, 1.f, 0.f, 0.f };
    for (int x = 0; x < emvadon; x++) {
        n[x] = normal;
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

    free(v);
    free(t);
    free(n);
    free(f);

	free(bmp.data);
}


