#ifndef RIGID_H
#define RIGID_H 1

#ifndef _INC_STRING
    #include <string.h>
#endif // !_INC_STRING

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef OBJ_H
    #include "headers/components/obj.h"
#endif // !OBJ_H

void loadRigid(mesh* m, const char path[]);
void releaseRigid(mesh* m);

#endif // !RIGID_H


