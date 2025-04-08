#ifndef RELATIONS_H
#define RELATIONS_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef _STDINT
    #include <stdint.h>
#endif // !_STDINT

#ifndef _INC_STRING
    #include <string.h>
#endif // !_INC_STRING

// Local created headers.
#ifndef FLAGS_H
    #include "flags.h"
#endif // !FLAGS_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // STRUCTS_H

int addChild(mesh *t, mesh *m);
int removeChild(mesh *t, mesh *m);

#endif // RELATIONS_H


