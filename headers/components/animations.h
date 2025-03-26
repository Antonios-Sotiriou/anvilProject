#ifndef ANIMATIONS_H
#define ANIMATIONS_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void readBlenderAnimations(animationData *ad, char path[]);
void releaseBlenderAnimations(animationData* ad);
void loadModelAnimations(model* m);

#endif // !ANIMATIONS_H


