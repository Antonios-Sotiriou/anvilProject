#ifndef SCENE_H
#define SCENE_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef MESH_H
    #include "headers/components/mesh.h"
#endif // !MESH_H

void createScene(void);
void releaseScene(void);

#endif // !SCENE_H


