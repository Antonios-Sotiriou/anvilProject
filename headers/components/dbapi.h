#ifndef DBAPI_H
#define DBAPI_H 1

#include "headers/structs.h"

/* Struct to pass usefull data to the database callback functions. */
typedef struct DBparams {
    scene *s;
    int index;
} DBparams;

/* Struct to pass usefull data to the database callback functions for initializing terrain info structures. */
typedef struct DBTinit {
    TerrainInitInfo *tinfo;
    int index;
} DBTinit;

const int dbcountTableRows(const char dbpath[], const char sql_cmd[]);
void dbloadTable(const char dbpath[], scene *s, const int table_type, const char sql_cmd[]);
void dbExecuteCommand(const char dbpath[], const char sql_cmd[]);
void dbloadTerrainInfo(const char dbpath[], TerrainInitInfo *tif);

#endif // !DBAPI_H


