#include "headers/components/obj.h"

/* Reads RAW obj file data from the given path and stores them in OBJ obj pointer.
It's the programmer's responsibillity to rearange the data according to hes needs.
Data must be freed when no longer needed. */
/* Very Compicated function to read an obj file with one iteration. Must be simplified for future readabillity. */
void readOBJ(OBJ *obj, const char path[]) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        debug_log_error(stdout, "fopen()");
        exit(1);
    }

    const int entry_size = sizeof(ENTRY);
    int v_inc = 1, n_inc = 1, t_inc = 1, f_inc = 1, e_inc = 1, ch_inc = 1;
    int v_index = 0, n_index = 0, t_index = 0, f_index = 0, e_index = 0, ch_index = 0, e_cache = -1, last_index = 0;
    int f_v = 1, f_n = 1, f_t = 1;

    char ch;
    while (!feof(fp)) {

        /* Identify a new ENTRY if the character that follows is a space. */
        ch = getc(fp);
        if (ch == 'o') {

            if ((ch = getc(fp)) == ' ') {

                e_cache++;
                /* Dynamically allocating memory for an entry of an object in the obj file. ########################################## */
                ENTRY *temp_e = realloc(obj->e, entry_size * e_inc);
                if (!temp_e) {
                    debug_log_error(stdout, "realloc(&obj->e[e_cache].e, sizeof(ENTRIES) * e_inc)");
                    free(obj->e);
                    exit(1);
                } else {
                    obj->e = temp_e;

                    /* Allocating memory for the name to hold the obj name of the processing ENTRY. ######################################### */
                    obj->e[e_cache].cname = malloc(ch_inc);
                    if (!obj->e[e_cache].cname) {
                        debug_log_error(stdout, "obj->e[e_cache].cname = malloc(ch_inc)");
                        exit(1);
                    } else {
                        /* Extract all the characters to  form the ENTRY name until we meet a New Line char */
                        while ((ch = getc(fp)) != '\n') {
                            char *temp = realloc(obj->e[e_cache].cname, 1 + ch_inc);
                            if (!temp) {
                                debug_log_error(stdout, "realloc(obj->e[e_cache].cname, 1 * ch_inc)");
                                free(obj->e[e_cache].cname);
                                exit(1);
                            } else {
                                obj->e[e_cache].cname = temp;

                                obj->e[e_cache].cname[ch_index] = ch;
                                ch_index++;
                                ch_inc++;
                            }
                        }
                        obj->e[e_cache].cname[ch_index] = '\0';
                        obj->e[e_cache].length_cname = ch_index;
                        /* Reset the char count variables for the next ENTRY. */
                        ch_inc = 1;
                        ch_index = 0;

                        /* Allocating memory for the arrays to hold the obj data of the processing ENTRY. ######################################### */
                        obj->e[e_cache].v = malloc(4);
                        obj->e[e_cache].n = malloc(4);
                        obj->e[e_cache].t = malloc(4);
                        obj->e[e_cache].f = malloc(4);
                    }
                    /* Update and increment the ENTRIES counters. */
                    e_index++;
                    e_inc++;
                }
            }
        } else if (ch == 'v') {
            if ((ch = getc(fp)) == ' ') {
                static float va, vb, vc;
                if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                    float *temp = realloc(obj->e[e_cache].v, 12 * v_inc);
                    if (!temp) {
                        debug_log_error(stdout, "realloc(obj->e[e_cache].v, 12 * v_inc)");
                        free(obj->e[e_cache].v);
                        exit(1);
                    }
                    obj->e[e_cache].v = temp;
                    obj->e[e_cache].v[v_index] = va;
                    obj->e[e_cache].v[v_index + 1] = vb;
                    obj->e[e_cache].v[v_index + 2] = vc;

                    v_index += 3;
                    obj->e[e_cache].v_indexes = v_index;
                    v_inc++;
                }
            } else if (ch == 'n') {
                if ((ch = getc(fp)) == ' ') {
                    static float na, nb, nc;
                    if (fscanf(fp, "%f %f %f", &na, &nb, &nc) == 3) {

                        float *temp = realloc(obj->e[e_cache].n, 12 * n_inc);
                        if (!temp) {
                            debug_log_error(stdout, "realloc(obj->e[e_cache].n, 12 * n_inc)");
                            free(obj->e[e_cache].n);
                            exit(1);
                        }
                        obj->e[e_cache].n = temp;
                        obj->e[e_cache].n[n_index] = na;
                        obj->e[e_cache].n[n_index + 1] = nb;
                        obj->e[e_cache].n[n_index + 2] = nc;

                        n_index += 3;
                        obj->e[e_cache].n_indexes = n_index;
                        n_inc++;
                    }
                }
            } else if (ch == 't') {
                if ((ch = getc(fp)) == ' ') {
                    static float ta, tb;
                    if (fscanf(fp, "%f %f", &ta, &tb) == 2) {

                        float *temp = realloc(obj->e[e_cache].t, 8 * t_inc);
                        if (!temp) {
                            debug_log_error(stdout, "realloc(obj->e[e_cache].t, 8 * t_inc)");
                            free(obj->e[e_cache].t);
                            exit(1);
                        }
                        obj->e[e_cache].t = temp;
                        obj->e[e_cache].t[t_index] = ta;
                        obj->e[e_cache].t[t_index + 1] = tb;

                        t_index += 2;
                        obj->e[e_cache].t_indexes = t_index;
                        t_inc++;
                    }
                }
            }
        } else if (ch == 'f') {
            if ((ch = getc(fp)) == ' ') {
                static int fa, fb, fc, fd, fe, ff, fg, fh, fi;
                if (fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &fa, &fb, &fc, &fd, &fe, &ff, &fg, &fh, &fi) == 9) {

                    int *temp = realloc(obj->e[e_cache].f, 36 * f_inc);
                    if (!temp) {
                        debug_log_error(stdout, "realloc(obj->e[e_cache].f, 36 * f_inc");
                        free(obj->e[e_cache].f);
                        exit(1);
                    }
                    obj->e[e_cache].f = temp;
                    obj->e[e_cache].f[f_index] = fa - f_v,     obj->e[e_cache].f[f_index + 1] = fb - f_t, obj->e[e_cache].f[f_index + 2] = fc - f_n,
                    obj->e[e_cache].f[f_index + 3] = fd - f_v, obj->e[e_cache].f[f_index + 4] = fe - f_t, obj->e[e_cache].f[f_index + 5] = ff - f_n,
                    obj->e[e_cache].f[f_index + 6] = fg - f_v, obj->e[e_cache].f[f_index + 7] = fh - f_t, obj->e[e_cache].f[f_index + 8] = fi - f_n;

                    f_index += 9;
                    obj->e[e_cache].f_indexes = f_index;
                    f_inc++;
                }
            }
        } else {
            continue;
        }

        if (e_index && e_index != last_index) {
            /* Reset counters for the next iteration. */
            f_v += v_index / 3, f_n += n_index / 3, f_t += t_index / 2;
            v_index = n_index = t_index = f_index = 0;
            v_inc = n_inc = t_inc = f_inc = 1;
            last_index = e_index;
        }
    }

    fclose(fp);
    obj->e_indexes = e_index;
}
/* Frees OBJ allocated data releasing sources. */
void releaseOBJ(OBJ *obj) {
    for (int i = 0; i < obj->e_indexes; i++) {
        free(obj->e[i].cname);
        free(obj->e[i].v);
        free(obj->e[i].n);
        free(obj->e[i].t);
        free(obj->e[i].f);
    }
    free(obj->e);
}


