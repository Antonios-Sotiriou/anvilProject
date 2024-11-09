#include "headers/components/mesh.h"

static void initMesh(mesh* m);
static void loadMesh(mesh* m, const char path[]);

void createMesh(mesh *m, const char path[]) {
    /* Initializing data for all meshes */
    //initMesh(m);
    /* Load obj file to mesh. */
    loadMesh(m, path);
}
static void initMesh(mesh *m) {
    /* Assign starting coordinates for all meshes */
    //memcpy(&m->coords, &initCoords, 64);
    //m->q = unitQuat();
    //m->scale = 10.f;
}
/* Loads obj file data to a mesh. */
static void loadMesh(mesh *m, const char path[]) {
    int path_length = strlen(path) + strlen(anvil_SOURCE_DIR) + 2; // Plus 2 here for the / between source dir and file location and a null termination \0.
    char* dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        printf("Could not Create bmp file Path: < %s >! loadMesh() -- malloc().\n", path);
        return;
    }
    sprintf_s(dynamic_path, path_length, "%s/%s", anvil_SOURCE_DIR, path);

    FILE *fp = fopen(dynamic_path, "r");
    free(dynamic_path);
    if (!fp) {
        printf("Could not open file : %s. loadMesh() -- fopen().\n", path);
        return;
    }
    float *v = malloc(4);
    if (!v) {
        printf("Could not allocate memory vectors. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }
    float* n = malloc(4);
    if (!n) {
        printf("Could not allocate memory normals. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }
    float *t = malloc(4);
    if (!t) {
        printf("Could not allocate memory texels. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }
    int* f = malloc(4);
    if (!f) {
        printf("Could not allocate memory faces. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }

    static float va, vb, vc, na, nb, nc, ta, tb;
    static int fa, fb, fc, fd, fe, ff, fg, fh, fi;
    int v_inc = 1, n_inc = 1, t_inc = 1, f_inc = 1;
    int v_index = 0, n_index = 0, t_index = 0, f_index = 0;

    char ch;
    while (!feof(fp)) {
        ch = getc(fp);
        if (ch == 'v') {
            ch = getc(fp);
            if (ch == ' ') {
                if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                    float* temp = realloc(v, 12 * v_inc);
                    if (!temp) {
                        printf("Could not reallocate memory for vectors. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(v);
                        return;
                    }
                    v = temp;
                    v[v_index] = va;
                    v[v_index + 1] = vb;
                    v[v_index + 2] = vc;

                    v_index += 3;
                    v_inc++;
                }
            } else if (ch == 'n') {
                if ((ch = getc(fp)) == ' ')
                    if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                        float* temp = realloc(n, 12 * n_inc);
                        if (!temp) {
                            printf("Could not reallocate memory for normals. loadMesh() -- realloc().\n");
                            fclose(fp);
                            free(n);
                            return;
                        }
                        n = temp;
                        n[n_index] = va;
                        n[n_index + 1] = vb;
                        n[n_index + 2] = vc;

                        n_index += 3;
                        n_inc++;
                    }
            } else if (ch == 't') {
                if ((ch = getc(fp)) == ' ')
                    if (fscanf(fp, "%f %f", &ta, &tb) == 2) {

                        float* temp = realloc(t, 8 * t_inc);
                        if (!temp) {
                            printf("Could not reallocate memory for texels. loadMesh() -- realloc().\n");
                            fclose(fp);
                            free(t);
                            return;
                        }
                        t = temp;
                        t[t_index] = ta;
                        t[t_index + 1] = tb;

                        t_index += 2;
                        t_inc++;
                    }
            }
        } else if (ch == 'f') {
            if ((ch = getc(fp)) == ' ')
                if (fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &fa, &fb, &fc, &fd, &fe, &ff, &fg, &fh, &fi) == 9) {

                    int* temp = realloc(f, 36 * f_inc);
                    if (!temp) {
                        printf("Could not reallocate memory for faces. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(f);
                        return;
                    }
                    f = temp;
                    f[f_index] = fa - 1,     f[f_index + 1] = fb - 1, f[f_index + 2] = fc - 1,
                    f[f_index + 3] = fd - 1, f[f_index + 4] = fe - 1, f[f_index + 5] = ff - 1,
                    f[f_index + 6] = fg - 1, f[f_index + 7] = fh - 1, f[f_index + 8] = fi - 1;

                    f_index += 9;
                    f_inc++;
                }
        }
    }

    m->vbo_indexes = (f_index / 9) * 24;
    m->faces_indexes = m->vbo_indexes / 24;
    m->vecs_indexes = m->faces_indexes * 3;
    m->vbo_size = m->vbo_indexes * 4;

    m->vbo = malloc(m->vbo_size);
    if (!m->vbo) {
        printf("Could not create mesh vbo. loadMesh() - malloc()\n");
        return;
    }

    int index = 0, vpad, tpad;
    for (int i = 0; i < f_index; i++) {
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
    fclose(fp);
}
/* Releases allocated ressources of a mesh. */
void releaseMesh(mesh* m) {
    free(m->vbo);
    glDeleteVertexArrays(1, &m->VAO);
    glDeleteBuffers(1, &m->VBO);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}


