#ifndef LOGGING_H
#define LOGGING_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

// Local created headers.
#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void logvec4(const vec4 v);
void logmat4x4(const mat4x4 m);
void logcoords(const coords m);
void logmesh(const mesh m);

#endif // !LOGGING_H


