#ifndef RASTERIZATION_H
#define RASTERIZATION_H 1

#include "headers/structs.h"

const void rasterize(scene *s);
const int rigidFrustumCulling(rigid *r, mat4x4 *PROJECTION_M);

#endif // !RASTERIZATION_H


