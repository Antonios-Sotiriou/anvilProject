#include "headers/components/obj.h"

/* Reads RAW obj file data from the given path and stores them in OBJ obj pointer.
It's the programmer's responsibillity to rearange the data according to hes needs.
Data must be freed when no longer needed. */
/* Very Compicated function to read an obj file with one iteration. Must be simplified for future readabillity. */
OBJ *readOBJ(const char path[]) {
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

    FILE *fp = fopen(dynamic_path, "r");
    free(dynamic_path);
    if (!fp) {
        debug_log_error(stdout, "fopen()");
        exit(1);
    }

    int v_inc = 1, n_inc = 1, t_inc = 1, f_inc = 1, e_inc = 1, ch_inc = 2;
    int v_index = 0, n_index = 0, t_index = 0, f_index = 0, e_index = 0, ch_index = 0, e_cache = -1, last_index = 0;

    char ch;
    while (!feof(fp)) {

        /* Identify a new ENTRY if the character that follows is a space. */
        ch = getc(fp);
        if (ch == 'o') {

            ch = getc(fp);
            if (ch == ' ') {
                /* Extract all the characters to  form the ENTRY name until we meet a New Line char */
                while ((ch = getc(fp)) != '\n');
                /* Update and increment the ENTRIES counters. */
                e_index++;
                e_inc++;
            }
        }
    }

    const int entry_size = sizeof(OBJ);
    OBJ *obj = malloc(entry_size * e_index);
    if (!obj) {
        debug_log_critical(stdout, "obj->e = malloc(entry_size)");
        exit(1);
    }

    for (int i = 0; i < e_index; i++) {
        obj[i].v = malloc(4);
        obj[i].n = malloc(4);
        obj[i].t = malloc(4);
        obj[i].f = malloc(4);
    }

    //while (!feof(fp)) {

    //    /* Identify a new ENTRY if the character that follows is a space. */
    //    ch = getc(fp);
    //    if (ch == 'o') {

    //        ch = getc(fp);
    //        if (ch == ' ') {

    //            e_cache += 1;
    //            /* Allocating memory for the name to hold the obj name of the processing ENTRY. ######################################### */
    //            //obj->e[e_cache].cname = malloc(ch_inc);
    //            //if (!obj->e[e_cache].cname) {
    //            //    debug_log_error(stdout, "obj->e[e_cache].cname = malloc(ch_inc)");
    //            //    break;
    //            //} else {
    //                /* Extract all the characters to  form the ENTRY name until we meet a New Line char */
    //                while ((ch = getc(fp)) != '\n') {
    //                    char *temp = realloc(obj->e[e_cache].cname, 1 * ch_inc);
    //                    if (!temp) {
    //                        debug_log_error(stdout, "realloc(obj->e[e_cache].cname, 1 * ch_inc)");
    //                        free(obj->e[e_cache].cname);
    //                        exit(1);
    //                    } else {
    //                        obj->e[e_cache].cname = temp;

    //                        obj->e[e_cache].cname[ch_index] = ch;
    //                        ch_index++;
    //                        ch_inc++;
    //                    }
    //                //}
    //                obj->e[e_cache].cname[ch_index] = '\0';
    //                obj->e[e_cache].c_indexes = ch_index;
    //                /* Reset the char count variables for the next ENTRY. */
    //                ch_inc = 1;
    //                ch_index = 0;

    //                /* Allocating memory for the arrays to hold the obj data of the processing ENTRY. ######################################### */
    //                obj->e[e_cache].v = malloc(4);
    //                obj->e[e_cache].n = malloc(4);
    //                obj->e[e_cache].t = malloc(4);
    //                obj->e[e_cache].f = malloc(4);
    //            }
    //            /* Update and increment the ENTRIES counters. */
    //            e_index++;
    //            e_inc++;
    //        }
    //    } else {
    //        continue;
    //    }

    //    if (e_index && e_index != last_index) {
    //        /* Reset counters for the next iteration. */
    //        v_index = n_index = t_index = f_index = 0;
    //        v_inc = n_inc = t_inc = f_inc = 1;
    //        last_index = e_index;
    //    }
    //}

    fclose(fp);
    obj->e_indexes = e_index;

    printf("%d\n", e_index);
    printf("%d\n", e_inc);
    //printf("obj->e_indexes: %d\n", obj->e_indexes);
    return obj;
}
/* Frees OBJ allocated data releasing sources. */
void releaseOBJ(OBJ *obj) {
    //printf("obj->e_indexes: %d\n", obj->e_indexes);
    if (obj->e_indexes)
        for (int i = 0; i < obj->e_indexes; i++) {
            printf("%p\n", &obj[i]);
            //free(&obj[i]);
            free(obj[i].v);
            free(obj[i].n);
            free(obj[i].t);
            free(obj[i].f);
        }
    free(obj);
}


