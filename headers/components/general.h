#ifndef GENERAL_H
#define GENERAL_H 1

#if !defined(NC_STRING) || !defined(_STRING_H)
    #include <string.h>
#endif // !INC_STRING _STRING_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void swap(void* a, void* b, unsigned long size);
const float radians(const float value);
void getvec4ArrayLimits(vec4 v[], const int array_len, vec4 *min, vec4 *max);

#endif // !GENERAL_H


