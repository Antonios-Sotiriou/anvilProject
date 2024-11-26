#include "headers/components/terrain.h"

void createTerrain(mesh *m, const char name[]) {
    int path_length = (strlen(name) * 2) + 22; // Plus 1 here for the null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "malloc()");
        debug_log_info(stdout, "%s\n", name);
        return;
    }
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sprintf_s(dynamic_path, path_length, "terrains/%s/%s128x128.bmp", name, name);
#elif defined(LINUX) || defined(__linux__)
    snprintf(dynamic_path, path_length, "terrains/%s/%s128x128.bmp", name, name);
#endif

    BMP bmp;
	readBMP(&bmp, dynamic_path);
    free(dynamic_path);

    const int emvadon = (bmp.info.Width > 0) && (bmp.info.Height > 0) ? bmp.info.Width * bmp.info.Height : 0;
    if (!emvadon) {
        debug_log_critical(stdout, "Null value for Emvadon");
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

    /* Populate the gloabal SCENE TerrainInfo struct with infos. */
    SCENE.t.vecWidth = bmp.info.Width;
    SCENE.t.vecHeight = bmp.info.Height;
    SCENE.t.quadCols = quad_vcols;
    SCENE.t.quadRows = quad_vrows;
    SCENE.t.quad_indexes = quads;
    SCENE.t.quad = calloc(SCENE.t.quad_indexes, sizeof(Quad));

    /* Faces. */
    const int faces_per_row = quad_vrows * 2;
    const int num_of_faces = quads * 2 * 9;

    vec3 *v = calloc(emvadon, 12);
    vec2 *t = calloc(emvadon, 8);
    vec3 *n = calloc(emvadon, 12);
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

        v[x].m96_f32[0] += x_step_cache;
         //v[v_index + 1] = (float)rand() / (float)(RAND_MAX / 0.09f);
        // v[v_index + 1] = 0.f;
        v[x].m96_f32[1] = bmp.data[x] / 255.f;
        v[x].m96_f32[2] = z_step_cache;

        x_step_cache += step_x;
    }
    releaseBMP(&bmp);

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
        t[x].m64_f32[0] = tu_step_cache;
        t[x].m64_f32[1] = tv_step_cache;

        tu_step_cache += step_tu;
    }

    /* Normals initialization. ############################## */
    for (int x = 0; x < emvadon; x++) {
        n[x].m96_f32[0] = 0.f;
        n[x].m96_f32[1] = 1.f;
        n[x].m96_f32[2] = 0.f;
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

    /* Mesh info initialize, vbo aray initialize. */
    m->vbo_indexes = (num_of_faces / 9) * 24;
    m->faces_indexes = m->vbo_indexes / 24;
    m->vecs_indexes = m->faces_indexes * 3;
    m->vbo_size = m->vbo_indexes * 4;

    m->vbo = malloc(m->vbo_size);
    if (!m->vbo) {
        debug_log_critical(stdout, "malloc()");
        free(v);
        free(t);
        free(n);
        free(f);
        exit(-1);
    }

    int index = 0, vpad, tpad;
    for (int i = 0; i < num_of_faces; i++) {
        m->vbo[index]     = v[f[i]].m96_f32[0];
        m->vbo[index + 1] = v[f[i]].m96_f32[1];
        m->vbo[index + 2] = v[f[i]].m96_f32[2];
        i++;
        m->vbo[index + 3] = t[f[i]].m64_f32[0];
        m->vbo[index + 4] = t[f[i]].m64_f32[1];
        i++;
        m->vbo[index + 5] = n[f[i]].m96_f32[0];
        m->vbo[index + 6] = n[f[i]].m96_f32[1];
        m->vbo[index + 7] = n[f[i]].m96_f32[2];
        index += 8;
    }

    createMeshVAO(m);

    free(v);
    free(t);
    free(n);
    free(f);
}
/* Assigns the Terrain quad index to the given mesh. */
void initMeshQuadInfo(mesh* m) {
    const int quad_index = getTerrainQuadIndex(m->coords.v[0]);
    /* Set meshes m quadIndex to index. */
    if (m->quadIndex != quad_index && m->quadInit) {
        removeMeshFromQuad(m);
    } else if (m->quadIndex == quad_index && m->quadInit) {
        /* Mesh is already a memmber of this quad. */
        return;
    }
    m->quadIndex = quad_index;
    m->quadInit = 1;
    addMeshToQuad(m);
}
/* Adds a Mesh to the Quad that is standing on to, if its not already a member of this Quad. */
void addMeshToQuad(mesh* m) {
    const int quad_index = m->quadIndex;

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
void removeMeshFromQuad(mesh *m) {
    const int quad_index = m->quadIndex;

    if (quad_index < 0) {
        /* Mesh is out of terrain if its quadIndex is less than Zero. */
        debug_log_warning(stdout, "Out of terrain boundaries");
        return;
    }
    const int num_of_indexes = SCENE.t.quad[quad_index].mpks_indexes - 1;

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
/* Retrieves Terrain Quad index at given coords. */
const int getTerrainQuadIndex(vec4 coords) {
    const float t_scale = SCENE.mesh[terrain].scale * 2.f;
    float quad_len = t_scale / SCENE.t.vecWidth;
    const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(coords, vecSubf32(SCENE.mesh[terrain].coords.v[0], SCENE.mesh[terrain].scale));
#ifdef VECTORIZED_CODE // ######################################################
    vec4 test_limits = _mm_set_ps1(t_limit);   // Probably must be declared as globals because are the same for every iteration.
    vec4 test_zero = _mm_setzero_ps();         // Probably must be declared as globals because are the same for every iteration.
    vec4 test_res = _mm_shuffle_ps(_mm_cmplt_ps(t_coords, test_zero), _mm_cmpgt_ps(t_coords, test_limits), _MM_SHUFFLE(2, 0, 2, 0));

    if (_mm_movemask_ps(test_res)) {
        //printf("Out of terrain Limits -- getTerrainQuadIndex().\n");
        return -1;
    }
    /* Function to get t quads indexes. */
    vec4 pos = vecMulf32(vecDivf32(t_coords, t_scale), SCENE.t.vecWidth);
    const int quad_index = ((int)_mm_cvtss_f32(_mm_shuffle_ps(pos, pos, _MM_SHUFFLE(0, 0, 0, 2))) * SCENE.t.quadRows) + (int)_mm_cvtss_f32(pos);
#else // ITERATIVE_CODE ########################################################
    if ((t_coords.m128_f32[0] >= t_limit || t_coords.m128_f32[0] < 0) || (t_coords.m128_f32[2] >= t_limit || t_coords.m128_f32[2] < 0)) {
        //printf("Out of terrain Limits -- getTerrainQuadIndex().\n");
        return -1;
    }
    vec4 pos = vecMulf32(vecDivf32(t_coords, t_scale), SCENE.t.vecWidth);
    const int quad_index = ((int)pos.m128_f32[2] * SCENE.t.quadRows) + (int)pos.m128_f32[0];
#endif // VECTORIZED_CODE // ######################################################

    return quad_index;
}
/* Prints the members of given Quad index. */
void logTerrainQuad(const int quad_index) {
    if (quad_index < 0) {
        /* Mesh is out of terrain if its quadIndex is less than Zero. */
        return;
    }

    printf("Quad: %d\n", quad_index);
    printf("indexes: %d\n", SCENE.t.quad[quad_index].mpks_indexes);
    if (!SCENE.t.quad[quad_index].mpks) {
        fprintf(stderr, "Quad %d has no members.\n", quad_index);
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
    printf("vecWidth : %d\n", SCENE.t.vecWidth);
    printf("vecHeight: %d\n", SCENE.t.vecHeight);
    printf("quadRows : %d\n", SCENE.t.quadRows);
    printf("quadCols : %d\n", SCENE.t.quadCols);
    printf("quadsArea: %d\n", SCENE.t.quad_indexes);
}


