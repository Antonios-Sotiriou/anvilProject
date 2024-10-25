#include "headers/components/mesh.h"

/* Usefull to initialize the starting mesh coordinates. */
const static vec4 initCoords[4] = {
    { 0.f, 0.f, 0.f, 1.f },
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0., 0.f, 1.f, 0.f }
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
static vec4 *loadVectors(const char path[]);
//static vec2 *loadTexels(const char path[]);
//static vec4 *loadNormals(const char path[]);
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
}
/* Loads obj file data to a mesh. */
static void loadMesh(mesh* m, const char type[]) {
    vec4 *v = loadVectors(type);
    if (!v)
        printf("Could not reallocate Vectors array. loadMesh() - get_vectors()\n");

    //vec2 *t = loadtextors(objfile);
    //if (!t)
    //    printf("Could not create Vectors array. loadMesh() - get_textors()\n");

    //vec4 *n = loadnormals(objfile);
    //if (!n)
    //    printf("Could not create Vectors array. loadMesh() - get_normals()\n");

    int *f = loadFaces(type);
    if (!f)
        printf("Could not create Faces array. loadMesh() - get_faces()\n");

    m->face_indexes = f_indexes / 9;
    m->face = malloc(sizeof(face) * m->face_indexes);

    int index = 0;
    for (int i = 0; i < f_indexes; i += 9) {
        m->face[index].v[0] = v[f[i]];
        m->face[index].v[1] = v[f[i + 3]];
        m->face[index].v[2] = v[f[i + 6]];

        index++;
    }

    free(v);
    //free(t);
    //free(n);
    free(f);
}
static int *loadFaces(const char type[]) {
    size_t facesize = sizeof(int);
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
    int dynamic_inc = 9;
    int index = 0;

    char ch;
    while (!feof(fp)) {
        if ((ch = getc(fp)) == 'f')
            if ((ch = getc(fp)) == ' ')
                if (fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &va, &vb, &vc, &vd, &ve, &vf, &vg, &vh, &vi) == 9) {

                    f = realloc(f, facesize * dynamic_inc);
                    if (!f) {
                        printf("Could not reallocate memory for Face struct array. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(f);
                        return NULL;
                    }
                    f[index] = va - 1, f[index + 1] = vb - 1, f[index + 2] = vc - 1,
                        f[index + 3] = vd - 1, f[index + 4] = ve - 1, f[index + 5] = vf - 1,
                        f[index + 6] = vg - 1, f[index + 7] = vh - 1, f[index + 8] = vi - 1;

                    index += 9;
                    dynamic_inc += 9;
                }
    }

    f_indexes = index;
    fclose(fp);
    return f;
}
static vec4 *loadVectors(const char type[]) {
    size_t vecsize = sizeof(vec4);
    FILE *fp = fopen(type, "r");
    if (!fp) {
        printf("Could not open file : %s.\n", type);
        return NULL;
    }

    vec4 *v = malloc(vecsize);
    if (!v) {
        printf("Could not allocate memory for vec4f struct. get_vectors() -- calloc().\n");
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

                    v = realloc(v, vecsize * dynamic_inc);
                    if (!v) {
                        printf("Could not reallocate memory for vec4f struct array. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(v);
                        return NULL;
                    }
                    v[index].m128_f32[0] = va;
                    v[index].m128_f32[1] = vb;
                    v[index].m128_f32[2] = vc;
                    v[index].m128_f32[3] = 1.00;

                    index++;
                    dynamic_inc++;
                }
    }
    v_indexes = index;
    fclose(fp);
    return v;
}
/* Releases allocated ressources of a mesh. */
void releaseMesh(mesh* m) {
    free(m->vec4);
    free(m->face);
}


