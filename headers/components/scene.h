#ifndef SCENE_H
#define SCENE_H 1

#include "headers/structs.h"

//#ifndef FLAGS_H
//    #include "headers/flags.h"
//#endif // !FLAGS_H
//
//#ifndef STRUCTS_H
//    
//#endif // !STRUCTS_H
//
//#ifndef MESH_H
//    #include "headers/components/mesh.h"
//#endif // !MESH_H
//
//#ifndef MODEL_H
//    #include "headers/components/model.h"
//#endif // !MODEL_H
//
//#ifndef DBAPI_H
//    #include "headers/components/dbapi.h"
//#endif // !DBAPI_H
//
//#ifndef ANIMATIONS_H
//    #include "headers/components/animations.h"
//#endif // !ANIMATIONS_H

void doNothink(scene *s);
void initScene(scene* s, int winWidth, int winHeight);
void createScene(scene *s);
void releaseScene(scene *s);
void createSceneCanvas(canvas *c);
void releaseSceneCanvas(canvas *c);
void drawOnSceneCanvas(canvas *c, const int textureID);

#endif // !SCENE_H


