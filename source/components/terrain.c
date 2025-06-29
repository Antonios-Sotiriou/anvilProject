#include "headers/components/terrain.h"

static float *initTerrainVectors(model *m, BMP *bmp, const int emvadon);
static float *initTerrainTextors(model *m, BMP *bmp, const int emvadon);
static float *initTerrainNormals(model *m, BMP *bmp, const int emvadon);
static int *initTerrainFaces(model *m, BMP *bmp, const int num_of_faces);

/* Creates a Terrain from a height map.Sets a predictable order of all the components.Create an obj file for the model. */
void createTerrain(model *m) {
    int path_length = (strlen(m->cname) * 2) + 22; // Plus 1 here for the null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "malloc()");
        debug_log_info(stdout, "%s\n", m->cname);
        return;
    }
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sprintf_s(dynamic_path, path_length, "terrains/%s/%s128x128.bmp", m->cname, m->cname);
#elif defined(LINUX) || defined(__linux__)
    snprintf(dynamic_path, path_length, "terrains/%s/%s128x128.bmp", m->cname, m->cname);
#endif

    BMP bmp;
	readBMP(&bmp, dynamic_path);
    free(dynamic_path);

    const int emvadon = (bmp.info.Width > 0) && (bmp.info.Height > 0) ? bmp.info.Width * bmp.info.Height :
        (bmp.info.Width == 0) ? bmp.info.Height :
        (bmp.info.Height == 0) ? bmp.info.Width : 0;
    if (!emvadon) {
        debug_log_critical(stdout, "Null value for Terrain Emvadon");
        releaseBMP(&bmp);
        exit(-1);
    }

    /* Quads. */
    const int quad_vcols = bmp.info.Width - 1;
    const int quad_vrows = bmp.info.Height - 1;
    const int quads = quad_vrows * quad_vcols;
    if (quads <= 0) {
        debug_log_critical(stdout, "Null value for quads");
        releaseBMP(&bmp);
        exit(-1);
    }

    /* Populate the global SCENE TerrainInfo struct with infos. */
    SCENE.t.vec_width = bmp.info.Width;
    SCENE.t.vec_height = bmp.info.Height;
    SCENE.t.quad_cols = quad_vcols;
    SCENE.t.quad_rows = quad_vrows;
    SCENE.t.quad_indexes = quads;
    SCENE.t.quad = calloc(SCENE.t.quad_indexes, sizeof(Quad));

    /* Model initialization. ############################## */
    m->model_matrix = identityMatrix();
    m->mesh_indexes = 1;
    m->mesh = calloc(1, sizeof(mesh));
    if (!m->mesh)
        debug_log_critical(stdout, "Could not allocate memory for terrain mesh: m->mesh = calloc(1, sizeof(mesh))");

    /* Mesh initialization.Terrain consists of only one mesh at the moment. ############################## */
    OBJ obj = { 0 };
    obj.e = calloc(1, sizeof(ENTRY));
    if (!obj.e)
        debug_log_critical(stdout, "Could not allocate memory for OBJ ENTRY to create mesh for the terrain: obj.e = calloc(1, sizeof(ENTRY))");

    obj.e[0].cname = m->cname;
    obj.e[0].cname_length = m->cname_length;
    /* Vectors initialization. ############################## */
    obj.e[0].v = initTerrainVectors(m, &bmp, emvadon);
    /* Textors initialization. ############################## */
    obj.e[0].t = initTerrainTextors(m, &bmp, emvadon);
    /* Normals initialization. ############################## */
    obj.e[0].n = initTerrainNormals(m, &bmp, emvadon);
    /* faces initialization. ############################## */
    obj.e[0].f_indexes = quads * 2 * 9;
    obj.e[0].f = initTerrainFaces(m, &bmp, obj.e[0].f_indexes);

    createMesh(&m->mesh[0], obj.e[0]);
    releaseBMP(&bmp);
    releaseOBJ(&obj);
}
static float *initTerrainVectors(model* m, BMP *bmp, const int emvadon) {
    float *v = calloc(emvadon, 12);
    if (!v) {
        debug_log_critical(stdout, "vec3 *v = calloc(emvadon, 12)");
        return NULL;
    }

    float step_x = 2.f / bmp->info.Width;
    float step_z = 2.f / bmp->info.Height;
    float start_x = -1.f;//((step_x * (bmp.info.Width * 0.5)) - 1);
    float start_z = -1.f;//((step_z * (bmp.info.Height * 0.5)) - 1);
    float x_step_cache = start_x;
    float z_step_cache = start_z;

    int vcols_count = bmp->info.Width;
    int inc = 0;
    for (int x = 0; x < emvadon; x++) {

        if (x == vcols_count) {
            x_step_cache = start_x;
            z_step_cache += step_z;

            vcols_count += bmp->info.Width;
        }

        v[inc] += x_step_cache;
        v[inc + 1] = bmp->data[x] / 255.f;
        v[inc + 2] = z_step_cache;

        x_step_cache += step_x;
        inc += 3;
    }

    return v;
}
static float *initTerrainTextors(model *m, BMP *bmp, const int emvadon) {
    float *t = calloc(emvadon, 8);
    if (!t) {
        debug_log_critical(stdout, "vec2 *t = calloc(emvadon, 8)");
        return NULL;
    }

    float step_tu = 1.f / (bmp->info.Width - 1);
    float step_tv = 1.f / (bmp->info.Height - 1);
    float start_tu = 0.f;
    float start_tv = 0.f;
    float tu_step_cache = start_tu;
    float tv_step_cache = start_tv;

    int tx_count = bmp->info.Height;
    int inc = 0;
    for (int x = 0; x < emvadon; x++) {

        if (x == tx_count) {
            tu_step_cache = start_tu;
            tv_step_cache += step_tv;

            tx_count += bmp->info.Height;
        }
        t[inc] = tu_step_cache;
        t[inc + 1] = tv_step_cache;

        tu_step_cache += step_tu;
        inc += 2;
    }
    return t;
}
static float *initTerrainNormals(model *m, BMP *bmp, const int emvadon) {
    float *n = calloc(emvadon, 12);
    if (!n) {
        debug_log_critical(stdout, "vec3 *n = calloc(emvadon, 12)");
        return NULL;
    }

    int inc = 0;
    for (int x = 0; x < emvadon; x++) {
        n[inc] = 0.f;
        n[inc + 1] = 1.f;
        n[inc + 2] = 0.f;

        inc += 3;
    }
    return n;
}
static int *initTerrainFaces(model* m, BMP *bmp, const int num_of_faces) {
    int *f = calloc(num_of_faces, 4);
    if (!f) {
        debug_log_critical(stdout, "int *f = calloc(num_of_faces, 4)");
        return NULL;
    }

    const int faces_per_row = (bmp->info.Height - 1) * 2;
 
    int face_1_0 = 0;
    int face_1_1 = bmp->info.Height;
    int face_1_2 = bmp->info.Height + 1;

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
    return f;
}
/* Assigns the Terrain quad index to the given mesh. */
void initModelQuadInfo(model *m) {
    int quad_index, tri;
    getTerrainPointInfo(m->coords.v[0], &quad_index, &tri);
    /* Set meshes m quadIndex to index. */
    if (m->quad_index != quad_index && m->quad_init) {
        removeModelFromQuad(m);
    } else if (m->quad_index == quad_index && m->quad_init) {
        /* Mesh is already a memmber of this quad. */
        return;
    }
    m->quad_index = quad_index;
    m->quad_face = tri;
    m->quad_init = 1;
    addModelToQuad(m);
}
/* Adds a Mesh to the Quad that is standing on to, if its not already a member of this Quad. */
void addModelToQuad(model *m) {
    const int quad_index = m->quad_index;

    if ((quad_index < 0 || !m->pk)) {
        /* Mesh is out of terrain if its quadIndex is less than Zero or it is the terrain if its ID is 0. */
        debug_log_warning(stdout, "Out of terrain boundaries");
        return;
    }

    if (!SCENE.t.quad[quad_index].mpks) {
        /* Quad had not previous members in it so we must allocate memory for the new member. */
        SCENE.t.quad[quad_index].mpks = calloc(1, 4);
        SCENE.t.quad[quad_index].mpks_indexes = 1;
        SCENE.t.quad[quad_index].mpks[0] = m->pk;
        debug_log_message(stdout, "Adding 1st quad member");
        return;
    }

    /* Increase the size of Quad members pointer to add the new member.Increment the necessery values also. */
    int *temp = realloc(SCENE.t.quad[quad_index].mpks, (SCENE.t.quad[quad_index].mpks_indexes + 1) * 4);
    if (!temp) {
        free(SCENE.t.quad[quad_index].mpks);
        return;
    }
    SCENE.t.quad[quad_index].mpks = temp;
    SCENE.t.quad[quad_index].mpks[SCENE.t.quad[quad_index].mpks_indexes] = m->pk;
    SCENE.t.quad[quad_index].mpks_indexes += 1;
}
/* Removes a mesh id from the TerrainInfo global Quad memmbers array. */
void removeModelFromQuad(model *m) {
    const int quad_index = m->quad_index;

    if (quad_index < 0) {
        /* Mesh is out of terrain if its quadIndex is less than Zero. */
        debug_log_warning(stdout, "Out of terrain boundaries");
        return;
    }
    const int num_of_indexes = SCENE.t.quad[quad_index].mpks_indexes - 1;
    if (!num_of_indexes) {
        return;
    }

    int *new_array = calloc(num_of_indexes, 4);
    if (!new_array) {
        debug_log_warning(stdout, "new_array");
        return;
    }

    int inc = 0;
    for (int i = 0; i < SCENE.t.quad[quad_index].mpks_indexes; i++) {
        if (SCENE.t.quad[quad_index].mpks[i] != m->pk) {
            new_array[inc] = SCENE.t.quad[quad_index].mpks[i];
            inc++;
        }
    }
    free(SCENE.t.quad[quad_index].mpks);
    SCENE.t.quad[quad_index].mpks = realloc(new_array, (num_of_indexes * 4));
    SCENE.t.quad[quad_index].mpks_indexes = num_of_indexes;
    debug_log_message(stdout, "Removed mesh from quad members");
}
/* Retrieves Terrain Quad index and terrain triangle at given coords. Terrain triangle Can either be UPPER: 0 or LOWER: 1. Returns Terrain quad index -1 if outside of terrain.*/
void getTerrainPointInfo(vec4 coords, int *qi, int *uol) {
    const float t_scale = vec4ExtractX(SCENE.model[TERRAIN_INDEX].scale) * 2.f;
    float quad_len = t_scale / SCENE.t.vec_width;
    const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(coords, vecSubf32(SCENE.model[TERRAIN_INDEX].coords.v[0], vec4ExtractX(SCENE.model[TERRAIN_INDEX].scale)));

    if ((vec4ExtractX(t_coords) >= t_limit || vec4ExtractX(t_coords) < 0) || (vec4ExtractZ(t_coords) >= t_limit || vec4ExtractZ(t_coords) < 0)) {
        debug_log_error(stdout, "Out of terrain boundaries");
        *qi = -1;
        return;
    }
    /* Function to get terrain quad index. */
    vec4 pos = floorvec4(vecMulf32(vecDivf32(t_coords, t_scale), SCENE.t.vec_width));
    *qi = (vec4ExtractZ(pos) * SCENE.t.quad_rows) + vec4ExtractX(pos);

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
void getModelPositionData(model *m, vec4 *tp, vec4 *tn) {
    // const float t_scale = SCENE.model[TERRAIN_INDEX].scale * 2.f;
    // float quad_len = t_scale / SCENE.t.vec_width;
    // const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(m->coords.v[0], vecSubf32(SCENE.model[TERRAIN_INDEX].coords.v[0], vec4ExtractX(SCENE.model[TERRAIN_INDEX].scale)));

    if ( m->quad_index == -1 ) {
        debug_log_warning(stdout, "Out of terrain boundaries");
        *tp = setvec4Zero();
        *tn = setvec4Zero();
        return;
    }

    /* Every quad has two faces incrementally. Every face constists of 24 indexes for vectors, normals, textors.
        So to get the right index we multiply faces with 24, because indexes are stored raw until now. */
    const int faceIndex = ((m->quad_index * 2) + m->quad_face) * 24;

    /* FInd in which face we are. */
    vec4 vf[3];
    memcpy(&vf[0], &SCENE.model[TERRAIN_INDEX].mesh[0].vbo[faceIndex], 12);
    memcpy(&vf[1], &SCENE.model[TERRAIN_INDEX].mesh[0].vbo[faceIndex + 8], 12);
    memcpy(&vf[2], &SCENE.model[TERRAIN_INDEX].mesh[0].vbo[faceIndex + 16], 12);

    vec4SetW(&vf[0], 1.f);
    vec4SetW(&vf[1], 1.f);
    vec4SetW(&vf[2], 1.f);

    /* Translate the face from object space to world space for the edge function to work and get the interpolated height value. */
    mat4x4 ttm = modelMatfromQST(SCENE.model[TERRAIN_INDEX].q, SCENE.model[TERRAIN_INDEX].scale, SCENE.model[TERRAIN_INDEX].coords.v[0]);
    setvec4arrayMulmat(vf, 3, ttm);

    const vec4 xs = setvec4(vec4ExtractX(vf[0]), vec4ExtractX(vf[1]), vec4ExtractX(vf[2]), 0.f);
    const vec4 zs = setvec4(vec4ExtractZ(vf[0]), vec4ExtractZ(vf[1]), vec4ExtractZ(vf[2]), 0.f);

    const vec4 xmx = vecSubvec(xs, setvec4(vec4ExtractY(xs), vec4ExtractZ(xs), vec4ExtractX(xs), vec4ExtractW(xs)));
    const vec4 zmz = vecSubvec(zs, setvec4(vec4ExtractY(zs), vec4ExtractZ(zs), vec4ExtractX(zs), vec4ExtractW(zs)));

    const float area = (vec4ExtractX(xmx) * vec4ExtractY(zmz)) - (vec4ExtractX(zmz) * vec4ExtractY(xmx));
    vec4 za = vecDivf32(
                  vecSubvec(
                      vecMulvec(vecSubf32(xs, vec4ExtractX(t_coords)), zmz),
                      vecMulvec(vecSubf32(zs, vec4ExtractZ(t_coords)), xmx)),
                  area);

    *tp = vecAddvec(
              vecAddvec(vecMulf32(vf[2], vec4ExtractX(za)), vecMulf32(vf[0], vec4ExtractY(za))),
              vecMulf32(vf[1], vec4ExtractZ(za)));
    *tn = crossProduct(vecSubvec(vf[1], vf[0]), vecSubvec(vf[2], vf[0]));
}
/* Retrieves Terrain *t height at given coords and, sets given meshes *m terain quadIndex to the id of the quad at those coords. */
const TerrainPointInfo getvec4PositionData(const vec4 v) {
    TerrainPointInfo tp = { 0 };
    const float t_scale = vec4ExtractX(SCENE.model[TERRAIN_INDEX].scale) * 2.f;
    float quad_len = t_scale / SCENE.t.vec_width;
    const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(v, vecSubf32(SCENE.model[TERRAIN_INDEX].coords.v[0], vec4ExtractX(SCENE.model[TERRAIN_INDEX].scale)));

    if ((vec4ExtractX(t_coords) >= t_limit || vec4ExtractX(t_coords) < 0) || (vec4ExtractZ(t_coords) >= t_limit || vec4ExtractZ(t_coords) < 0)) {
        debug_log_warning(stdout, "Out of terrain boundaries");
        tp.quad_index = -1;
        return tp;
    }

    /* Function to get t quads indexes. */
    vec4 pos = floorvec4(vecMulf32(vecDivf32(t_coords, t_scale), SCENE.t.vec_width));
    const int quad_index = (vec4ExtractZ(pos) * SCENE.t.quad_rows) + vec4ExtractX(pos);

    /* Every quad has two faces incrementally. Every face constists of 24 indexes for vectors, normals, textors.
        So to get the right index we multiply faces with 24, because indexes are stored raw until now. */
    // const int Upperface = (quad_index * 2) * 24;
    // const int Lowerface = ((quad_index * 2) + 1) * 24;

    /* Find in which Quad we are. */
    float x = vec4ExtractX(t_coords) / quad_len;
    float z = vec4ExtractZ(t_coords) / quad_len;
    x = x >= 1.f ? x - vec4ExtractX(pos) : x;
    z = z >= 1.f ? z - vec4ExtractZ(pos) : z;

    /* FInd in which face we are. */
    int uol = 1;
    if ((x - z) <= 0)
        uol = 0;
    const int faceIndex = ((quad_index * 2) + uol) * 24;

    vec4 vf[3];
    memcpy(&vf[0], &SCENE.model[TERRAIN_INDEX].mesh[0].vbo[faceIndex], 12);
    memcpy(&vf[1], &SCENE.model[TERRAIN_INDEX].mesh[0].vbo[faceIndex + 8], 12);
    memcpy(&vf[2], &SCENE.model[TERRAIN_INDEX].mesh[0].vbo[faceIndex + 16], 12);

    vec4SetW(&vf[0], 1.f);
    vec4SetW(&vf[1], 1.f);
    vec4SetW(&vf[2], 1.f);

    /* Translate the face from object space to world space for the edge function to work and get the interpolated height value. */
    mat4x4 ttm = modelMatfromQST(SCENE.model[TERRAIN_INDEX].q, SCENE.model[TERRAIN_INDEX].scale, SCENE.model[TERRAIN_INDEX].coords.v[0]);
    setvec4arrayMulmat(vf, 3, ttm);

    const vec4 xs = setvec4(vec4ExtractX(vf[0]), vec4ExtractX(vf[1]), vec4ExtractX(vf[2]), 0.f);
    const vec4 zs = setvec4(vec4ExtractZ(vf[0]), vec4ExtractZ(vf[1]), vec4ExtractZ(vf[2]), 0.f);

    const vec4 xmx = vecSubvec(xs, setvec4(vec4ExtractY(xs), vec4ExtractZ(xs), vec4ExtractX(xs), vec4ExtractW(xs)));
    const vec4 zmz = vecSubvec(zs, setvec4(vec4ExtractY(zs), vec4ExtractZ(zs), vec4ExtractX(zs), vec4ExtractW(zs)));

    const float area = (vec4ExtractX(xmx) * vec4ExtractY(zmz)) - (vec4ExtractX(zmz) * vec4ExtractY(xmx));
    vec4 za = vecDivf32(
        vecSubvec(
            vecMulvec(vecSubf32(xs, vec4ExtractX(t_coords)), zmz),
            vecMulvec(vecSubf32(zs, vec4ExtractZ(t_coords)), xmx)),
        area);

    tp.pos = vecAddvec(
        vecAddvec(vecMulf32(vf[2], vec4ExtractX(za)), vecMulf32(vf[0], vec4ExtractY(za))),
        vecMulf32(vf[1], vec4ExtractZ(za)));
    tp.normal = crossProduct(vecSubvec(vf[1], vf[0]), vecSubvec(vf[2], vf[0]));
    tp.quad_index = quad_index;
    tp.quad_face = uol;

    return tp;
}
/* Prints the members of given Quad index. */
void logTerrainQuad(const int quad_index) {
    if (quad_index < 0) {
        /* Mesh is out of terrain if its quadIndex is less than Zero. */
        debug_log_warning(stdout, "Out of terrain boundaries");
        return;
    }

    printf("Quad: %d\n", quad_index);
    printf("indexes: %d\n", SCENE.t.quad[quad_index].mpks_indexes);
    if (!SCENE.t.quad[quad_index].mpks) {
        debug_log_info(stdout, "Quad %d has no members.", quad_index);
        return;
    }
    printf("members ids: ");
    for (int i = 0; i < SCENE.t.quad[quad_index].mpks_indexes; i++) {
        printf("%d, ", SCENE.t.quad[quad_index].mpks[i]);
    }
    printf("\n");
}
/* Prints the TerrainInfo struct. */
void logTerrainInfo(void) {
    printf("vecWidth : %d\n", SCENE.t.vec_width);
    printf("vecHeight: %d\n", SCENE.t.vec_height);
    printf("quadRows : %d\n", SCENE.t.quad_rows);
    printf("quadCols : %d\n", SCENE.t.quad_cols);
    printf("quadsArea: %d\n", SCENE.t.quad_indexes);
}


