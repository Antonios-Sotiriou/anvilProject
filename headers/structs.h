#ifndef STRUCTS_H
#define STRUCTS_H 1

#if !defined(_INCLUDED_SMM) || !defined(_SMMINTRIN_H_INCLUDED)
    #include <smmintrin.h>
#endif // !_INCLUDED_SMM _SMMINTRIN_H_INCLUDED

// Local created headers.
#ifndef FLAGS_H
    #include "flags.h"
#endif // !FLAGS_H

#ifdef VECTORIZED_CODE // ######################################################
/* Vector's internal format is X Y Z W. */
typedef __m128 vec4;
/* Quaternion's internal format is W X Y Z. */
typedef __m128 quat;
typedef union {
    vec4 m[4];
} mat4x4;
#else // ITERATIVE_CODE ########################################################
/* Vector's internal format is X Y Z W. */
typedef union {
    float m128_f32[4];
} vec4;
/* Quaternion's internal format is W X Y Z. */
typedef vec4 quat;

typedef union {
    vec4 m[4];
} mat4x4;
#endif // VECTORIZED_CODE ######################################################

/* vectors internal format is X Y. Can be used with intrinsics, but didn't found a usefull usage so far. */
typedef union {
    float m64_f32[2];
} vec2;
/* Vector's internal format is X Y Z. Can't be used for intrinsics SSE, because it's size is 12 bytes. Not a power multiplier of 2. */
typedef struct {
    float m96_f32[3];
} vec3;

/* Struct to save infos about a point on the terrain. */
typedef struct {
    vec4 pos, normal;
    int quad_index;
} TerrainPointInfo;
/* Struct to hold infos about a terrain quad. */
typedef struct {
    int *mpks,             // Integer array to save the Primary keys of the meshes, which are memmbers of this quad.
        mpks_indexes;       // m_pks: Primary keys aka(Scene index) of the meshes, m_indexes: number of m_pks in the m_pks array.
} Quad;
/* Struct to hold usefull Terrain information to be available throught the program after we release the height map. */
typedef struct {
    Quad *quad;            // Quads pointer to save info about each quad of the terrain.
    int vecWidth,          // Number of vectors at width direction. Number is diferent from quads number, because some vectors are shared between quads.
        vecHeight,         // Number of vectors at height direction. Number is diferent from quads number, because some vectors are shared between quads.
        quad_indexes,      // Emvadon of the Terrain quads.
        quadRows,          // Number os quads in rows direction. They are always vectors Width - 1.
        quadCols;          // Number os quads in columns direction. They are always vectors Height - 1.
} TerrainInfo;

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
    vec4 *v;                             // Vectors array to be used for primitive AABB collision. Vectors are unique to save iterations when aquairing min and max 3d values.
    face *f;                             // Faces array to be used for collisions. Thats the minimum low Poly represantation of the model, with texels and normals also included.
    int v_indexes,
        f_indexes,
        state;                           // State of the rigid of the mesh. Can be either ENABLE: 1 or DISABLE: 0.
    vec4 min,                            // Minimum values for X, Y, Z, W. The minimum limits of the mesh.
        max,                             // Maximum values for X, Y, Z, W. The maximum limits of the mesh.
        velocity;                        // Velocity of a mesh.
    float rot_angle;                     // The rotation angle of the rigid body.
    quat q;                              // Rotation quaternion W, X, Y, Z.
} rigid;
/* Base structure to represent a shape. */
typedef struct {
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    float *vbo,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        scale,                           // Value to store the scale of the mesh.
        outer_radius;                    // Value to hold the radius of the circle which surounding the mesh. aka( sqrtf(scale * scale) + (scale * scale)). Pythagorean Theorem.
    int vbo_indexes,                     // Number of vbo indexes as individual floats.
        faces_indexes,                   // Number of faces in vbo. ( vbo_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vbo. ( vbo_indexes / 8 or faces_indexes * 3).
        vbo_size,                        // The size of the vbo in bytes.( vbo_indexes * 4 ).
        VAO,
        VBO,
        pk,
        type,
        quadInit,
        quadIndex;
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
    TerrainInfo t;
} scene;

#endif // !STRUCTS_H


