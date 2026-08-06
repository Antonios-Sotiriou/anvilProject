#ifndef LOGGING_H
#define LOGGING_H 1

#include "headers/structs.h"
#include "headers/components/animations.h"

void logvec4(const vec4 v);
void logmat4x4(const mat4x4 m);
void loganim(const animation an);
void logcoords(const coords c);
void logrigid(const rigid r);
void logmesh(const mesh m);
void logmodel(const model m);
void logscene(const scene s);

#endif // !LOGGING_H


