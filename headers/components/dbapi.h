#ifndef DBAPI_H
#define DBAPI_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

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
    #include "libraries/sqlite/sqlite3.h"
#endif // !SQLITE3_H

const int dbcountTableRows(const char path[], const char sql_cmd[]);
void dbloadTable(const char path[], const int type, const char sql_cmd[]);

#endif // !DBAPI_H

