#ifndef TERRAIN_H
#define TERRAIN_H 1

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef BMP_H
    #include "headers/components/bmp.h"
#endif // !BMP_H

#ifndef OPENGL_H
    #include "headers/components/openGL.h"
#endif // !OPENGL_H

void createTerrain(mesh *m, const char type[]);

#endif // !TERRAIN_H

