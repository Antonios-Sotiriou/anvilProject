#ifndef SCENE_H
#define SCENE_H 1

#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef MESH_H
    #include "headers/components/mesh.h"
#endif // !MESH_H

#ifndef DBAPI_H
    #include "headers/components/dbapi.h"
#endif // !DBAPI_H

#ifndef OPENGL_H
    #include "headers/components/openGL.h"
#endif // !OPENGL_H

#ifndef TERRAIN_H
    #include "headers/components/terrain.h"
#endif // !TERRAIN_H

#ifndef QUATERNIONS_H
    #include "headers/components/quaternions.h"
#endif // !QUATERNIONS_H

#ifndef RIGID_H
    #include "headers/components/rigid.h"
#endif // !RIGID_H


void createScene(void);
void releaseScene(void);

#endif // !SCENE_H


