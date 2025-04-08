#ifndef STRUCTS_H
#define STRUCTS_H 1

#if !defined(_INCLUDED_SMM) || !defined(_SMMINTRIN_H_INCLUDED)
    #include <smmintrin.h>
#endif // !_INCLUDED_SMM _SMMINTRIN_H_INCLUDED

/* OpenGL headers. */
#if !defined(__glew_h__) || !defined(__GLEW_H__)
    #include "libraries/glew-2.1.0/include/GL/glew.h"
#endif

// Local created headers.
#ifndef FLAGS_H
    #include "flags.h"
#endif // !FLAGS_H

#ifdef VECTORIZED_CODE // ######################################################
    #if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        /* Vector's internal format is X Y Z W. */
        typedef union __m128 vec4;
        /* Quaternion's internal format is W X Y Z. */
        typedef union __m128 quat;
    #elif defined(LINUX) || defined(__linux__)
        /* Vector's internal format is X Y Z W. */
        typedef __m128 vec4;
        /* Quaternion's internal format is W X Y Z. */
        typedef __m128 quat;
    #endif
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

/* Struct to hold infos about a terrain quad. */
typedef struct {
    int *mpks,             // Integer array to save the Primary keys of the meshes, which are memmbers of this quad.
        mpks_indexes;       // m_pks: Primary keys aka(Scene index) of the meshes, m_indexes: number of m_pks in the m_pks array.
} Quad;
/* Struct to hold usefull Terrain information to be available throught the program after we release the height map. */
typedef struct {
    Quad *quad;            // Quads pointer to save info about each quad of the terrain.
    int vec_width,          // Number of vectors at width direction. Number is diferent from quads number, because some vectors are shared between quads.
        vec_height,         // Number of vectors at height direction. Number is diferent from quads number, because some vectors are shared between quads.
        quad_indexes,      // Emvadon of the Terrain quads.
        quad_rows,          // Number os quads in rows direction. They are always vectors Width - 1.
        quad_cols;          // Number os quads in columns direction. They are always vectors Height - 1.
} TerrainInfo;

/* Cordinate system structure with components represented as P: position, U: up, V: right(vertical), N: front. */
typedef struct {
    vec4 v[4];
} coords;
/* Base struct to represent a face. */
typedef struct {
    vec4 v[3];          // 3 vectors with 4 dimensions each
    vec2 vt[3];         // 3 texels with 2 dimensions each
    vec4 vn[3];         // 3 vectors-normals with 4 dimensions each
} face;

/* Main struct to hold animations extraxted from blender. */
typedef struct {
    int frames;                          // Number of frames which the data are represent. That is also the array length of all the struct members.
    vec4 *lc;                            // Locations array. Every index represents a frame at a given location.
    quat *rq;                            // Rotation quaternions array. Every index represents a frame with the given rotation.
    vec4 *sc;                            // Scales array. Every index represents a frame with a given scale.
} animation;

typedef struct {
    vec4 *v,                             // Vectors array to be used for primitive AABB collision. Vectors are unique to save iterations when aquairing min and max 3d values.
        *n,                              // Normals array to be used for OBB collision. Normals are unique to save iterations.
        min,                             // Minimum values for X, Y, Z, W. The minimum limits of the mesh.
        max;                             // Maximum values for X, Y, Z, W. The maximum limits of the mesh.
    quat q;                              // Rotation quaternion W, X, Y, Z.
    float *vbo,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        falling_time,                    // Calculates the time, since the object starts falling, until it hits the ground or another object.
        collision_t;                     // The time of collision to help us sort them.Used in sortCollisions function, to find out which collision take place earlier.
    int v_indexes,
        n_indexes,
        vbo_indexes,                     // Number of vbo indexes as individual floats.
        faces_indexes,                   // Number of faces in vbo. ( vbo_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vbo. ( vbo_indexes / 8 or faces_indexes * 3).
        vbo_size,                        // The size of the vbo in bytes.( vbo_indexes * 4 ).
        state,                           // State of the rigid of the mesh. Can be either ENABLE: 1 or DISABLE: 0.
        grounded;                        // Switch which tracks if object in grounded on the terrain or not. Can be 1 for grounded or 0 for floating objects.
    GLuint VAO,                          // VAO id or name represented by an unsigned integer.
        VBO;                             // VBO id or name represented by an unsigned integer.
} rigid;
/* Base structure to represent a shape. */
typedef struct mesh {
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    char *cname;                         // The name to identify a mesh. Thats a dynamically size adoptaable null terminating string.
    float *vbo,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        scale,                           // Value to store the scale of the mesh.
        outer_radius;                    // Value to hold the radius of the circle which surounding the mesh. aka( sqrtf(scale * scale) + (scale * scale)). Pythagorean Theorem.
    int length_cname,                    // Length of the cname char array. SOS !! (not included the NULL terminated char).
        vbo_indexes,                     // Number of vbo indexes as individual floats.
        faces_indexes,                   // Number of faces in vbo. ( vbo_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vbo. ( vbo_indexes / 8 or faces_indexes * 3).
        vbo_size,                        // The size of the vbo in bytes.( vbo_indexes * 4 ).
        pk,                              // Primary key of the mesh, representing its position in the database. That is also the mesh index in the SCENE meshes array.
        type,                            // The type of the mesh.
        visible;                         // Wether the mesh should be drawn on screen. Can be visible 1 to be drawn, or visible 0 not to.
    GLuint VAO,                          // VAO id or name represented by an unsigned integer.
        VBO;                             // VBO id or name represented by an unsigned integer.
    rigid rigid;                         // Rigid body struct, which holds all usefull variables, for Physics and Collision Detection.
    struct mesh *children;               // The children mesh array of the mesh
    struct mesh *parent;                 // The parent at which the mesh belongs.
    animation anim;
    int number_of_children;              // The number of the children that the mesh owns
} mesh;
/* Model structure to represent a collection of shapes. Probably we dont need a Children relation in this struct, because all the meshes it has are its children. */
typedef struct {
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    vec4 velocity;                       // Velocity of a model.
    char *cname;                         // The name to identify a model. Thats a dynamically size adoptaable null terminating string.
    float scale,                         // Value to store the scale of the model.
        outer_radius,                    // Value to hold the radius of the circle which surounding the model. aka( sqrtf(scale * scale) + (scale * scale)). Pythagorean Theorem.
        rotate;                          // The rotation angle of the rigid body.
    mesh *mesh;                          // Meshes array from which the model is consisting.
    int mesh_indexes,                    // Number of mesh indexes that the mesh pointer holds.
        length_cname,                    // Length of the cname char array. SOS !! (not included the NULL terminated char).
        pk,                              // Primary key of the model, representing its position in the database. That is also the model index in the SCENE meshes array.
        type,                            // The type of the model.
        visible,                         // Wether the mesh should be drawn on screen. Can be visible 1 to be drawn, or visible 0 not to.
        quad_init,                       // Flag, which shows if the model went through the terrain initialization pipeline, at least one time, at the start of the program.
        quad_index,                      // The index of the terrain quad that the model is standing on.
        quad_face;                       // Flag to track on which triangle of the terrain quad we are in.Can be UPPER: 0, or LOWER: 1.
    rigid rigid;                         // Rigid body struct, which holds all usefull variables, for Physics and Collision Detection.
} model;
/* Model structure to represent a scene which consists of one or more models. */
typedef struct {
    model *model;
    int model_indexes;
    TerrainInfo t;
} scene;

#endif // !STRUCTS_H


