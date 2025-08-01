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

/* Cordinate system structure with components represented as P: position, U: up, V: right(vertical), N: front. */
typedef struct {
    vec4 v[4];
} coords;
/* Base struct to represent a face. */
typedef struct {
    vec4 v[3];          // 3 vectors with 4 dimensions each
    //vec2 vt[3];         // 3 texels with 2 dimensions each
    //vec4 vn[3];         // 3 vectors-normals with 4 dimensions each
} face;

/* Main struct to hold animations extraxted from blender. */
typedef struct {
    vec4 *lc;                            // Locations array. Every index represents a frame at a given location.
    quat *rq;                            // Rotation quaternions array. Every index represents a frame with the given rotation.
    vec4 *sc;                            // Scales array. Every index represents a frame with a given scale.
    mat4x4 *bm;                          // Bone matrices array.Every index represents a bone matrix frame.
    mat4x4 anim_matrix;
    int frames;                          // Number of frames which the data are represent. That is also the array length of all the struct members.
} animation;

typedef struct {
    vec4 min,                            // Minimum values for X, Y, Z, W. The minimum limits of the mesh.
        max;                             // Maximum values for X, Y, Z, W. The maximum limits of the mesh.
    quat q;                              // Rotation quaternion W, X, Y, Z.
    float *vbo,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        falling_time,                    // Calculates the time, since the object starts falling, until it hits the ground or another object.
        collision_t,                     // The time of collision to help us sort them.Used in sortCollisions function, to find out which collision take place earlier.
        lowest_Y;                        // The distance between the object pivot point and its Lowest Y value.That helps us for terrain collisions until we implement swept Triangle Rigid Terrain collisions.
    int vbo_indexes,                     // Number of vbo indexes as individual floats.
        faces_indexes,                   // Number of faces in vbo. ( vbo_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vbo. ( vbo_indexes / 8 or faces_indexes * 3).
        vbo_size,                        // The size of the vbo in bytes.( vbo_indexes * 4 ).
        grounded;                        // Switch which tracks if object is grounded on the terrain or not. Can be 1 for grounded or 0 for floating objects.
    GLuint VAO,                          // VAO id or name represented by an unsigned integer.
        VBO;                             // VBO id or name represented by an unsigned integer.
    face *f;                             // The faces from them our rigid body consists.
} rigid;
/* Base structure to represent a shape. */
typedef struct mesh {
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    vec4 scale;                          // Vector to store the scale of the model.
    mat4x4 model_matrix;
    char *cname;                         // The name to identify a mesh. Thats a dynamically size adoptaable null terminating string.
    float *vbo,                          // The vertex array object with format { vXvYvZtUtVnXnYnZ }. v: vector, t: texels, n: normal.
        outer_radius;                    // Value to hold the radius of the circle which surounding the mesh. aka( sqrtf(scale * scale) + (scale * scale)). Pythagorean Theorem.
    int cname_length,                    // Length of the cname char array. SOS !! (not included the NULL terminated char).
        vbo_indexes,                     // Number of vbo indexes as individual floats.
        faces_indexes,                   // Number of faces in vbo. ( vbo_indexes / 24 ).
        vecs_indexes,                    // Number of vectors in vbo. ( vbo_indexes / 8 or faces_indexes * 3).
        vbo_size,                        // The size of the vbo in bytes.( vbo_indexes * 4 ).
        pk,                              // Primary key of the mesh, representing its position in the database. That is also the mesh index in the SCENE meshes array.
        asset_type,                      // The type of the asset.Can be terrain, model, mesh, etc.Identifies a base structure. 
        mesh_type,                       // The type of the mesh.
        visible,                         // Wether the mesh should be drawn on screen. Can be visible 1 to be drawn, or visible 0 not to.
        owns_rigid,                      // Wether or not the model has a rigid body attached to it.
        owns_anim,                        // Wether or not the mesh has an animation.
        rotate,                          // The rotation angle of the rigid body.
        number_of_children;              // The number of the children that the mesh owns
    GLuint VAO,                          // VAO id or name represented by an unsigned integer.
        VBO;                             // VBO id or name represented by an unsigned integer.
    rigid rigid;                         // Rigid body struct, which holds all usefull variables, for Physics and Collision Detection.
    struct mesh **children;               // The children mesh array of the mesh
    struct mesh *parent;                 // The parent at which the mesh belongs.
    animation anim;
} mesh;
/* Model structure to represent a collection of shapes. Probably we dont need a Children relation in this struct, because all the meshes it has are its children. */
typedef struct {
    coords coords;                       // The coordinates and orientation axis of the mesh P, U, V, N.
    quat q;                              // Quaternion to save rotations.
    vec4 velocity,                       // Velocity of a model.
        scale;                           // Vector to store the scale of the model.
    mat4x4 model_matrix;
    char *cname;                         // The name to identify a model. Thats a dynamically size adoptaable null terminating string.
    float outer_radius;                  // Value to hold the radius of the circle which surounding the model. aka( sqrtf(scale * scale) + (scale * scale)). Pythagorean Theorem.
    mesh *mesh;                          // Meshes array from which the model is consisting.
    int mesh_indexes,                    // Number of mesh indexes that the mesh pointer holds.
        cname_length,                    // Length of the cname char array. SOS !! (not included the NULL terminated char).
        pk,                              // Primary key of the model, representing its position in the database. That is also the model index in the SCENE meshes array.
        asset_type,                      // The type of the asset.Can be terrain, model, mesh, etc.Identifies a base structure. 
        model_type,                      // The type of the model.Can be Player, camera, light source etc.Identifies types of the same Primitive.
        visible,                         // Wether the mesh should be drawn on screen. Can be visible 1 to be drawn, or visible 0 not to.
        owns_rigid,                      // Wether or not the model has a rigid body attached to it.
        owns_anim,                       // Wether or not the model has an animation.
        rotate,                          // The rotation angle of the rigid body.
        quad_init,                       // Flag, which shows if the model went through the terrain initialization pipeline, at least one time, at the start of the program.
        quad_index,                      // The index of the terrain quad that the model is standing on.
        quad_face;                       // Flag to track on which triangle of the terrain quad we are in.Can be UPPER: 0, or LOWER: 1.
    rigid rigid;                         // Rigid body struct, which holds all usefull variables, for Physics and Collision Detection.
    animation anim;
} model;
/* Struct which is usefull only to initialize terrains from the first time and create the obj files for them. */
typedef struct {
    char *cname;                         // The name to identify a terrain. Thats a dynamically size adoptaable null terminating string.
    int cname_length,                    // Length of the cname char array. SOS !! (not included the NULL terminated char).
        width,                           // The width of the height map in pixels aka (vectors);
        height;                          // The height of the height map in pixels aka (vectors);
} TerrainInitInfo;
/* Struct to retrieve data of terrain. */
typedef struct {
    vec4 pos, normal;
    int quad_index, quad_face;
} TerrainPointInfo;
/* Struct to hold infos about a terrain quad. */
typedef struct {
    int* mpks,             // Integer array to save the Primary keys of the meshes, which are memmbers of this quad.
        mpks_indexes;      // m_pks: Primary keys aka(Scene index) of the meshes, m_indexes: number of m_pks in the m_pks array.
} Quad;
/* Struct to hold usefull Terrain information to be available throught the program after we release the height map. */
typedef struct {
    Quad *quad;            // Quads pointer to save info about each quad of the terrain.
    int vec_width,         // Number of vectors at width direction. Number is diferent from quads number, because some vectors are shared between quads.
        vec_height,        // Number of vectors at height direction. Number is diferent from quads number, because some vectors are shared between quads.
        quad_indexes,      // Emvadon of the Terrain quads.
        quad_rows,         // Number os quads in rows direction. They are always vectors Width - 1.
        quad_cols;         // Number os quads in columns direction. They are always vectors Height - 1.
} TerrainInfo;
/* Model structure to represent a scene which consists of one or more models. */
typedef struct {
    model *model;
    int model_indexes;
    TerrainInfo t;
} scene;

#endif // !STRUCTS_H


