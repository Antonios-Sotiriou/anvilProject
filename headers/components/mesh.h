#ifndef MESH_H
#define MESH_H 1

#include "headers/structs.h"
#include "headers/components/internal_libraries/obj.h"

void createMesh(mesh *m, ENTRY obj);
void createMeshVAO(mesh *m);
void releaseMeshVAO(mesh *m);
void releaseMesh(mesh *m);

#endif // !MESH_H


