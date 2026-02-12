#include "headers/components/terrain.h"

static int initIndividualTerrainHeightMap(TerrainInitInfo *tif);
static void initTerrainVectors(BMP *bmp, const int emvadon, const char new_file_path[]);
static void initTerrainTextors(BMP *bmp, const int emvadon, const char new_file_path[]);
static void initTerrainNormals(BMP *bmp, const int emvadon, const char new_file_path[]);
static void initTerrainFaces(BMP *bmp, const int num_of_faces, const char new_file_path[]);
static void updateTerrainQuadsInfoDB(BMP* bmp, const char cname[]);

/* Reads terrains database to aquire informations about height maps stored.Creates obj files from those height maps and updates database with usefull informations, which are used for other calculations like terrain collision detection. */
int initTerrainsHeightMaps(void) {
    int terrain_entries = dbcountTableRows(GITANA_DB, "SELECT COUNT(*) FROM terrain;");
    TerrainInitInfo *tif = calloc(terrain_entries, sizeof(TerrainInitInfo));
    if (!tif) {
        debug_log_critical(stdout, "model *m = malloc(terrain_entries * sizeof(TerrainInitInfo))");
        return -1;
    }

    dbloadTerrainInfo(GITANA_DB, tif);

    for (int i = 0; i < terrain_entries; i++) {
        initIndividualTerrainHeightMap(&tif[i]);
    }

    debug_log_info(stdout, "Initialized %d terrain entries.\n", terrain_entries);
    free(tif);

    return 0;
}
static int initIndividualTerrainHeightMap(TerrainInitInfo *tif) {
    char path[100] = { 0 };
    anvil_snprintf(path, 100, "height_maps/%s/%s%dx%d.bmp", tif->cname, tif->cname, tif->width, tif->height);

    BMP bmp;
    readBMP(&bmp, path);

    const int emvadon = (bmp.info.Width > 0) && (bmp.info.Height > 0) ? bmp.info.Width * bmp.info.Height :
        (bmp.info.Width == 0) ? bmp.info.Height :
        (bmp.info.Height == 0) ? bmp.info.Width : 0;
    if (!emvadon) {
        debug_log_critical(stdout, "Null value for Terrain Emvadon");
        releaseBMP(&bmp);
        return -1;
    }

    // Create the obj file of the bmp terrain height map and write the database height map entry name.
    char filepath[100] = { 0 };
    anvil_snprintf(filepath, 100, "%s/models/%s/%s.obj", anvil_SOURCE_DIR, tif->cname, tif->cname);

    FILE* fp = fopen(filepath, "w");
    if (fp == NULL) {
        debug_log_error(stdout, "Could not open file to initialize terrain cname from height map.");
        return -1;
    }
    /* Write the header informations of newly created bmp file.Infos are name of the objects, materials that we may use etc. */
    char header[100] = { 0 };
    anvil_snprintf(header, 100, "# Blender 3.3.0\n# www.blender.org\no %s\n", tif->cname);

    fwrite(header, strlen(header), 1, fp);
    fclose(fp);

    /* Vectors initialization. ############################## */
    initTerrainVectors(&bmp, emvadon, filepath);
    /* Normals initialization. ############################## */
    initTerrainNormals(&bmp, emvadon, filepath);
    /* Textors initialization. ############################## */
    initTerrainTextors(&bmp, emvadon, filepath);
    /* Faces initialization. ############################## */
    int num_of_faces = ((bmp.info.Width - 1) * (bmp.info.Height - 1)) * 2 * 9;
    initTerrainFaces(&bmp, num_of_faces, filepath);
    /* Update the Values in the database which help us with terrain collisions. */
    updateTerrainQuadsInfoDB(&bmp, tif->cname);


    releaseBMP(&bmp);
    return 0;
}
static void initTerrainVectors(BMP *bmp, const int emvadon, const char new_file_path[]) {
    FILE* fp = fopen(new_file_path, "a");
    if (fp == NULL) {
        debug_log_error(stdout, "Could not open file to initialize terrain vectors from height map.");
        return;
    }

    float step_x = 2.f / bmp->info.Width;
    float step_z = 2.f / bmp->info.Height;
    float start_x = -1.f;//((step_x * (bmp.info.Width * 0.5)) - 1);
    float start_z = -1.f;//((step_z * (bmp.info.Height * 0.5)) - 1);
    float x_step_cache = start_x;
    float z_step_cache = start_z;

    int vcols_count = bmp->info.Width;
    for (int x = 0; x < emvadon; x++) {
        char data[50] = { 0 };
        if (x == vcols_count) {
            x_step_cache = start_x;
            z_step_cache += step_z;

            vcols_count += bmp->info.Width;
        }

        anvil_snprintf(data, 50, "v %f %f %f\n", x_step_cache, bmp->data[x] / 255.f, z_step_cache);
        fwrite(&data, strlen(data), 1, fp);

        x_step_cache += step_x;
    }

    fclose(fp);
}
static void initTerrainNormals(BMP* bmp, const int emvadon, const char new_file_path[]) {
    FILE* fp = fopen(new_file_path, "a");
    if (fp == NULL) {
        debug_log_error(stdout, "Could not open file to initialize terrain normals from height map.");
        return;
    }

    for (int x = 0; x < emvadon; x++) {
        char data[50] = { 0 };
        anvil_snprintf(data, 50, "vn %f %f %f\n", 0.f, 1.f, 0.f);
        fwrite(&data, strlen(data), 1, fp);
    }

    fclose(fp);
}
static void initTerrainTextors(BMP* bmp, const int emvadon, const char new_file_path[]) {
    FILE* fp = fopen(new_file_path, "a");
    if (fp == NULL) {
        debug_log_error(stdout, "Could not open file to initialize terrain textors from height map.");
        return;
    }

    float step_tu = 1.f / (bmp->info.Width - 1);
    float step_tv = 1.f / (bmp->info.Height - 1);
    float start_tu = 0.f;
    float start_tv = 0.f;
    float tu_step_cache = start_tu;
    float tv_step_cache = start_tv;

    int tx_count = bmp->info.Height;
    for (int x = 0; x < emvadon; x++) {
        char data[25] = { 0 };
        if (x == tx_count) {
            tu_step_cache = start_tu;
            tv_step_cache += step_tv;

            tx_count += bmp->info.Height;
        }

        anvil_snprintf(data, 25, "vt %f %f\n", tu_step_cache, tv_step_cache);
        fwrite(&data, strlen(data), 1, fp);

        tu_step_cache += step_tu;
    }

    fclose(fp);
}
static void initTerrainFaces(BMP* bmp, const int num_of_faces, const char new_file_path[]) {
    FILE* fp = fopen(new_file_path, "a");
    if (fp == NULL) {
        debug_log_error(stdout, "Could not open file to initialize terrain faces from height map.");
        return;
    }
    fwrite("s 0\n", 4, 1, fp);

    const int faces_per_row = (bmp->info.Height - 1) * 2;

    /* 1 here because indexes in obj file starting from 1. */
    int face_1_0 = 1;
    int face_1_1 = bmp->info.Width + 1;
    int face_1_2 = bmp->info.Width + 2;

    int face_counter = 0;
    for (int x = 0; x < num_of_faces; x += 18) {
        char data_1[90] = { 0 };
        char data_2[90] = { 0 };
        if (face_counter == faces_per_row) {
            face_1_0 += 1;
            face_1_1 += 1;
            face_1_2 += 1;

            face_counter = 0;
        }

        /* Face 1st Up. */
        anvil_snprintf(data_1, 90, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", face_1_0, face_1_0, 0, face_1_1, face_1_1, 0, face_1_2, face_1_2, 0);
        fwrite(&data_1, strlen(data_1), 1, fp);

        /* Face 2nd Down. */
        anvil_snprintf(data_2, 90, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", face_1_0, face_1_0, 0, face_1_2, face_1_2, 0, face_1_0 + 1, face_1_0 + 1, 0);
        fwrite(&data_2, strlen(data_2), 1, fp);

        face_1_0++;
        face_1_1++;
        face_1_2++;

        face_counter += 2;
    }
    fclose(fp);
}
static void updateTerrainQuadsInfoDB(BMP* bmp, const char cname[]) {
    int quad_cols = bmp->info.Width - 1;   // -1 here because the quads are 1 less that the vectors of the terrain in each dimension.
    int quad_rows = bmp->info.Height - 1;  // -1 here because the quads are 1 less that the vectors of the terrain in each dimension.
    int quads_indexes = quad_cols * quad_rows;

    char command[150] = { 0 };
    anvil_snprintf(command, 150, "UPDATE terrain SET quad_cols = %d, quad_rows = %d, quads_indexes = %d WHERE cname = '%s';", quad_cols, quad_rows, quads_indexes, cname);

    dbExecuteCommand(GITANA_DB, command);
}
/* Assigns the Terrain quad index to the given mesh. */
void initModelQuadInfo(scene *s, model *m) {
    int quad_index, quad_face;
    getTerrainPointInfo(s, m->coords.v[0], &quad_index, &quad_face);

    /* Set meshes m quadIndex to index. */
    if (m->quad_index != quad_index && m->quad_init) {
        removeModelFromQuad(s, m);
    } else if (m->quad_index == quad_index && m->quad_init) {
        /* Mesh is already a member of this quad. */
        m->quad_face = quad_face;
        retrieveNearbyColliders(s, m);
        return;
    }
    m->quad_index = quad_index;
    m->quad_face = quad_face;
    m->quad_init = 1;
    addModelToQuad(s, m);
    updateSurroundingQuads(s, m);  // Here we have to find out the surrounding quads of the quad that the medel is in.
    retrieveNearbyColliders(s, m);
}
/* Adds a Mesh to the Quad that is standing on to, if its not already a member of this Quad. */
void addModelToQuad(scene *s, model *m) {
    const int quad_index = m->quad_index;

    if (quad_index < 0) {
        debug_log_info(stdout, "Model %s is out of terrain boundaries - addModelToQuad()\n", m->cname);
        return;
    }

    if (!s->t.quad[quad_index].mpks) {
        /* Quad had not previous members in it so we must allocate memory for the new member. */
        s->t.quad[quad_index].mpks = calloc(1, 4);
        if (!s->t.quad[quad_index].mpks) {
            debug_log_critical(stdout, "s->t.quad[quad_index].mpks = calloc(1, 4);\n");
            return;
        }
        s->t.quad[quad_index].mpks_indexes = 1;
        s->t.quad[quad_index].mpks[0] = m->pk;
        debug_log_info(stdout, "Adding 1st quad member");
        return;
    }

    /* Increase the size of Quad members pointer to add the new member.Increment the necessery values also. */
    int *temp = realloc(s->t.quad[quad_index].mpks, (s->t.quad[quad_index].mpks_indexes + 1) * 4);
    if (!temp) {
        free(s->t.quad[quad_index].mpks);
        return;
    }
    s->t.quad[quad_index].mpks = temp;
    s->t.quad[quad_index].mpks[s->t.quad[quad_index].mpks_indexes] = m->pk;
    s->t.quad[quad_index].mpks_indexes += 1;
}
/* Removes a mesh id from the TerrainInfo global Quad memmbers array. */
void removeModelFromQuad(scene *s, model *m) {
    const int quad_index = m->quad_index;

    if (quad_index < 0) {
        debug_log_info(stdout, "Model %s is out of terrain boundaries - removeModelFromQuad()\n", m->cname);
        return;
    }

    const int num_of_indexes = s->t.quad[quad_index].mpks_indexes - 1;
    if (num_of_indexes == 0) {
        free(s->t.quad[quad_index].mpks);
        s->t.quad[quad_index].mpks_indexes = 0;
        s->t.quad[quad_index].mpks = NULL;
        debug_log_info(stdout, "Previous quad has now NULL members. Freeing.\n");
        return;
    }

    int *new_array = calloc(num_of_indexes, 4);
    if (!new_array) {
        debug_log_critical(stdout, "int *new_array = calloc(num_of_indexes, 4);\n");
        return;
    }

    int inc = 0;
    for (int i = 0; i < s->t.quad[quad_index].mpks_indexes; i++) {
        if (s->t.quad[quad_index].mpks[i] != m->pk) {
            new_array[inc] = s->t.quad[quad_index].mpks[i];
            inc++;
        }
    }
    free(s->t.quad[quad_index].mpks);
    s->t.quad[quad_index].mpks = NULL;
    if (num_of_indexes > 0) {
        s->t.quad[quad_index].mpks = realloc(new_array, (num_of_indexes * 4));
    }
    s->t.quad[quad_index].mpks_indexes = num_of_indexes;
    debug_log_info(stdout, "Removed model from quad members\n");
}
/* Retrieves Terrain Quad index and terrain triangle at given coords. Terrain triangle Can either be UPPER: 0 or LOWER: 1. Returns Terrain quad index -1 if outside of terrain.*/
void getTerrainPointInfo(scene *s, vec4 coords, int* qi, int* uol) {
    const float t_scale = vec4ExtractX(s->model[s->last_model_index].scale) * 2.f;
    float quad_len = t_scale / s->t.vec_width;
    const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(coords, vecSubvec(s->model[s->last_model_index].coords.v[0], s->model[s->last_model_index].scale));

    if ((vec4ExtractX(t_coords) >= t_limit || vec4ExtractX(t_coords) < 0) || (vec4ExtractZ(t_coords) >= t_limit || vec4ExtractZ(t_coords) < 0)) {
        debug_log_info(stdout, "Point { %f, %f, %f, %f } is out of terrain boundaries - getTerrainPointInfo()", vec4ExtractX(coords), vec4ExtractY(coords), vec4ExtractZ(coords), vec4ExtractW(coords));
        *qi = -1;
        return;
    }
    /* Function to get terrain quad index. */
    vec4 pos = floorvec4(vecMulf32(vecDivf32(t_coords, t_scale), s->t.vec_width));
    *qi = (vec4ExtractZ(pos) * s->t.quad_rows) + vec4ExtractX(pos);

    /* Find in which Quad we are. */
    float x = vec4ExtractX(t_coords) / quad_len;
    float z = vec4ExtractZ(t_coords) / quad_len;
    x = x >= 1.f ? x - vec4ExtractX(pos) : x;
    z = z >= 1.f ? z - vec4ExtractZ(pos) : z;

    /* Find in which face we are. */
    if ((x - z) <= 0) {
        *uol = 0; // Upper face.
    } else {
        *uol = 1; // Lower face.
    }
}
/* Retrieves Terrain Position data tp and Terain position normal tn, at the given model's coordinates. */
void getModelPositionData(scene *s, model *m, vec4 *tp, vec4 *tn) {

    vec4 t_coords = vecSubvec(m->coords.v[0], vecSubvec(s->model[s->last_model_index].coords.v[0], s->model[s->last_model_index].scale));

    if ( m->quad_index == -1 ) {
        debug_log_info(stdout, "Model %s is out of terrain boundaries - getModelPositionData()\n", m->cname);
        *tp = setvec4Zero();
        *tn = setvec4Zero();
        return;
    }

    /* FInd in which face we are. */
    const int faceIndex = (m->quad_index * 2) + m->quad_face;

    vec4 vf[3] = {
        s->model[s->last_model_index].rigid.f[faceIndex].v[0],
        s->model[s->last_model_index].rigid.f[faceIndex].v[1],
        s->model[s->last_model_index].rigid.f[faceIndex].v[2]
    };

    const vec4 xs = setvec4(vec4ExtractX(vf[0]), vec4ExtractX(vf[1]), vec4ExtractX(vf[2]), 0.f);
    const vec4 zs = setvec4(vec4ExtractZ(vf[0]), vec4ExtractZ(vf[1]), vec4ExtractZ(vf[2]), 0.f);

    const vec4 xmx = vecSubvec(xs, setvec4(vec4ExtractY(xs), vec4ExtractZ(xs), vec4ExtractX(xs), vec4ExtractW(xs)));
    const vec4 zmz = vecSubvec(zs, setvec4(vec4ExtractY(zs), vec4ExtractZ(zs), vec4ExtractX(zs), vec4ExtractW(zs)));

    const float area = (vec4ExtractX(xmx) * vec4ExtractY(zmz)) - (vec4ExtractX(zmz) * vec4ExtractY(xmx));
    vec4 za = vecDivf32(
                  vecSubvec(
                      vecMulvec(vecSubf32(zs, vec4ExtractZ(t_coords)), xmx),
                      vecMulvec(vecSubf32(xs, vec4ExtractX(t_coords)), zmz)),
                  area);

    *tp = vecAddvec(
              vecAddvec(vecMulf32(vf[2], vec4ExtractX(za)), vecMulf32(vf[0], vec4ExtractY(za))),
              vecMulf32(vf[1], vec4ExtractZ(za)));
    *tn = crossProduct(vecSubvec(vf[1], vf[0]), vecSubvec(vf[2], vf[0]));
}
/* Retrieves Terrain *t height at given coords and, sets given meshes *m terain quadIndex to the id of the quad at those coords. */
const TerrainPointInfo getvec4PositionData(scene *s, const vec4 v) {
    TerrainPointInfo tp = { 0 };
    const float t_scale = vec4ExtractX(s->model[s->last_model_index].scale) * 2.f;
    float quad_len = t_scale / s->t.vec_width;
    const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(v, vecSubf32(s->model[s->last_model_index].coords.v[0], vec4ExtractX(s->model[s->last_model_index].scale)));

    if ((vec4ExtractX(t_coords) >= t_limit || vec4ExtractX(t_coords) < 0) || (vec4ExtractZ(t_coords) >= t_limit || vec4ExtractZ(t_coords) < 0)) {
        debug_log_info(stdout, "Point { %f, %f, %f, %f } is out of terrain boundaries - getvec4PositionData()\n", vec4ExtractX(v), vec4ExtractY(v), vec4ExtractZ(v), vec4ExtractW(v));
        tp.quad_index = -1;
        return tp;
    }

    /* Function to get t quads indexes. */
    vec4 pos = floorvec4(vecMulf32(vecDivf32(t_coords, t_scale), s->t.vec_width));
    const int quad_index = (vec4ExtractZ(pos) * s->t.quad_rows) + vec4ExtractX(pos);

    /* Find in which Quad we are. */
    float x = vec4ExtractX(t_coords) / quad_len;
    float z = vec4ExtractZ(t_coords) / quad_len;
    x = x >= 1.f ? x - vec4ExtractX(pos) : x;
    z = z >= 1.f ? z - vec4ExtractZ(pos) : z;

    /* FInd in which face we are. */
    int uol = 1;
    if ((x - z) <= 0)
        uol = 0;
    const int faceIndex = (quad_index * 2) + uol;

    vec4 vf[3] = {
        s->model[s->last_model_index].rigid.f[faceIndex].v[0],
        s->model[s->last_model_index].rigid.f[faceIndex].v[1],
        s->model[s->last_model_index].rigid.f[faceIndex].v[2]
    };

    const vec4 xs = setvec4(vec4ExtractX(vf[0]), vec4ExtractX(vf[1]), vec4ExtractX(vf[2]), 0.f);
    const vec4 zs = setvec4(vec4ExtractZ(vf[0]), vec4ExtractZ(vf[1]), vec4ExtractZ(vf[2]), 0.f);

    const vec4 xmx = vecSubvec(xs, setvec4(vec4ExtractY(xs), vec4ExtractZ(xs), vec4ExtractX(xs), vec4ExtractW(xs)));
    const vec4 zmz = vecSubvec(zs, setvec4(vec4ExtractY(zs), vec4ExtractZ(zs), vec4ExtractX(zs), vec4ExtractW(zs)));

    const float area = (vec4ExtractX(xmx) * vec4ExtractY(zmz)) - (vec4ExtractX(zmz) * vec4ExtractY(xmx));
    vec4 za = vecDivf32(
        vecSubvec(
            vecMulvec(vecSubf32(zs, vec4ExtractZ(t_coords)), xmx),
            vecMulvec(vecSubf32(xs, vec4ExtractX(t_coords)), zmz)),
        area);

    tp.pos = vecAddvec(
        vecAddvec(vecMulf32(vf[2], vec4ExtractX(za)), vecMulf32(vf[0], vec4ExtractY(za))),
        vecMulf32(vf[1], vec4ExtractZ(za)));
    tp.normal = crossProduct(vecSubvec(vf[1], vf[0]), vecSubvec(vf[2], vf[0]));
    tp.quad_index = quad_index;
    tp.quad_face = uol;

    return tp;
}
void updateSurroundingQuads(scene *s, model *m) {
    if (m->quad_index < 0) {
        m->surroundingQuadsIndexes = 0;
        m->collidersIndexes = 0;
        debug_log_info(stdout, "Model %s is out of terrain boundaries - updateSurroundingQuads()\n", m->cname);
        return;
    }
    int edge = 0;    // this variable tracks in which edge we are on the grid. 1 is Buttom and 2 is Upper edge. If this variable stays zero, that means we are not on an edgerather on middle.
    if (m->quad_index - s->t.quad_rows < 0)
        edge = 1;
    else if (m->quad_index + s->t.quad_rows > (s->t.quad_indexes - 1))
        edge = 2;

    if (m->surroundingQuads) {
        free(m->surroundingQuads);
        m->surroundingQuads = NULL;
    }

    if (m->quad_index % s->t.quad_rows == 0) {
        // Beginning
        if (edge == 1) {
            // Bottom Edge
            m->surroundingQuads = malloc(16);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index + 1;
            m->surroundingQuads[2] = m->quad_index + s->t.quad_rows;
            m->surroundingQuads[3] = m->surroundingQuads[2] + 1;
            m->surroundingQuadsIndexes = 4;
        } else if (edge == 2) {
            // Up Edge
            m->surroundingQuads = malloc(16);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index + 1;
            m->surroundingQuads[2] = m->quad_index - s->t.quad_rows;
            m->surroundingQuads[3] = m->surroundingQuads[2] + 1;
            m->surroundingQuadsIndexes = 4;
        } else {
            m->surroundingQuads = malloc(24);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index + 1;
            m->surroundingQuads[2] = m->quad_index + s->t.quad_rows;
            m->surroundingQuads[3] = m->surroundingQuads[2] + 1;
            m->surroundingQuads[4] = m->quad_index - s->t.quad_rows;
            m->surroundingQuads[5] = m->surroundingQuads[4] + 1;
            m->surroundingQuadsIndexes = 6;
        }
    } else if ((m->quad_index + 1) % s->t.quad_rows == 0) {
        // End
        if (edge == 1) {
            // Bottom Edge
            m->surroundingQuads = malloc(16);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index - 1;
            m->surroundingQuads[2] = m->quad_index + s->t.quad_rows;
            m->surroundingQuads[3] = m->surroundingQuads[2] - 1;
            m->surroundingQuadsIndexes = 4;
        } else if (edge == 2) {
            // Up Edge
            m->surroundingQuads = malloc(16);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index - 1;
            m->surroundingQuads[2] = m->quad_index - s->t.quad_rows;
            m->surroundingQuads[3] = m->surroundingQuads[2] - 1;
            m->surroundingQuadsIndexes = 4;
        } else {
            m->surroundingQuads = malloc(24);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index - 1;
            m->surroundingQuads[2] = m->quad_index + s->t.quad_rows;
            m->surroundingQuads[3] = m->surroundingQuads[2] - 1;
            m->surroundingQuads[4] = m->quad_index - s->t.quad_rows;
            m->surroundingQuads[5] = m->surroundingQuads[4] - 1;
            m->surroundingQuadsIndexes = 6;
        }
    } else {
        // Middle
        if (edge == 1) {
            // Bottom Edge
            m->surroundingQuads = malloc(24);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index + 1;
            m->surroundingQuads[2] = m->quad_index - 1;
            m->surroundingQuads[3] = m->quad_index + s->t.quad_rows;
            m->surroundingQuads[4] = m->surroundingQuads[3] + 1;
            m->surroundingQuads[5] = m->surroundingQuads[3] - 1;
            m->surroundingQuadsIndexes = 6;
        } else if (edge == 2) {
            // Up Edge
            m->surroundingQuads = malloc(24);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index + 1;
            m->surroundingQuads[2] = m->quad_index - 1;
            m->surroundingQuads[3] = m->quad_index - s->t.quad_rows;
            m->surroundingQuads[4] = m->surroundingQuads[3] + 1;
            m->surroundingQuads[5] = m->surroundingQuads[3] - 1;
            m->surroundingQuadsIndexes = 6;
        } else {
            m->surroundingQuads = malloc(36);
            m->surroundingQuads[0] = m->quad_index;
            m->surroundingQuads[1] = m->quad_index + 1;
            m->surroundingQuads[2] = m->quad_index - 1;
            m->surroundingQuads[3] = m->quad_index + s->t.quad_rows;
            m->surroundingQuads[4] = m->surroundingQuads[3] + 1;
            m->surroundingQuads[5] = m->surroundingQuads[3] - 1;
            m->surroundingQuads[6] = m->quad_index - s->t.quad_rows;
            m->surroundingQuads[7] = m->surroundingQuads[6] + 1;
            m->surroundingQuads[8] = m->surroundingQuads[6] - 1;
            m->surroundingQuadsIndexes = 9;
        }
    }
}
void retrieveNearbyColliders(scene *s, model *m) {
    if (m->quad_index < 0) {
        m->surroundingQuadsIndexes = 0;
        m->collidersIndexes = 0;
        debug_log_info(stdout, "Model %s out of terrain boundaries - retrieveNearbyColliders().", m->cname);
        return;
    }
    int total_models = 0;
    for (int i = 0; i < m->surroundingQuadsIndexes; i++) {
        total_models += s->t.quad[m->surroundingQuads[i]].mpks_indexes;
    }

    m->collidersIndexes = total_models - 1; // Minus 1 here because we don't want to count the active model also.
    if (m->collidersIndexes <= 0) {
        if (m->colliders) {
            free(m->colliders);
            m->colliders = NULL;
        }
        m->collidersIndexes = 0;
        return;
    }

    m->colliders = malloc(m->collidersIndexes * 4);
    if (!m->colliders) {
        debug_log_warning(stdout, "m->colliders = malloc(m->collidersIndexes * 4)");
        return;
    }
    int index = 0;
    for (int i = 0; i < m->surroundingQuadsIndexes; i++) {
        for (int y = 0; y < s->t.quad[m->surroundingQuads[i]].mpks_indexes; y++) {
            if (m->pk != s->t.quad[m->surroundingQuads[i]].mpks[y]) {
                m->colliders[index] = s->t.quad[m->surroundingQuads[i]].mpks[y];
                index++;
            }
        }
    }
}
/* Prints the members of given Quad index. */
void logTerrainQuad(scene *s, const int quad_index) {
    if (quad_index < 0) {
        debug_log_info(stdout, "Out of terrain boundaries - logTerrainQuad()");
        return;
    }

    printf("Quad: %d\n", quad_index);
    printf("indexes: %d\n", s->t.quad[quad_index].mpks_indexes);
    if (!s->t.quad[quad_index].mpks) {
        debug_log_info(stdout, "Quad %d has no members.", quad_index);
        return;
    }
    printf("members ids: ");
    for (int i = 0; i < s->t.quad[quad_index].mpks_indexes; i++) {
        printf("%d, ", s->t.quad[quad_index].mpks[i]);
    }
    printf("\n");
}
/* Prints the TerrainInfo struct. */
void logTerrainInfo(scene *s) {
    printf("vecWidth : %d\n", s->t.vec_width);
    printf("vecHeight: %d\n", s->t.vec_height);
    printf("quadRows : %d\n", s->t.quad_rows);
    printf("quadCols : %d\n", s->t.quad_cols);
    printf("quadsArea: %d\n", s->t.quad_indexes);
}


