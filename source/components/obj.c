#include "headers/components/obj.h"

/* Reads RAW obj file data from the given path and stores them in OBJ o pointer.
It's the programmer's responsibillity to rearange the data according to hes needs.
Data must be freed when no longer needed. */
void readOBJ(OBJ *o, const char path[]) {
    int path_length = strlen(path) + strlen(anvil_SOURCE_DIR) + 2; // Plus 2 here for the / between source dir and file location and a null termination \0.
    char *dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "malloc()");
        debug_log_info(stdout, "%s\n", path);
        return;
    }
    sprintf_s(dynamic_path, path_length, "%s/%s", anvil_SOURCE_DIR, path);

    FILE *fp = fopen(dynamic_path, "r");
    free(dynamic_path);
    if (!fp) {
        debug_log_error(stdout, "fopen()");
        return;
    }

    o->v = malloc(4);
    o->n = malloc(4);
    o->t = malloc(4);
    o->f = malloc(4);

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
                        debug_log_error(stdout, "malloc()");
                        free(o->v);
                        break;
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
                            debug_log_error(stdout, "malloc()");
                            free(o->n);
                            break;
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
                            debug_log_error(stdout, "malloc()");
                            free(o->t);
                            break;
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
                        debug_log_error(stdout, "malloc()");
                        free(o->f);
                        break;
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
    fclose(fp);
    o->v_indexes = v_index;
    o->n_indexes = n_index;
    o->t_indexes = t_index;
    o->f_indexes = f_index;
}
/* Frees OBJ allocated data releasing sources. */
void releaseOBJ(OBJ *o) {
    free(o->v);
    free(o->n);
    free(o->t);
    free(o->f);
}

