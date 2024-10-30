#include "headers/components/mesh.h"

/* Usefull to initialize the starting mesh coordinates. */
const static vec4 initCoords[4] = {
    { 0.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0.f, 0.f, 1.f, 0.f }
};

/* vectors array indexes */
static int v_indexes = 0;
/* normals array indexes */
static int n_indexes = 0;
/* textures array indexes */
static int t_indexes = 0;
/* Face array indexes */
static int f_indexes = 0;

static void initMesh(mesh* m);
static void loadMesh(mesh* m, const char type[]);
static float *loadVectors(const char path[]);
static float *loadTexels(const char path[]);
static float *loadNormals(const char path[]);
static int *loadFaces(const char path[]);

void createMesh(mesh *m, const char type[]) {
    /* Initializing data for all meshes */
    initMesh(m);
    /* Load obj file to mesh. */
    loadMesh(m, type);
}
static void initMesh(mesh *m) {
    /* Assign starting coordinates for all meshes */
    memcpy(&m->coords, &initCoords, 64);
    //m->q = unitQuat();
    m->scale = 10.f;
}
/* Loads obj file data to a mesh. */
static void loadMesh(mesh* m, const char type[]) {
    float *v = loadVectors(type);
    if (!v) {
        printf("Could not reallocate Vectors array. loadMesh() - loadVectors()\n");
        return;
    }

    float *t = loadTexels(type);
    if (!t) {
        printf("Could not create Vectors array. loadMesh() - loadTexels()\n");
        return;
    }

    float *n = loadNormals(type);
    if (!n) {
        printf("Could not create Vectors array. loadMesh() - loadNormals()\n");
        return;
    }

    int *f = loadFaces(type);
    if (!f) {
        printf("Could not create Faces array. loadMesh() - loadFaces()\n");
        return;
    }

    m->vao_indexes = (f_indexes / 9) * 24;
    m->faces_indexes = m->vao_indexes / 24;
    m->vecs_indexes = m->faces_indexes * 3;
    m->vao_size = m->vao_indexes * 4;

    m->vao = malloc(m->vao_indexes * 4);
    if (!m->vao) {
        printf("Could not create mesh vao. loadMesh() - malloc()\n");
        return;
    }

    int index = 0, vpad, tpad;
    for (int i = 0; i < f_indexes; i++) {
        vpad = f[i] * 3;
        m->vao[index] = v[vpad];
        m->vao[index + 1] = v[vpad + 1];
        m->vao[index + 2] = v[vpad + 2];
        i++;
        tpad = f[i] * 2;
        m->vao[index + 3] = t[tpad];
        m->vao[index + 4] = t[tpad + 1];
        i++;
        vpad = f[i] * 3;
        m->vao[index + 5] = n[vpad];
        m->vao[index + 6] = n[vpad + 1];
        m->vao[index + 7] = n[vpad + 2];
        index += 8;
    }

    free(v);
    free(t);
    free(n);
    free(f);
}
static float *loadVectors(const char type[]) {
    FILE *fp = fopen(type, "r");
    if (!fp) {
        printf("Could not open file : %s.\n", type);
        return NULL;
    }

    float *v = malloc(12);
    if (!v) {
        printf("Could not allocate memory for vec4f struct. loadVectors() -- malloc().\n");
        fclose(fp);
        return NULL;
    }
    float va, vb, vc;
    int dynamic_inc = 1;
    int index = 0;

    char ch;
    while (!feof(fp)) {
        if ((ch = getc(fp)) == 'v')
            if ((ch = getc(fp)) == ' ')
                if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                    float *temp = realloc(v, 12 * dynamic_inc);
                    if (!temp) {
                        printf("Could not reallocate memory for vec4f struct array. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(v);
                        return NULL;
                    }
                    v = temp;
                    v[index] = va;
                    v[index + 1] = vb;
                    v[index + 2] = vc;

                    index += 3;
                    dynamic_inc++;
                }
    }
    v_indexes = index;
    fclose(fp);
    return v;
}
static float *loadTexels(const char path[]) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Could not open file : %s.\n", path);
        return NULL;
    }

    float *t = malloc(4);
    if (!t) {
        fprintf(stderr, "Could not allocate memory for Textor struct. loadTexels() -- malloc().\n");
        fclose(fp);
        return NULL;
    }
    float va, vb;
    int dynamic_inc = 1;
    int index = 0;

    char ch;
    while (!feof(fp)) {
        if ((ch = getc(fp)) == 'v')
            if ((ch = getc(fp)) == 't')
                if ((ch = getc(fp)) == ' ')
                    if (fscanf(fp, "%f %f", &va, &vb) == 2) {

                        float *temp = realloc(t, 8 * dynamic_inc);
                        if (!temp) {
                            fprintf(stderr, "Could not reallocate memory for vec4f struct array. loadTexels() -- realloc().\n");
                            fclose(fp);
                            free(t);
                            return NULL;
                        }
                        t = temp;
                        t[index] = va;
                        t[index + 1] = vb;

                        index += 2;
                        dynamic_inc++;
                    }
    }
    t_indexes = index;
    fclose(fp);
    return t;
}
static float *loadNormals(const char path[]) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Could not open file : %s.\n", path);
        return NULL;
    }

    float *n = malloc(12);
    if (!n) {
        fprintf(stderr, "Could not allocate memory for Textor struct. loadNormals() -- malloc().\n");
        fclose(fp);
        return NULL;
    }

    float va, vb, vc;
    int dynamic_inc = 1;
    int index = 0;

    char ch;
    while (!feof(fp)) {
        if ((ch = getc(fp)) == 'v')
            if ((ch = getc(fp)) == 'n')
                if ((ch = getc(fp)) == ' ')
                    if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                        float *temp = realloc(n, 12 * dynamic_inc);
                        if (!temp) {
                            fprintf(stderr, "Could not reallocate memory for vec4f struct array. loadMesh() -- realloc().\n");
                            fclose(fp);
                            free(n);
                            return NULL;
                        }
                        n = temp;
                        n[index] = va;
                        n[index + 1] = vb;
                        n[index + 2] = vc;

                        index += 3;
                        dynamic_inc++;
                    }
    }
    n_indexes = index;
    fclose(fp);
    return n;
}
static int* loadFaces(const char type[]) {
    size_t facesize = 4;
    FILE* fp = fopen(type, "r");
    if (!fp) {
        printf("Could not open file : %s.\n", type);
        return NULL;
    }

    int* f = malloc(facesize);
    if (!f) {
        printf("Could not allocate memory for Face struct. loadMesh() -- malloc().\n");
        fclose(fp);
        return NULL;
    }
    int va, vb, vc, vd, ve, vf, vg, vh, vi;
    int dynamic_inc = 1;
    int index = 0;

    char ch;
    while (!feof(fp)) {
        if ((ch = getc(fp)) == 'f')
            if ((ch = getc(fp)) == ' ')
                if (fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &va, &vb, &vc, &vd, &ve, &vf, &vg, &vh, &vi) == 9) {

                    int *temp = realloc(f, 36 * dynamic_inc);
                    if (!temp) {
                        printf("Could not reallocate memory for Face struct array. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(f);
                        return NULL;
                    }
                    f = temp;
                    f[index] = va - 1,     f[index + 1] = vb - 1, f[index + 2] = vc - 1,
                    f[index + 3] = vd - 1, f[index + 4] = ve - 1, f[index + 5] = vf - 1,
                    f[index + 6] = vg - 1, f[index + 7] = vh - 1, f[index + 8] = vi - 1;

                    index += 9;
                    dynamic_inc++;
                }
    }

    f_indexes = index;
    fclose(fp);
    return f;
}
/* Releases allocated ressources of a mesh. */
void releaseMesh(mesh* m) {
    free(m->vao);
}


