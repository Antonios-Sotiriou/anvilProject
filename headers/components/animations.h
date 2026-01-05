#ifndef ANIMATIONS_H
#define ANIMATIONS_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

/* Global variables. */
#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef ANIMTEXT_H
    #include "headers/components/animText.h"
#endif // !ANIMTEXT_H

// Local created headers
#ifndef VEC_MATH_H
    #include "headers/components/internal_libraries/vec_math.h"
#endif // !VEC_MATH_H

#ifndef QUATERNIONS_H
    #include "headers/components/internal_libraries/quaternions.h"
#endif // !QUATERNIONS_H

#ifndef MATRICES_H
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

void loadModelAnimations(model *m);
void animateModels(scene *s);
void releaseAnimations(animation *m);

#endif // !ANIMATIONS_H


