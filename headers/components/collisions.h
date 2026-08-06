#ifndef COLLISIONS_H
#define COLLISIONS_H 1

#include "headers/structs.h"

void modelTerrainCollision(scene *s, model *m);
const int staticOuterRadiusCollision(scene *s, model *m);
const int sweptAABBCollision(scene *s, model *m);
const int rotationCollision(scene* s, model* m);
void sortCollisions(scene* s, model* m);
const int staticOBBCollision(model *m, const int pk);

#endif // !COLLISIONS_H


