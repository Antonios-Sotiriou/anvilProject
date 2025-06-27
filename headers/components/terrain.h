#ifndef TERRAIN_H
#define TERRAIN_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef BMP_H
    #include "headers/components/bmp.h"
#endif // !BMP_H

#ifndef OPENGL_H
    #include "headers/components/openGL.h"
#endif // !OPENGL_H

typedef struct {
    vec4 pos, normal;
    int quad_index, quad_face;
} TerrainPointInfo;

void createTerrain(model *m);
void initModelQuadInfo(model *m);
void addModelToQuad(model *m);
void removeModelFromQuad(model *m);
void getTerrainPointInfo(vec4 coords, int *qi, int *uol);
void getModelPositionData(model *m, vec4 *tp, vec4 *tn);
const TerrainPointInfo getvec4PositionData(const vec4 v);
void logTerrainInfo(void);
void logTerrainQuad(const int quad_index);

#endif // !TERRAIN_H

