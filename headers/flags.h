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
	#define CUBE_FLAT           anvil_SOURCE_DIR "/meshes/cube/cube_smooth.obj"
	#define CUBE_SMOOTH         anvil_SOURCE_DIR "/meshes/cube/cube_smooth.obj"

#endif // !anvil_MAJOR_VERSION

#endif // !FLAGS_H


