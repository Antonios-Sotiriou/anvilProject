#include "headers/components/obj.h"

/* Reads RAW obj file data from the given path and stores them in OBJ obj pointer.
It's the programmer's responsibillity to rearange the data according to hes needs.
Data must be freed when no longer needed. */
/* Very Compicated function to read an obj file with one iteration. Must be simplified for future readabillity. */
void readOBJ(OBJ *obj, const char path[]) {
    int path_length = strlen(path) + strlen(anvil_SOURCE_DIR) + 2; // Plus 2 here for the / between source dir and file location and a null termination \0.
    char* dynamic_path = malloc(path_length);
    if (!dynamic_path) {
        debug_log_error(stdout, "malloc()");
        debug_log_info(stdout, "%s\n", path);
        exit(1);
    }
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sprintf_s(dynamic_path, path_length, "%s/%s", anvil_SOURCE_DIR, path);
#elif defined(LINUX) || defined(__linux__)
    snprintf(dynamic_path, path_length, "%s/%s", anvil_SOURCE_DIR, path);
#endif

    FILE* fp = fopen(dynamic_path, "r");
    free(dynamic_path);
    if (!fp) {
        debug_log_error(stdout, "fopen()");
        exit(1);
    }

    const int entry_size = sizeof(ENTRY);
    obj->e = malloc(entry_size);
    if (!obj->e) {
        debug_log_critical(stdout, "obj->e = malloc(entry_size)");
        exit(1);
    }

    int v_inc = 1, n_inc = 1, t_inc = 1, f_inc = 1, e_inc = 1, ch_inc = 1;
    int v_index = 0, n_index = 0, t_index = 0, f_index = 0, e_index = 0, ch_index = 0, e_cache = -1, last_index = 0;

    char ch;
    while (!feof(fp)) {

        /* Identify a new ENTRY if the character that follows is a space. */
        ch = getc(fp);
        if (ch == 'o') {

            ch = getc(fp);
            if (ch == ' ') {

                e_cache++;
                /* Dynamically allocating memory for an entry of an object in the obj file. ########################################## */
                ENTRY *temp_e = realloc(obj->e, entry_size * e_inc);
                if (!temp_e) {
                    debug_log_error(stdout, "realloc(&obj->e[e_cache].e, sizeof(ENTRIES) * e_inc)");
                    free(obj->e);
                    break;
                } else {
                    obj->e = temp_e;

                    /* Allocating memory for the name to hold the obj name of the processing ENTRY. ######################################### */
                    obj->e[e_cache].cname = malloc(ch_inc);
                    if (!obj->e[e_cache].cname) {
                        debug_log_error(stdout, "obj->e[e_cache].cname = malloc(ch_inc)");
                        break;
                    } else {
                        /* Extract all the characters to  form the ENTRY name until we meet a New Line char */
                        while ((ch = getc(fp)) != '\n') {
                            char *temp = realloc(obj->e[e_cache].cname, 1 * ch_inc);
                            if (!temp) {
                                debug_log_error(stdout, "realloc(obj->e[e_cache].cname, 1 * ch_inc)");
                                free(obj->e[e_cache].cname);
                                break;
                            } else {
                                obj->e[e_cache].cname = temp;

                                obj->e[e_cache].cname[ch_index] = ch;
                                ch_index++;
                                ch_inc++;
                            }
                        }
                        obj->e[e_cache].cname[ch_index] = '\0';
                        obj->e[e_cache].c_indexes = ch_index;
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
            ch = getc(fp);
            if (ch == ' ') {
                static float va, vb, vc;
                if (fscanf(fp, "%f %f %f", &va, &vb, &vc) == 3) {

                    float *temp = realloc(obj->e[e_cache].v, 12 * v_inc);
                    if (!temp) {
                        debug_log_error(stdout, "realloc(obj->e[e_cache].v, 12 * v_inc)");
                        free(obj->e[e_cache].v);
                        break;
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
                            break;
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
                            break;
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
                        break;
                    }
                    obj->e[e_cache].f = temp;
                    obj->e[e_cache].f[f_index] = fa - 1,     obj->e[e_cache].f[f_index + 1] = fb - 1, obj->e[e_cache].f[f_index + 2] = fc - 1,
                    obj->e[e_cache].f[f_index + 3] = fd - 1, obj->e[e_cache].f[f_index + 4] = fe - 1, obj->e[e_cache].f[f_index + 5] = ff - 1,
                    obj->e[e_cache].f[f_index + 6] = fg - 1, obj->e[e_cache].f[f_index + 7] = fh - 1, obj->e[e_cache].f[f_index + 8] = fi - 1;

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
            v_index = n_index = t_index = f_index = 0;
            v_inc = n_inc = t_inc = f_inc = 1;
            last_index = e_index;
        }
    }

    fclose(fp);
    obj->e_indexes = e_index;

    //for (int i = 0; i < obj->e_indexes; i++) {
    //    printf("%s \n", obj->e[i].cname);
    //    printf("v_index: %d\n", obj->e[i].v_indexes);
    //    printf("n_index: %d\n", obj->e[i].n_indexes);
    //    printf("t_index: %d\n", obj->e[i].t_indexes);
    //    printf("f_index: %d\n", obj->e[i].f_indexes);
    //}

    printf("e_index: %d\n", e_index);
    printf("e_inc: %d\n", e_inc);
}
/* Frees OBJ allocated data releasing sources. */
void releaseOBJ(OBJ* obj) {
    for (int i = 0; i < obj->e_indexes; i++) {
        free(&obj->e[i].cname);
        free(&obj->e[i].v);
        free(&obj->e[i].n);
        free(&obj->e[i].t);
        free(&obj->e[i].f);
    }
    free(obj->e);
}


