#ifndef ANIMATIONS_H
#define ANIMATIONS_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void readBlenderAnimations(animationData *ad, char path[]);
void releaseBlenderAnimations(animationData* ad);
void loadModelAnimations(model* m);

#endif // !ANIMATIONS_H


