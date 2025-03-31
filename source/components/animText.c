#include "headers/components/animText.h"

void readAnimText(animTextData *an, char path[]) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("");
        return;
    }

    /* Incremental values. an_idx starts from -1 and increments imediatly when we get to the first object.
       Thats mandatory because we want to reade the whole file in one iteration.  */
    int an_idx = -1, child_idx = 0, lc_idx = 0, rq_idx = 0, sc_idx = 0;
    char ch;
    while (!feof(fp)) {

        ch = getc(fp);
        if (ch == 'n') {
            /* Get number of objects in animText file. */
            ch = getc(fp);
            if (ch == 'o') {
                if ((ch = getc(fp)) == ' ') {
                    if (fscanf(fp, "%d", &an->number_of_objects) == 1) {
                        an->object = malloc(an->number_of_objects * sizeof(Object));
                        if (!an->object) {
                            debug_log_error(stdout, "an->object = malloc(an->number_of_objects * sizeof(animTextObject)");
                            return;
                        }
                    }
                }
            /* Get number of frames in animText file. */
            } else if (ch == 'f') {
                if ((ch = getc(fp)) == ' ') {
                    if (fscanf(fp, "%d", &an->number_of_frames) == 1);
                }
            /* Get name of each object in animText file. */
            } else if (ch == 'm') {
                /* Set the appropriate incremental values for each object's data arrays. */
                an_idx++;
                child_idx = 0, lc_idx = 0, rq_idx = 0, sc_idx = 0;

                if ((ch = getc(fp)) == ' ') {
                    int ch_inc = 1, ch_idx = 0;
                    an->object[an_idx].cname = 0;

                    while ((ch = getc(fp)) != '\n') {
                        char *temp_ch = realloc(an->object[an_idx].cname, 1 + ch_inc);
                        if (!temp_ch) {
                            debug_log_error(stdout, "char *temp_ch = realloc(an->object->cname, 1 + ch_inc)");
                            free(an->object[an_idx].cname);
                            return;
                        } else {
                            an->object[an_idx].cname = temp_ch;

                            an->object[an_idx].cname[ch_idx] = ch;
                            ch_idx++;
                            ch_inc++;
                        }
                    }
                    if (ch_idx)
                        an->object[an_idx].cname[ch_idx] = '\0';

                    an->object[an_idx].location = malloc(16 * an->number_of_frames);
                    an->object[an_idx].rotation_quaternion = malloc(16 * an->number_of_frames);
                    an->object[an_idx].scale = malloc(16 * an->number_of_frames);
                }
            /* Get number of children of each object in animText file. */
            } else if (ch == 'c') {
                if ((ch = getc(fp)) == ' ') {
                    if (fscanf(fp, "%d", &an->object[an_idx].number_of_children) == 1) {
                        if (an->object[an_idx].number_of_children > 0) {
                            an->object[an_idx].children = malloc(an->object[an_idx].number_of_children * 8);
                        }
                    }
                }
            }
        /* Get children of each object in animText file. */
        } else if (ch == 'c') {
            if ((ch = getc(fp)) == 'h') {
                if ((ch = getc(fp)) == ' ') {
                    int ch_inc = 1, ch_idx = 0;
                    an->object[an_idx].children[child_idx] = 0;

                    while ((ch = getc(fp)) != '\n') {
                        char *temp_ch = realloc(an->object[an_idx].children[child_idx], 1 + ch_inc);
                        if (!temp_ch) {
                            debug_log_error(stdout, "char *temp_ch = realloc(&an->object[an_idx].children[child_idx], 1 + ch_inc)");
                            free(an->object[an_idx].children[child_idx]);
                            return;
                        } else {
                            an->object[an_idx].children[child_idx] = temp_ch;

                            an->object[an_idx].children[child_idx][ch_idx] = ch;
                            ch_idx++;
                            ch_inc++;
                        }
                    }
                    if (ch_idx)
                        an->object[an_idx].children[child_idx][ch_idx] = '\0';
                    child_idx++;
                }
            }
        /* Get the locations. */
        } else if (ch == 'l') {
            if ((ch = getc(fp)) == 'c') {
                if ((ch = getc(fp)) == ' ') {

                    float data[4];
                    if (fscanf(fp, "%f %f %f %f", &data[0], &data[1], &data[2], &data[3]) == 4) {
                        memcpy(&an->object[an_idx].location[lc_idx], &data, 16);
                        lc_idx++;
                    }
                }
            }
        /* Get the Rotation Quaternions. */
        } else if (ch == 'r') {
            if ((ch = getc(fp)) == 'q') {
                if ((ch = getc(fp)) == ' ') {

                    float data[4];
                    if (fscanf(fp, "%f %f %f %f", &data[0], &data[1], &data[2], &data[3]) == 4) {
                        memcpy(&an->object[an_idx].rotation_quaternion[rq_idx], &data, 16);
                        rq_idx++;
                    }
                }
            }
        } else if (ch == 's') {
            if ((ch = getc(fp)) == 'c') {
                if ((ch = getc(fp)) == ' ') {

                    float data[4];
                    if (fscanf(fp, "%f %f %f %f", &data[0], &data[1], &data[2], &data[3]) == 4) {
                        memcpy(&an->object[an_idx].scale[sc_idx], &data, 16);
                        sc_idx++;
                    }
                }
            }
        }
    }

    printf("number_of_objects      : %d\n", an->number_of_objects);
    printf("number_of_frames       : %d\n", an->number_of_frames);
    for (int x = 0; x < an->number_of_objects; x++) {
        printf("cname              : %s\n", an->object[x].cname);
        printf("number_of_children : %d\n", an->object[x].number_of_children);

        printf("Children           : ");
        for (int i = 0; i < an->object[x].number_of_children; i++)
            printf("%s ", an->object[x].children[i]);
        printf("\n");

        for (int y = 0; y < an->number_of_frames; y++) {
            logvec4(an->object[x].location[y]);
        }
        for (int y = 0; y < an->number_of_frames; y++) {
            logvec4(an->object[x].rotation_quaternion[y]);
        }
        for (int y = 0; y < an->number_of_frames; y++) {
            logvec4(an->object[x].scale[y]);
        }
    }

    fclose(fp);
}
void releaseAnimText(animTextData *an) {
    for (int i = 0; i < 1; i++) {
        free(an->object[i].cname);

        for (int x = 0; x < an->object[i].number_of_children; x++)
            free(an->object[i].children[x]);

        free(an->object[i].children);
        free(an->object[i].location);
        free(an->object[i].rotation_quaternion);
        free(an->object[i].scale);
    }
    free(an->object);
}


