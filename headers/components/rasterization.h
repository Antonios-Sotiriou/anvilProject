#ifndef RASTERIZATION_H
#define RASTERIZATION_H 1

/* Shaders. */
#ifndef MAINSHADER_H
    #include "headers/shaders/mainShader.h"
#endif // !MAINSHADER_H

#ifndef DISPLAYSHADER_H
    #include "headers/shaders/displayShader.h"
#endif // !DISPLAYSHADER_H

#ifndef TESTSHADER_H
    #include "headers/shaders/testShader.h"
#endif // !TESTSHADER_H

#ifndef RIGIDSHADER_H
    #include "headers/shaders/rigidShader.h"
#endif // !RIGIDSHADER_H

const void rasterize();

#endif // !RASTERIZATION_H


