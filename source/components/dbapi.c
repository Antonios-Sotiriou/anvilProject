#include "headers/components/dbapi.h"

/* Function pointer to dispatch the appropriate function, according to the type of Table. */
static int (*callback)(void*, int, char**, char**);
static int (*sscanf_os)(const char*, const char*, ...);
/* Loads models from the Database model Table to the Global SCENE. */
static int modelCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    //for (int i = 0; i < argc; i++) {
    //    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    //}
    //printf("\n");
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sscanf_os = sscanf_s;
#elif defined(LINUX) || defined(__linux__)
    sscanf_os = sscanf;
#endif

    int idx = *(int*)NotUsed; // Increment value depending on how much times this callback was called. Every invocation populates a mesh with data.
    int name_index = 0;
    for (int i = 0; i < argc; i++) {
        if (strncmp(azColName[i], "pk", 2) == 0) {
            SCENE.model[idx].pk = atoi(argv[i]);
        } else if (strncmp(azColName[i], "type", 4) == 0) {
            SCENE.model[idx].type = atoi(argv[i]);
        } else if (strncmp(azColName[i], "coords", 6) == 0) {
            float vecs[16];
            sscanf_os(argv[i], "{%f %f %f %f} {%f %f %f %f} {%f %f %f %f} {%f %f %f %f}",
                &vecs[0], &vecs[1], &vecs[2], &vecs[3],&vecs[4], &vecs[5], &vecs[6], &vecs[7],
                &vecs[8], &vecs[9], &vecs[10], &vecs[11],&vecs[12], &vecs[13], &vecs[14], &vecs[15]
            );
            memcpy(&SCENE.model[idx].coords, vecs, 64);
        } else if (strncmp(azColName[i], "quat", 4) == 0) {
            float q[4];
            sscanf_os(argv[i], "{%f %f %f %f}", &q[0], &q[1], &q[2], &q[3]);
            memcpy(&SCENE.model[idx].q, q, 16);
            memcpy(&SCENE.model[idx].rigid.q, q, 16);
        } else if (strncmp(azColName[i], "scale", 5) == 0) {
            SCENE.model[idx].scale = strtof(argv[i], NULL);
        } else if (strncmp(azColName[i], "outer_radius", 12) == 0) {
            float rad2 = SCENE.model[idx].scale * SCENE.model[idx].scale;
            SCENE.model[idx].outer_radius = sqrtf(rad2 + rad2);
        } else if (strncmp(azColName[i], "state", 5) == 0) {
            SCENE.model[idx].rigid.state = atoi(argv[i]);
        } else if (strncmp(azColName[i], "visible", 7) == 0) {
            SCENE.model[idx].visible = atoi(argv[i]);
        } else if (strncmp(azColName[i], "velocity", 8) == 0) {
            float v[4];
            sscanf_os(argv[i], "{%f %f %f %f}", &v[0], &v[1], &v[2], &v[3]);
            memcpy(&SCENE.model[idx].rigid.velocity, v, 16);
        } else if (strncmp(azColName[i], "rot_angle", 9) == 0) {
            SCENE.model[idx].rigid.rot_angle = strtof(argv[i], NULL);
        } else if (strncmp(azColName[i], "cname", 5) == 0) {
            name_index = i;
             //if (SCENE.model[idx].type == MESH_TYPE_TERRAIN) {
             //    createTerrain(&SCENE.model[idx], argv[i]);
             //} else {
             //    createMesh(&SCENE.model[idx], argv[i]);
             //}
             //if (SCENE.model[idx].rigid.state == ENABLED)
             //    loadmeshRigid(&SCENE.model[idx], argv[i]);
        } else if (strncmp(azColName[i], "length_cname", 12) == 0) {
            SCENE.model[idx].length_cname = atoi(argv[i]);
        }
    }
    /* Compose cname with a NULL terminating string. */
    SCENE.model[idx].cname = malloc(SCENE.model[idx].length_cname + 1); // +1 For the NULL terminating character also.
    if (!SCENE.model[idx].cname) {
        debug_log_error(stdout, "SCENE.model[idx].cname = malloc(SCENE.model[idx].length_cname + 1)");
        return 1;
    }
    memcpy(SCENE.model[idx].cname, argv[name_index], SCENE.model[idx].length_cname + 1);

    *(int*)NotUsed += 1;

    return 0;
}
/* Loads meshes from the Database mesh Table. */
static int meshCallback(void* NotUsed, int argc, char** argv, char** azColName) {
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    sscanf_os = sscanf_s;
#elif defined(LINUX) || defined(__linux__)
    sscanf_os = sscanf;
#endif

    int idx = *(int*)NotUsed; // Increment value depending on how much times this callback was called. Every invocation populates a mesh with data.

    //for (int i = 0; i < argc; i++) {
    //    if (strncmp(azColName[i], "pk", 2) == 0) {
    //        SCENE.mesh[idx].pk = atoi(argv[i]);
    //    } else if (strncmp(azColName[i], "type", 4) == 0) {
    //        SCENE.mesh[idx].type = atoi(argv[i]);
    //    } else if (strncmp(azColName[i], "coords", 6) == 0) {
    //        float vecs[16];
    //        sscanf_os(argv[i], "{%f %f %f %f} {%f %f %f %f} {%f %f %f %f} {%f %f %f %f}",
    //            &vecs[0], &vecs[1], &vecs[2], &vecs[3],&vecs[4], &vecs[5], &vecs[6], &vecs[7],
    //            &vecs[8], &vecs[9], &vecs[10], &vecs[11],&vecs[12], &vecs[13], &vecs[14], &vecs[15]
    //        );
    //        memcpy(&SCENE.mesh[idx].coords, vecs, 64);
    //    } else if (strncmp(azColName[i], "quat", 4) == 0) {
    //        float q[4];
    //        sscanf_os(argv[i], "{%f %f %f %f}", &q[0], &q[1], &q[2], &q[3]);
    //        memcpy(&SCENE.mesh[idx].q, q, 16);
    //        memcpy(&SCENE.mesh[idx].rigid.q, q, 16);
    //    } else if (strncmp(azColName[i], "scale", 5) == 0) {
    //        SCENE.mesh[idx].scale = strtof(argv[i], NULL);
    //    } else if (strncmp(azColName[i], "outer_radius", 12) == 0) {
    //        float rad2 = SCENE.mesh[idx].scale * SCENE.mesh[idx].scale;
    //        SCENE.mesh[idx].outer_radius = sqrtf(rad2 + rad2);
    //    } else if (strncmp(azColName[i], "state", 5) == 0) {
    //        SCENE.mesh[idx].rigid.state = atoi(argv[i]);
    //    } else if (strncmp(azColName[i], "velocity", 8) == 0) {
    //        float v[4];
    //        sscanf_os(argv[i], "{%f %f %f %f}", &v[0], &v[1], &v[2], &v[3]);
    //        memcpy(&SCENE.mesh[idx].rigid.velocity, v, 16);
    //    } else if (strncmp(azColName[i], "rot_angle", 9) == 0) {
    //        SCENE.mesh[idx].rigid.rot_angle = strtof(argv[i], NULL);
    //    } else if (strncmp(azColName[i], "cname", 5) == 0) {
    //         if (SCENE.mesh[idx].type == MESH_TYPE_TERRAIN) {
    //             createTerrain(&SCENE.mesh[idx], argv[i]);
    //         } else {
    //             createMesh(&SCENE.mesh[idx], argv[i]);
    //         }
    //         if (SCENE.mesh[idx].rigid.state == ENABLED)
    //             loadmeshRigid(&SCENE.mesh[idx], argv[i]);
    //    }
    //}

    *(int*)NotUsed += 1;

    return 0;
}
const int dbcountTableRows(const char path[], const char sql_cmd[]) {
    sqlite3* db;
    sqlite3_stmt* stmt = { 0 };
    int rc, rows = 0;

    rc = sqlite3_open_v2(path, &db, SQLITE_OPEN_READONLY, NULL);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql_cmd, strlen(sql_cmd), &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error sqlite3_prepare_v2().");
        printf("%s\n", sqlite3_errmsg(db));
    }

    while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
        rows = sqlite3_column_int(stmt, 0);
        if (rc == SQLITE_ERROR) {
            printf("SQL error sqlite3_step().");
            printf("%s\n", sqlite3_errmsg(db));
        }
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return rows;
}
/* Execute sql_cmd to the path Database. */
void dbloadTable(const char path[], const int type, const char sql_cmd[]) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open(path, &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (type == MODEL_TABLE)
        callback = &modelCallback;
    if (type == MESH_TABLE)
        callback = &meshCallback;

    int param = 0; // Passing an increment value to the calback, to initialize the a SCENE mesh every invocation.
    rc = sqlite3_exec(db, sql_cmd, callback, &param, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    return;
}


