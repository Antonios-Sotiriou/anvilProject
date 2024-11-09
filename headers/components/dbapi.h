#ifndef DBAPI_H
#define DBAPI_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef SQLITE3_H
    #include "libraries/sqlite/sqlite3.h"
#endif // !SQLITE3_H

void dbloadTable(const char path[], const int type, const char sql_cmd[]);

#endif // !DBAPI_H


