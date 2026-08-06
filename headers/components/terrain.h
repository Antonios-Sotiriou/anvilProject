#ifndef TERRAIN_H
#define TERRAIN_H 1

#include "headers/structs.h"

int initTerrainsHeightMaps(void);
void initModelQuadInfo(scene *s, model *m);
void addModelToQuad(scene *s, model *m);
void removeModelFromQuad(scene *s, model *m);
void getTerrainPointInfo(scene *s, vec4 coords, int *qi, int *uol);
void getModelPositionData(scene *s, model *m, vec4 *tp, vec4 *tn);
const TerrainPointInfo getvec4PositionData(scene *s, const vec4 v);
void updateSurroundingQuads(scene *s, model *m);
void retrieveNearbyColliders(scene* s, model* m);
void logTerrainQuad(scene *s, const int quad_index);
void logTerrainInfo(scene* s);

#endif // !TERRAIN_H

