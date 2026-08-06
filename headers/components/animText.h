#ifndef ANIMTEXT_H
#define ANIMTEXT_H 1

#include "headers/structs.h"

/* Base struct to organize per object animations, where object can be a mesh, a model or a rigid body. */
typedef struct {
    int number_of_frames,         // The number of frames that the data represent. We can increase or decrease this number through interpolation between frames.
        number_of_children;       // Self explanatory. The number of children which the object has.
    char *cname,                  // Name of the object. 
        **children;               // Pointer array of pointers, to create and hold for further processing, the children string names.
    vec4 *location;               // location per frame 
    quat *rotation_quaternion;    // rotation per frame
    vec4 *scale;                  // scale per frame
    mat4x4 *bone_matrix;          // bone matrix per frame
} Object;
/* Base struct to organize animations Data importedd from Blender. */
typedef struct {
    int number_of_frames,         // The number of frames that the data represent. We can increase or decrease this number through interpolation between frames.
        number_of_objects;        // the number of objects that the data represent. Can be meshes or bones.
    Object *object;
} animTextData;

void readAnimText(animTextData *an, char path[]);
void releaseAnimText(animTextData *an);

#endif // !ANIMTEXT_H


