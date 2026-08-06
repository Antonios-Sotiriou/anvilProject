#ifndef RIGID_H
#define RIGID_H 1

#include "headers/structs.h"

void loadModelRigid(model *m);
void createRigidVAO(rigid *r);
void releaseRigidVAO(rigid *r);
void getRigidLimits(rigid *r);
void releaseRigid(rigid *r);

#endif // !RIGID_H


