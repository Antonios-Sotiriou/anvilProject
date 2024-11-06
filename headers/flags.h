#ifndef FLAGS_H
#define FLAGS_H 1

#ifndef anvil_VERSION_MAJOR
    #include "headers/cmake_variables.h"
#endif // !anvil_VERSION_MAJOR

#define VECTORIZED_CODE

#ifdef anvil_VERSION_MAJOR
    /* The meshes which consist the scene enumeration. */ 
    enum { terrain, camera, initAssets };

    /* Flags for states changing. */
    #define ENABLE              1
    #define DISABLE             0
    #define ON                  1
    #define OFF                 0

    /* Meshes paths. */
    // Primitive Meshes.
	#define CUBE_FLAT           anvil_SOURCE_DIR "/meshes/cube/cube_smooth.obj"
	#define CUBE_SMOOTH         anvil_SOURCE_DIR "/meshes/cube/cube_smooth.obj"

    // Terrains.
    #define GITANA10x10         anvil_SOURCE_DIR "/terrains/gitana/gitana10x10.bmp"
    #define GITANA100x100       anvil_SOURCE_DIR "/terrains/gitana/gitana100x100.bmp"
    #define GITANA128x128       anvil_SOURCE_DIR "/terrains/gitana/gitana128x128.bmp"
    #define GITANA512x512       anvil_SOURCE_DIR "/terrains/gitana/gitana512x512.bmp"

#endif // !anvil_MAJOR_VERSION

#endif // !FLAGS_H


