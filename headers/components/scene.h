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

#ifndef MODEL_H
    #include "headers/components/model.h"
#endif // !MODEL_H

#ifndef DBAPI_H
    #include "headers/components/dbapi.h"
#endif // !DBAPI_H

#ifndef ANIMATIONS_H
    #include "headers/components/animations.h"
#endif // !ANIMATIONS_H

void createScene(void);
void releaseScene(void);

#endif // !SCENE_H


