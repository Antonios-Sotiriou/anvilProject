#include "headers/components/obj.h"

/* Reades obj file on the given path and stores data in OBJ o pointer. Data must be freed when no longer needed. */
void readOBJ(OBJ *o, const char path[]) {
    int path_length = strlen(path) + strlen(anvil_SOURCE_DIR) + 2; // Plus 2 here for the / between source dir and file location and a null termination \0.
    char* dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        printf("Could not Create bmp file Path: < %s >! loadMesh() -- malloc().\n", path);
        return;
    }
    sprintf_s(dynamic_path, path_length, "%s/%s", anvil_SOURCE_DIR, path);

    FILE* fp = fopen(dynamic_path, "r");
    free(dynamic_path);
    if (!fp) {
        printf("Could not open file : %s. loadMesh() -- fopen().\n", path);
        return;
    }
    o->v = malloc(4);
    if (!o->v) {
        printf("Could not allocate memory vectors. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }
    o->n = malloc(4);
    if (!o->n) {
        printf("Could not allocate memory normals. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }
    o->t = malloc(4);
    if (!o->t) {
        printf("Could not allocate memory texels. loadMesh() -- malloc().\n");
        fclose(fp);
        return;
    }
    o->f = malloc(4);
    if (!o->f) {
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

                    float *temp = realloc(o->v, 12 * v_inc);
                    if (!temp) {
                        printf("Could not reallocate memory for vectors. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(o->v);
                        return;
                    }
                    o->v = temp;
                    o->v[v_index] = va;
                    o->v[v_index + 1] = vb;
                    o->v[v_index + 2] = vc;

                    v_index += 3;
                    v_inc++;
                }
            }
            else if (ch == 'n') {
                if ((ch = getc(fp)) == ' ')
                    if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                        float *temp = realloc(o->n, 12 * n_inc);
                        if (!temp) {
                            printf("Could not reallocate memory for normals. loadMesh() -- realloc().\n");
                            fclose(fp);
                            free(o->n);
                            return;
                        }
                        o->n = temp;
                        o->n[n_index] = va;
                        o->n[n_index + 1] = vb;
                        o->n[n_index + 2] = vc;

                        n_index += 3;
                        n_inc++;
                    }
            }
            else if (ch == 't') {
                if ((ch = getc(fp)) == ' ')
                    if (fscanf(fp, "%f %f", &ta, &tb) == 2) {

                        float *temp = realloc(o->t, 8 * t_inc);
                        if (!temp) {
                            printf("Could not reallocate memory for texels. loadMesh() -- realloc().\n");
                            fclose(fp);
                            free(o->t);
                            return;
                        }
                        o->t = temp;
                        o->t[t_index] = ta;
                        o->t[t_index + 1] = tb;

                        t_index += 2;
                        t_inc++;
                    }
            }
        }
        else if (ch == 'f') {
            if ((ch = getc(fp)) == ' ')
                if (fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &fa, &fb, &fc, &fd, &fe, &ff, &fg, &fh, &fi) == 9) {

                    int *temp = realloc(o->f, 36 * f_inc);
                    if (!temp) {
                        printf("Could not reallocate memory for faces. loadMesh() -- realloc().\n");
                        fclose(fp);
                        free(o->f);
                        return;
                    }
                    o->f = temp;
                    o->f[f_index] = fa - 1,     o->f[f_index + 1] = fb - 1, o->f[f_index + 2] = fc - 1,
                    o->f[f_index + 3] = fd - 1, o->f[f_index + 4] = fe - 1, o->f[f_index + 5] = ff - 1,
                    o->f[f_index + 6] = fg - 1, o->f[f_index + 7] = fh - 1, o->f[f_index + 8] = fi - 1;

                    f_index += 9;
                    f_inc++;
                }
        }
    }
    o->v_inx = v_index;
    o->n_inx = n_index;
    o->t_inx = t_index;
    o->f_inx = f_index;
}
/* Frees OBJ allocated data releasing sources. */
void releaseOBJ(OBJ *o) {
    free(o->v);
    free(o->n);
    free(o->t);
    free(o->f);
}

