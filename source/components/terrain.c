#include "headers/components/terrain.h"

void createTerrain(mesh *m, const char path[]) {
	BMP bmp;
	readBMP(&bmp, path);
    const int emvadon = (bmp.info.Width > 0) && (bmp.info.Height > 0) ? bmp.info.Width * bmp.info.Height : 0;
    if (!emvadon) {
        fprintf(stderr, "Zero value for emvadon: %d. createTerrain() --> ERROR 1\n", emvadon);
        exit(1);
    }

    /* Quads. */
    const int quad_vcols = bmp.info.Width - 1;
    const int quad_vrows = bmp.info.Height - 1;
    const int quads = quad_vrows * quad_vcols;
    if (!quads) {
        fprintf(stderr, "Zero value for Quads: %d. createTerrain() --> ERROR 1\n", emvadon);
        exit(1);
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

    float *v = calloc(emvadon, 12);
    if (!v) {
        printf("Failed to allocate memory for *v -- createTerrain()\n");
        return;
    }
    float* t = calloc(emvadon, 8);
    if (!t) {
        printf("Failed to allocate memory for *t -- createTerrain()\n");
        return;
    }
    float *n = calloc(emvadon, 12);
    if (!n) {
        printf("Failed to allocate memory for *n -- createTerrain()\n"); 
        return;
    }
    int *f = calloc(num_of_faces, 4);
    if (!f) {
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
         //v[v_index + 1] = (float)rand() / (float)(RAND_MAX / 0.09f);
        // v[v_index + 1] = 0.f;
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
/* Assigns the Terrain quad index to the given mesh. */
void initMeshQuadInfo(mesh* m) {
    const int quad_index = getTerrainQuadIndex(m->coords.v[0]);
    /* Set meshes m quadIndex to index. */
    if (m->quadIndex != quad_index && m->quadInit) {
        //printf("Remove mesh from quad -- initMeshQuadInfo().\n");
        removeMeshFromQuad(m);
    }

    m->quadIndex = quad_index;
    m->quadInit = 1;
    addMeshToQuad(m);
    //printf("Initialized mesh quad info -- initMeshQuadInfo().\n");
}
/* Adds a Mesh to the Quad that is standing on to, if its not already a member of this Quad. */
void addMeshToQuad(mesh* m) {
    const int quad_index = m->quadIndex;

    if ((quad_index < 0 || !m->pk)) {
        /* Mesh is out of terrain if its quadIndex is less than Zero or it is the terrain if its ID is 0. */
        //printf("Outside of terrain limits -- addMeshToQuad().\n");
        return;
    }

    if (!SCENE.t.quad[quad_index].m_pks) {
        /* Quad had not previous members in it so we must allocate memory for the new member. */
        SCENE.t.quad[quad_index].m_pks = calloc(1, 4);
        SCENE.t.quad[quad_index].m_indexes = 1;
        SCENE.t.quad[quad_index].m_pks[0] = m->pk;
        //printf("Allocating memory for new quad memmbers array -- addMeshToQuad().\n");
        return;
    }

    for (int i = 0; i < SCENE.t.quad[quad_index].m_indexes; i++) {
        if (SCENE.t.quad[quad_index].m_pks[i] == m->pk) {
            /* Mesh is already a member of the Quad. */
            //printf("Mesh already quad member -- addMeshToQuad().\n");
            return;
        }
    }

    /* Increase the size of Quad members pointer to add the new member.Increment the necessery values also. */
    int *temp = realloc(SCENE.t.quad[quad_index].m_pks, (SCENE.t.quad[quad_index].m_indexes + 1) * 4);
    if (!temp) {
        free(SCENE.t.quad[quad_index].m_pks);
        return;
    }
    SCENE.t.quad[quad_index].m_pks = temp;
    SCENE.t.quad[quad_index].m_pks[SCENE.t.quad[quad_index].m_indexes] = m->pk;
    SCENE.t.quad[quad_index].m_indexes += 1;
}
/* Removes a mesh id from the TerrainInfo global Quad memmbers array. */
void removeMeshFromQuad(mesh* m) {
    const int quad_index = m->quadIndex;

    if (quad_index < 0) {
        /* Mesh is out of terrain if its quadIndex is less than Zero. */
        //printf("Outside of terrain limits -- removeMeshFromQuad().\n");
        return;
    }
    const int num_of_indexes = SCENE.t.quad[quad_index].m_indexes - 1;

    int *new_array = calloc(num_of_indexes, 4);
    if (!new_array) {
        //printf("Zero number of indexes -- removeMeshFromQuad().\n");
        return;
    }

    int inc = 0;
    for (int i = 0; i < SCENE.t.quad[quad_index].m_indexes; i++) {
        if (SCENE.t.quad[quad_index].m_pks[i] != m->pk) {
            new_array[inc] = SCENE.t.quad[quad_index].m_pks[i];
            inc++;
        }
    }
    free(SCENE.t.quad[quad_index].m_pks);
    SCENE.t.quad[quad_index].m_pks = realloc(new_array, (num_of_indexes * 4));
    SCENE.t.quad[quad_index].m_indexes = num_of_indexes;
}
/* Retrieves Terrain Quad index at given coords. */
const int getTerrainQuadIndex(vec4 coords) {
    const float t_scale = SCENE.mesh[terrain].scale * 2.f;
    float quad_len = t_scale / SCENE.t.vecWidth;
    const int t_limit = t_scale - quad_len;

    vec4 t_coords = vecSubvec(coords, vecSubf32(SCENE.mesh[terrain].coords.v[0], SCENE.mesh[terrain].scale));
#ifdef VECTORIZED_CODE // ######################################################
    if ((t_coords.m128_f32[0] >= t_limit || t_coords.m128_f32[0] < 0) || (t_coords.m128_f32[2] >= t_limit || t_coords.m128_f32[2] < 0)) {
        //printf("Out of terrain Limits -- getTerrainQuadIndex().\n");
        return -1;
    }
    /* Function to get t quads indexes. */
    vec4 pos = vecMulf32(vecDivf32(t_coords, t_scale), SCENE.t.vecWidth);
    const int quad_index = ((int)_mm_cvtss_f32(_mm_shuffle_ps(pos, pos, _MM_SHUFFLE(0, 0, 0, 2))) * SCENE.t.quadRows) + (int)_mm_cvtss_f32(pos);
#else // ITERATIVE_CODE ########################################################
    if ((t_coords.f32[0] >= t_limit || t_coords.f32[0] < 0) || (t_coords.f32[2] >= t_limit || t_coords.f32[2] < 0)) {
        //printf("Out of terrain Limits -- getTerrainQuadIndex().\n");
        return -1;
    }
    vec4 pos = vecMulf32(vecDivf32(t_coords, t_scale), SCENE.t.vecWidth);
    const int quad_index = ((int)pos.f32[2] * SCENE.t.quadRows) + (int)pos.f32[0];
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
    printf("indexes: %d\n", SCENE.t.quad[quad_index].m_indexes);
    if (!SCENE.t.quad[quad_index].m_pks) {
        fprintf(stderr, "Quad %d has no members.\n", quad_index);
        return;
    }
    printf("members ids: ");
    for (int i = 0; i < SCENE.t.quad[quad_index].m_indexes; i++) {
        printf("%d, ", SCENE.t.quad[quad_index].m_pks[i]);
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


