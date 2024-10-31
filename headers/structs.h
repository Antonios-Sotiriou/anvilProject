#ifndef STRUCTS_H
#define STRUCTS_H 1

#ifndef _INCLUDED_MM2
    #include <xmmintrin.h>
#endif // !_INCLUDED_MM2

// Local created headers.
#ifndef FLAGS_H
    #include "flags.h"
#endif // !FLAGS_H

#ifdef VECTORIZED_CODE // ######################################################
/* Vector's internal format is X Y Z W. */
typedef union __m128 vec4;
/* vectors internal format is X Y: */
typedef union __m64 vec2;
/* Quaternion's internal format is W X Y Z. */
typedef union __m128 quat;

typedef union {
    vec4 m[4];
} mat4x4;
#else // ITERATIVE_CODE ########################################################
/* Vector's internal format is X Y Z W. */
typedef union {
    float f32[4];
} vec4;
/* vectors internal format is X Y: */
typedef union {
    float f32[2];
} vec2;
/* Quaternion's internal format is W X Y Z. */
typedef vec4 quat;

typedef union {
    vec4 m[4];
} mat4x4;
#endif // VECTORIZED_CODE ######################################################

/* Cordinate system structure with components represented as P: position, U: up, V: right(vertical), N: front. */
typedef struct {
    vec4 v[4];
} coords;
/* Base struct to represent a face. */
typedef struct {
    vec4 v[3];
    vec2 vt[3];
    vec4 vn[3];
} face;
typedef struct {
    int state;                           // State of the rigid of the mesh. Can be either ENABLE: 1 or DISABLE: 0.
    vec4 velocity;                       // Velocity of a mesh.
    float rot_angle;                     // The rotation angle of the rigid body.
    quat q;                              // Rotation quaternion W, X, Y, Z.
} rigid;
/* Base structure to represent a shape. */
typedef struct {
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    float *vao,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        scale;                           // Value to store the scale of the mesh.
    int vao_indexes,                     // Number of vao indexes as individual floats.
        faces_indexes,                   // Number of faces in vao. ( vao_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vao. ( vao_indexes / 8 or faces_indexes * 3).
        vao_size;                        // The size of the vao in bytes.( vao_indexes * 4 ).
    rigid rigid;
} mesh;
/* Model structure to represent a collection of shapes. */
typedef struct {
    mesh *mesh;
    int mesh_indexes;
} model;
/* Model structure to represent a scene which consists of one or more models. */
typedef struct {
    //model *model;
    //int model_indexes;
    mesh *mesh;
    int mesh_indexes;
} scene;

#endif // !STRUCTS_H


