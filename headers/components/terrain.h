#ifndef TERRAIN_H
#define TERRAIN_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef BMP_H
    #include "headers/components/internal_libraries/bmp.h"
#endif // !BMP_H

#ifndef OBJ_H
    #include "headers/components/internal_libraries/obj.h"
#endif // !OBJ_H

#ifndef MESH_H
    #include "headers/components/mesh.h"
#endif // !MESH_H

#ifndef OPENGL_H
    #include "headers/components/openGL.h"
#endif // !OPENGL_H

#ifndef DBAPI_H
    #include "headers/components/dbapi.h"
#endif // !DBAPI_H

int initTerrainsHeightMaps(void);
void initModelQuadInfo(scene *s, model *m);
void addModelToQuad(scene *s, model *m);
void removeModelFromQuad(scene *s, model *m);
void getTerrainPointInfo(scene *s, vec4 coords, int *qi, int *uol);
void getModelPositionData(scene *s, model *m, vec4 *tp, vec4 *tn);
const TerrainPointInfo getvec4PositionData(scene *s, const vec4 v);
void updateSurroundingQuads(scene *s, model *m);
int* retrieveNearbyColliders(scene* s, model* m);
void logTerrainQuad(scene *s, const int quad_index);
void logTerrainInfo(scene* s);

#endif // !TERRAIN_H

