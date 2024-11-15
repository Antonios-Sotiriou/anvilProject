#ifndef GENERAL_H
#define GENERAL_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

void swap(void* a, void* b, unsigned long size);
const float radians(const float value);
void getRigidLimits(mesh *m, const int len);

#endif // !GENERAL_H


