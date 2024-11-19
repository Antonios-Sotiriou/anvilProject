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

void createTerrain(mesh *m, const char name[]);
void initMeshQuadInfo(mesh* m);
void addMeshToQuad(mesh* m);
void removeMeshFromQuad(mesh* m);
const int getTerrainQuadIndex(vec4 coords);
void logTerrainInfo(void);
void logTerrainQuad(const int quad_index);

#endif // !TERRAIN_H

