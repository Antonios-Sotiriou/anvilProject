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
    #include "headers/components/internal_libraries/vec_math.h"
#endif // !VEC_MATH_H

#ifndef ANIMATIONS_H
    #include "headers/components/animations.h"
#endif // !ANIMATIONS_H

void logvec4(const vec4 v);
void logmat4x4(const mat4x4 m);
void loganim(const animation an);
void logcoords(const coords c);
void logrigid(const rigid r);
void logmesh(const mesh m);
void logmodel(const model m);
void logscene(const scene s);

#endif // !LOGGING_H


