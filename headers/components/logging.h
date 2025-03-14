#ifndef LOGGING_H
#define LOGGING_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

// Local created headers.
#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef VEC_MATH_H
    #include "headers/components/vec_math.h"
#endif // !VEC_MATH_H

void logvec4(const vec4 v);
void logmat4x4(const mat4x4 m);
void logcoords(const coords m);
void logrigid(const rigid r);
void logmesh(const mesh m);
void logmodel(const model m);
void logscene(const scene s);

#endif // !LOGGING_H


