#ifndef DBAPI_H
#define DBAPI_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

//#ifndef GLOBAL_H
//    #include "headers/global.h"
//#endif // !GLOBAL_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef TERRAIN_H
    #include "headers/components/terrain.h"
#endif // !TERRAIN_H

#ifndef MESH_H
    #include "headers/components/mesh.h"
#endif // !MESH_H

#ifndef SQLITE3_H
    #include "libraries/sqlite-amalgamation/sqlite3.h"
#endif // !SQLITE3_H

/* Struct to pass usefull data to the database callback functions. */
typedef struct DBparams {
    scene *s;
    int index;
} DBparams;

const int dbcountTableRows(const char dbpath[], const char sql_cmd[]);
void dbloadTable(const char dbpath[], scene *s, const int table_type, const char sql_cmd[]);
void dbExecuteCommand(const char dbpath[], const char sql_cmd[]);
void dbloadTerrainInfo(const char dbpath[], TerrainInitInfo *tif);

#endif // !DBAPI_H


