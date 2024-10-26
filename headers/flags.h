#ifndef FLAGS_H
#define FLAGS_H 1

#ifndef anvil_VERSION_MAJOR
    #include "headers/cmake_variables.h"
#endif // !anvil_VERSION_MAJOR

#define VECTORIZED_CODE

#ifdef anvil_VERSION_MAJOR
    /* Meshes paths. */
	#define CUBE_FLAT           anvil_SOURCE_DIR "/meshes/cube/cube_flat.obj"
	#define CUBE_SMOOTH         anvil_SOURCE_DIR "/meshes/cube/cube_smooth.obj"

#endif // !anvil_MAJOR_VERSION

#endif // !FLAGS_H


