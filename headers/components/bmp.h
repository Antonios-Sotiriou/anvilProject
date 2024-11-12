#ifndef BMP_H
#define BMP_H 1

#ifndef _INC_STDIO
    #include <stdio.h>
#endif // !_INC_STDIO

#ifndef _INC_STRING
    #include <string.h>
#endif // !_INC_STRING

#ifndef _INC_STDLIB
    #include <stdlib.h>
#endif //_INC_STDLIB

#ifndef FLAGS_H
    #include "headers/flags.h"
#endif // !FLAGS_H

#ifndef anvil_VERSION_MAJOR
    #include "headers/cmake_variables.h"
#endif // !anvil_VERSION_MAJOR

/* BMP Image file reading structs. */
typedef struct {
    unsigned short Type;
    unsigned int Size;
    unsigned short Res1;
    unsigned short Res2;
    unsigned int OffSet;
} BMP_Header;
typedef struct {
    int RedX;
    int RedY;
    int RedZ;
} Red;
typedef struct {
    int BlueX;
    int BlueY;
    int BlueZ;
} Blue;
typedef struct {
    int GreenX;
    int GreenY;
    int GreenZ;
} Green;
typedef struct {
    Red red_coords;
    Green green_coords;
    Blue blue_coords;
} CS_EndPoints;
typedef struct {
    unsigned int Size;
    signed int Width;
    signed int Height;
    unsigned short Planes;
    unsigned short BitsPerPixel;
    unsigned int compression;
    unsigned int SizeOfBitmap;
    long  HorzResolution;
    long VertResolution;
    unsigned int ColorsUsed;
    unsigned int ColorsImportant;
    unsigned int RedMask;
    unsigned int GreenMask;
    unsigned int BlueMask;
    unsigned int AlphaMask;
    unsigned int CSType;
    CS_EndPoints CSEndPoints;
    unsigned int GammaRed;
    unsigned int GammaGreen;
    unsigned int GammaBlue;
    unsigned int Intent;
    unsigned int ProfileData;
    unsigned int ProfileSize;
    unsigned int Reserved;
} BMP_Info;
typedef struct {
    BMP_Header header;
    BMP_Info info;
    char *data;
} BMP;

void readBMP(BMP *f, const char path[]);
void releaseBMP(BMP* f);
void logBMP_Header(const BMP_Header bmp_h);
void logBMP_EndPoints(const CS_EndPoints cse);
void logBMP_Info(const BMP_Info bmp_i);
void logBMP(const BMP bmp);

#endif // !BMP_H


