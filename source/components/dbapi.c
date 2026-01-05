#include "headers/components/dbapi.h"

/* Function pointer to dispatch the appropriate function, according to the type of Table. */
static int (*callback)(void*, int, char**, char**);
// Pointer to use for initialization of the entries of terrains in the database.
static TerrainInitInfo* temp;

/* Loads terrains from the Database terrain Table to the Global params->s-> */
static int terrainCallback(void *NotUsed, int argc, char **argv, char **azColName) {

    DBparams *params = (DBparams*)NotUsed;
    int name_index = 0;
    for (int i = 0; i < argc; i++) {
        if (strncmp(azColName[i], "pk", 2) == 0) {
            params->s->model[params->s->model_indexes].pk = atoi(argv[i]);
        } else if (strncmp(azColName[i], "asset_type", 10) == 0) {
            params->s->model[params->s->model_indexes].asset_type = atoi(argv[i]);
        } else if (strncmp(azColName[i], "model_type", 10) == 0) {
            params->s->model[params->s->model_indexes].model_type = atoi(argv[i]);
        } else if (strncmp(azColName[i], "coords", 6) == 0) {
            float vecs[16];
            anvil_scanf(argv[i], "{%f %f %f %f} {%f %f %f %f} {%f %f %f %f} {%f %f %f %f}",
                &vecs[0], &vecs[1], &vecs[2], &vecs[3], &vecs[4], &vecs[5], &vecs[6], &vecs[7],
                &vecs[8], &vecs[9], &vecs[10], &vecs[11], &vecs[12], &vecs[13], &vecs[14], &vecs[15]
            );
            memcpy(&params->s->model[params->s->model_indexes].coords, vecs, 64);
        } else if (strncmp(azColName[i], "quat", 4) == 0) {
            float q[4];
            anvil_scanf(argv[i], "{%f %f %f %f}", &q[0], &q[1], &q[2], &q[3]);
            params->s->model[params->s->model_indexes].q = rotationQuat(q[0], q[1], q[2], q[3]);
            params->s->model[params->s->model_indexes].rigid.q = params->s->model[params->s->model_indexes].q;
        } else if (strncmp(azColName[i], "scale", 5) == 0) {
            float s[4];
            anvil_scanf(argv[i], "{%f %f %f %f}", &s[0], &s[1], &s[2], &s[3]);
            params->s->model[params->s->model_indexes].scale = setvec4(s[0], s[1], s[2], s[3]);
        } else if (strncmp(azColName[i], "outer_radius", 12) == 0) {
            float outer_rad = vec4ExtractX(params->s->model[params->s->model_indexes].scale) * vec4ExtractX(params->s->model[params->s->model_indexes].scale); // Posible bugg here after we changed scale to vec4
            params->s->model[params->s->model_indexes].outer_radius = sqrtf(outer_rad + outer_rad);
        } else if (strncmp(azColName[i], "owns_rigid", 10) == 0) {
            params->s->model[params->s->model_indexes].owns_rigid = atoi(argv[i]);
        } else if (strncmp(azColName[i], "visible", 7) == 0) {
            params->s->model[params->s->model_indexes].visible = atoi(argv[i]);
        } else if (strncmp(azColName[i], "velocity", 8) == 0) {
            float v[4];
            anvil_scanf(argv[i], "{%f %f %f %f}", &v[0], &v[1], &v[2], &v[3]);
            memcpy(&params->s->model[params->s->model_indexes].velocity, v, 16);
        } else if (strcmp(azColName[i], "cname") == 0) {
            name_index = i;
        } else if (strncmp(azColName[i], "cname_length", 12) == 0) {
            params->s->model[params->s->model_indexes].cname_length = atoi(argv[i]);
        } else if (strncmp(azColName[i], "owns_anim", 9) == 0) {
            params->s->model[params->s->model_indexes].owns_anim = atoi(argv[i]);
        } else if (strncmp(azColName[i], "width", 5) == 0) {
            params->s->t.vec_width = atoi(argv[i]);
        } else if (strncmp(azColName[i], "height", 6) == 0) {
            params->s->t.vec_height = atoi(argv[i]);
        } else if (strncmp(azColName[i], "quad_cols", 9) == 0) {
            params->s->t.quad_cols = atoi(argv[i]);
        } else if (strncmp(azColName[i], "quad_rows", 9) == 0) {
            params->s->t.quad_rows = atoi(argv[i]);
        } else if (strncmp(azColName[i], "quads_indexes", 13) == 0) {
            params->s->t.quad_indexes = atoi(argv[i]);
        }
    }
    /* Compose cname with a NULL terminating string. */
    params->s->model[params->s->model_indexes].cname = malloc(params->s->model[params->s->model_indexes].cname_length + 1); // +1 For the NULL terminating character also.
    if (!params->s->model[params->s->model_indexes].cname) {
        debug_log_error(stdout, "params->s->model[%d].cname = malloc(params->s->model[params->s->model_indexes].cname_length + 1)");
        return 1;
    }
    memcpy(params->s->model[params->s->model_indexes].cname, argv[name_index], params->s->model[params->s->model_indexes].cname_length + 1);


    params->s->t.quad = calloc(params->s->t.quad_indexes, sizeof(Quad));

    return 0;
}
/* Loads models from the Database model Table to the Global params->s-> */
static int modelCallback(void *NotUsed, int argc, char **argv, char **azColName) {

    DBparams *params = (DBparams*)NotUsed;
    int name_index = 0;
    for (int i = 0; i < argc; i++) {
        if (strncmp(azColName[i], "pk", 2) == 0) {
            params->s->model[params->index].pk = atoi(argv[i]);
        } else if (strncmp(azColName[i], "asset_type", 10) == 0) {
            params->s->model[params->index].asset_type = atoi(argv[i]);
        } else if (strncmp(azColName[i], "model_type", 10) == 0) {
            params->s->model[params->index].model_type = atoi(argv[i]);
        } else if (strncmp(azColName[i], "coords", 6) == 0) {
            float vecs[16];
            anvil_scanf(argv[i], "{%f %f %f %f} {%f %f %f %f} {%f %f %f %f} {%f %f %f %f}",
                &vecs[0], &vecs[1], &vecs[2], &vecs[3], &vecs[4], &vecs[5], &vecs[6], &vecs[7],
                &vecs[8], &vecs[9], &vecs[10], &vecs[11], &vecs[12], &vecs[13], &vecs[14], &vecs[15]
            );
            memcpy(&params->s->model[params->index].coords, vecs, 64);
        } else if (strncmp(azColName[i], "quat", 4) == 0) {
            float q[4];
            anvil_scanf(argv[i], "{%f %f %f %f}", &q[0], &q[1], &q[2], &q[3]);
            params->s->model[params->index].q = rotationQuat(q[0], q[1], q[2], q[3]);
            params->s->model[params->index].rigid.q = params->s->model[params->index].q;
        } else if (strncmp(azColName[i], "scale", 5) == 0) {
            float s[4];
            anvil_scanf(argv[i], "{%f %f %f %f}", &s[0], &s[1], &s[2], &s[3]);
            params->s->model[params->index].scale = setvec4(s[0], s[1], s[2], s[3]);
        } else if (strncmp(azColName[i], "outer_radius", 12) == 0) {
            float outer_rad = vec4ExtractX(params->s->model[params->index].scale) * vec4ExtractX(params->s->model[params->index].scale); // Posible bugg here after we changed scale to vec4
            params->s->model[params->index].outer_radius = sqrtf(outer_rad + outer_rad);
        } else if (strncmp(azColName[i], "owns_rigid", 10) == 0) {
            params->s->model[params->index].owns_rigid = atoi(argv[i]);
        } else if (strncmp(azColName[i], "visible", 7) == 0) {
            params->s->model[params->index].visible = atoi(argv[i]);
        } else if (strncmp(azColName[i], "velocity", 8) == 0) {
            float v[4];
            anvil_scanf(argv[i], "{%f %f %f %f}", &v[0], &v[1], &v[2], &v[3]);
            memcpy(&params->s->model[params->index].velocity, v, 16);
        } else if (strcmp(azColName[i], "cname") == 0) {
            name_index = i;
        } else if (strncmp(azColName[i], "cname_length", 12) == 0) {
            params->s->model[params->index].cname_length = atoi(argv[i]);
        } else if (strncmp(azColName[i], "owns_anim", 9) == 0) {
            params->s->model[params->index].owns_anim = atoi(argv[i]);
        }
    }
    /* Compose cname with a NULL terminating string. */
    params->s->model[params->index].cname = malloc(params->s->model[params->index].cname_length + 1); // +1 For the NULL terminating character also.
    if (!params->s->model[params->index].cname) {
        debug_log_error(stdout, "params->s->model[%d].cname = malloc(params->s->model[idx].cname_length + 1)");
        return 1;
    }
    memcpy(params->s->model[params->index].cname, argv[name_index], params->s->model[params->index].cname_length + 1);

    params->index += 1; // Increment value depending on how much times this callback was called. Every invocation populates a mesh with data.

    return 0;
}
/* Loads meshes from the Database mesh Table. */
static int meshCallback(void *NotUsed, int argc, char **argv, char **azColName) {

    // int idx = *(int*)NotUsed; // Increment value depending on how much times this callback was called. Every invocation populates a mesh with data.

    //for (int i = 0; i < argc; i++) {
    //    if (strncmp(azColName[i], "pk", 2) == 0) {
    //        params->s->mesh[idx].pk = atoi(argv[i]);
    //    } else if (strncmp(azColName[i], "type", 4) == 0) {
    //        params->s->mesh[idx].type = atoi(argv[i]);
    //    } else if (strncmp(azColName[i], "coords", 6) == 0) {
    //        float vecs[16];
    //        anvil_scanf(argv[i], "{%f %f %f %f} {%f %f %f %f} {%f %f %f %f} {%f %f %f %f}",
    //            &vecs[0], &vecs[1], &vecs[2], &vecs[3],&vecs[4], &vecs[5], &vecs[6], &vecs[7],
    //            &vecs[8], &vecs[9], &vecs[10], &vecs[11],&vecs[12], &vecs[13], &vecs[14], &vecs[15]
    //        );
    //        memcpy(&params->s->mesh[idx].coords, vecs, 64);
    //    } else if (strncmp(azColName[i], "quat", 4) == 0) {
    //        float q[4];
    //        anvil_scanf(argv[i], "{%f %f %f %f}", &q[0], &q[1], &q[2], &q[3]);
    //        memcpy(&params->s->mesh[idx].q, q, 16);
    //        memcpy(&params->s->mesh[idx].rigid.q, q, 16);
    //    } else if (strncmp(azColName[i], "scale", 5) == 0) {
    //        params->s->mesh[idx].scale = strtof(argv[i], NULL);
    //    } else if (strncmp(azColName[i], "outer_radius", 12) == 0) {
    //        float outer_rad = params->s->mesh[idx].scale * params->s->mesh[idx].scale;
    //        params->s->mesh[idx].outer_radius = sqrtf(outer_rad + outer_rad);
    //    } else if (strncmp(azColName[i], "state", 5) == 0) {
    //        params->s->mesh[idx].rigid.state = atoi(argv[i]);
    //    } else if (strncmp(azColName[i], "velocity", 8) == 0) {
    //        float v[4];
    //        anvil_scanf(argv[i], "{%f %f %f %f}", &v[0], &v[1], &v[2], &v[3]);
    //        memcpy(&params->s->mesh[idx].rigid.velocity, v, 16);
    //    } else if (strncmp(azColName[i], "rotate", 6) == 0) {
    //        params->s->mesh[idx].rigid.rotate = strtof(argv[i], NULL);
    //    } else if (strncmp(azColName[i], "cname", 5) == 0) {
    //         if (params->s->mesh[idx].type == MESH_TYPE_TERRAIN) {
    //             createTerrain(&params->s->mesh[idx], argv[i]);
    //         } else {
    //             createMesh(&params->s->mesh[idx], argv[i]);
    //         }
    //         if (params->s->mesh[idx].rigid.state == ENABLED)
    //             loadmeshRigid(&params->s->mesh[idx], argv[i]);
    //    }
    //}

    // *(int*)NotUsed += 1;

    return 0;
}
const int dbcountTableRows(const char dbpath[], const char sql_cmd[]) {
    sqlite3* db;
    sqlite3_stmt* stmt = { 0 };
    int rc, rows = 0;

    rc = sqlite3_open_v2(dbpath, &db, SQLITE_OPEN_READONLY, NULL);
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
void dbloadTable(const char dbpath[], scene *s, const int table_type, const char sql_cmd[]) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open(dbpath, &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (table_type == TABLE_TERRAIN)
        callback = &terrainCallback;
    if (table_type == TABLE_MODEL)
        callback = &modelCallback;
    if (table_type == TABLE_MESH)
        callback = &meshCallback;

    DBparams dbparams = {
        .s = s,
        .index = 0    // Passing an increment value to the calback, to initialize the a SCENE mesh every invocation.
    };
    rc = sqlite3_exec(db, sql_cmd, callback, &dbparams, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    return;
}
/* Execute sql_cmd to the path Database. */
void dbExecuteCommand(const char dbpath[], const char sql_cmd[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(dbpath, &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    int param = 0; // Passing an increment value to the calback, to initialize the a SCENE mesh every invocation.
    rc = sqlite3_exec(db, sql_cmd, callback, &param, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    return;
}
 /*   FOR TERRAIN INITIALIZATION #################################################################################    */
static int terrainInfoCallback(void *NotUsed, int argc, char** argv, char** azColName) {

    int idx = *(int*)NotUsed;
    int name_index = 0;
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "cname") == 0) {
            name_index = i;
        } else if (strncmp(azColName[i], "cname_length", 12) == 0) {
            temp[idx].cname_length = atoi(argv[i]);
        } else if (strncmp(azColName[i], "width", 5) == 0) {
            temp[idx].width = atoi(argv[i]);
        } else if (strncmp(azColName[i], "height", 6) == 0) {
            temp[idx].height = atoi(argv[i]);
        }
    }
    /* Compose cname with a NULL terminating string. */
    temp[idx].cname = malloc(temp[idx].cname_length + 1); // +1 For the NULL terminating character also.
    if (!temp[idx].cname) {
        debug_log_error(stdout, "temp[idx].cname = malloc(temp[idx].cname_length + 1)");
        return 1;
    }
    memcpy(temp[idx].cname, argv[name_index], temp[idx].cname_length + 1);

    *(int*)NotUsed += 1;

    return 0;
}
/* Execute sql_cmd to the path Database. */
void dbloadTerrainInfo(const char dbpath[], TerrainInitInfo *tif) {
    temp = tif;
    sqlite3* db;
    char *sql_cmd = "SELECT * FROM terrain;";
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(dbpath, &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    int param = 0; // Passing an increment value to the calback, to initialize the SCENE mesh every invocation.
    rc = sqlite3_exec(db, sql_cmd, terrainInfoCallback, &param, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    tif = temp;
    return;
}


