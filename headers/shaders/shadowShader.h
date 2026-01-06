#ifndef _SHADOWSHADER_H
#define _SHADOWSHADER_H 1

#if !defined(STDIO_H) || !defined(_STDIO_H)
    #include <stdio.h>
#endif // !STDIO_H _STDIO_H

#ifndef GLOBAL_H
    #include "headers/global.h"
#endif // !GLOBAL_H

#ifndef _ANVIL_STRUCTS_H
    #include "headers/structs.h"
#endif

#ifndef MATRICES_H
    #include "headers/components/internal_libraries/matrices.h"
#endif // !MATRICES_H

const int initShadowShader(void);
void shadowShader(scene *s);

#endif /* _SHADOWSHADER_H */