#ifndef TEXTURE_H
#define TEXTURE_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#ifndef STRUCTS_H
    #include "headers/structs.h"
#endif // !STRUCTS_H

#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef BMP_H
    #include "headers/components/internal_libraries/bmp.h"
#endif // !BMP_H

void loadModelTextureAtlas(model *m);

#endif // !TEXTURE_H