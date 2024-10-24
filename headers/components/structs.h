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

typedef union {
    vec4 m[4];
} mat4x4;
#endif // VECTORIZED_CODE ######################################################

/* Base struct to represent a face. */
typedef struct {
    vec4 v[3];
} face;
/* Base structure to represent a shape. */
typedef struct {
    vec4 *v;
    face *f;
    int v_indexes, f_indexes;
} mesh;
/* Model structure to represent a collection of shapes. */
typedef struct {
    mesh *ms;
    int m_indexes;
} model;
/* Model structure to represent a scene which consists of one or more models. */
typedef struct {
    model *md;
    int m_indexes;
} scene;

#endif // STRUCTS_H


