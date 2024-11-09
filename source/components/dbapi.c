#include "headers/components/dbapi.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    //for (int i = 0; i < argc; i++) {
    //    if (strncmp(azColName[i], "pk", 2) == 0)
    //        m.pk = atoi(argv[i]);
    //    else if (strncmp(azColName[i], "path", 4) == 0)
    //        m.path = argv[i];
    //    else if (strncmp(azColName[i], "type", 4) == 0)
    //        m.type = atoi(argv[i]);
    //    else if (strncmp(azColName[i], "coords", 6) == 0) {
    //        float vecs[16];
    //        sscanf_s(argv[i], "{%f %f %f %f} {%f %f %f %f} {%f %f %f %f} {%f %f %f %f}",
    //            &vecs[0], &vecs[1], &vecs[2], &vecs[3],
    //            &vecs[4], &vecs[5], &vecs[6], &vecs[7],
    //            &vecs[8], &vecs[9], &vecs[10], &vecs[11],
    //            &vecs[12], &vecs[13], &vecs[14], &vecs[15]
    //        );
    //        memcpy(m.coords, vecs, 64);
    //    }
    //}

    //printf("pk: %d\n", m.pk);
    //printf("path: %s\n", m.path);
    //printf("type: %d\n", m.type);
    //logvec4(m.coords[0]);
    //logvec4(m.coords[1]);
    //logvec4(m.coords[2]);
    //logvec4(m.coords[3]);

    return 0;
}
void dbloadTable(const char path[], const int type, const char sql_cmd[]) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open(path, &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    //if (type == MESH_TABLE)
    //    callback = &meshCallback;

    rc = sqlite3_exec(db, sql_cmd, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    return 0;
}