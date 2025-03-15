#ifndef FLAGS_H
#define FLAGS_H 1

#ifndef anvil_VERSION_MAJOR
    #include "headers/cmake_variables.h"
#endif // !anvil_VERSION_MAJOR

#define VECTORIZED_CODE

#ifdef anvil_VERSION_MAJOR
    /* The meshes which consist the scene enumeration. */ 
    enum { terrain, camera, light, initAssets };

    /* Databases paths. */
    #define GITANA_DB           anvil_SOURCE_DIR"/databases/gitana.db"

    /* Database entries Types. */
    #define MESH_TABLE                0
    #define MODEL_TABLE               1
    #define SCENE_TABLE               2

    /* Flags for states changing. */
    #define ENABLED             1
    #define DISABLED            0
    #define ON                  1
    #define OFF                 0

    /* Assets Types types. */
    #define ASSET_TYPE_MESH          0
    #define ASSET_TYPE_MODEL         1

    /* Models types. */
    #define MODEL_TYPE_TERRAIN             0
    #define MODEL_TYPE_CAMERA              1
    #define MODEL_TYPE_LIGHT               2
    #define MODEL_TYPE_GENERAL             3

#endif // !anvil_MAJOR_VERSION

/* DEBUG SECTOR ################################################################################### */
#define DEBUG_LVL_1         0    // DEBUG LVL INFO    : general informations which do not affect program execution.
#define DEBUG_LVL_2         0    // DEBUG LVL MESSAGE : information about specific parts of code and variables.
#define DEBUG_LVL_3         0    // DEBUG LVL WARNING : program possible will produce wrong values but continues execution.
#define DEBUG_LVL_4         0    // DEBUG LVL ERROR   : program will crash possibly or it will produce wrong values.
#define DEBUG_LVL_5         1    // DEBUG LVL CRITICAL: program stops execution and exits with an error code.

// ENABLE DEBUG LVL ACCORDING TO WHICH LVL IS ENABLED.
#if DEBUG_LVL_2 == 1
    #if DEBUG_LVL_1 == 0
        #undef DEBUG_LVL_1
        #define DEBUG_LVL_1 1
    #endif
#endif
#if DEBUG_LVL_3 == 1
    #if DEBUG_LVL_1 == 0
        #undef DEBUG_LVL_1
        #define DEBUG_LVL_1 1
    #endif
    #if DEBUG_LVL_2 == 0
        #undef DEBUG_LVL_2
        #define DEBUG_LVL_2 1
    #endif
#endif
#if DEBUG_LVL_4 == 1
    #if DEBUG_LVL_1 == 0
        #undef DEBUG_LVL_1
        #define DEBUG_LVL_1 1
    #endif
    #if DEBUG_LVL_2 == 0
        #undef DEBUG_LVL_2
        #define DEBUG_LVL_2 1
    #endif
    #if DEBUG_LVL_3 == 0
        #undef DEBUG_LVL_3
        #define DEBUG_LVL_3 1
    #endif
#endif
#if DEBUG_LVL_5 == 1
    #if DEBUG_LVL_1 == 0
        #undef DEBUG_LVL_1
        #define DEBUG_LVL_1 1
    #endif
    #if DEBUG_LVL_2 == 0
        #undef DEBUG_LVL_2
        #define DEBUG_LVL_2 1
    #endif
    #if DEBUG_LVL_3 == 0
        #undef DEBUG_LVL_3
        #define DEBUG_LVL_3 1
    #endif
    #if DEBUG_LVL_4 == 0
        #undef DEBUG_LVL_4
        #define DEBUG_LVL_4 1
    #endif
#endif
// DEBUGGING MACROS.
#define debug_log_info(out, ...)\
        do { if (DEBUG_LVL_1) fprintf(stderr, "INFO     "__VA_ARGS__); } while (0)

#define debug_log_message(out, ...)\
        do { if (DEBUG_LVL_2) fprintf(stderr, "MESSAGE  %s\n", __VA_ARGS__); } while (0)


#define debug_log_warning(out, ...)\
        do { if (DEBUG_LVL_3) fprintf(stderr, "WARNING  %s %s %d()\n", __VA_ARGS__, __func__, __LINE__); } while (0)

#define debug_log_error(out, ...)\
        do { if (DEBUG_LVL_4) fprintf(stderr, "ERROR    %s %s %d()\n", __VA_ARGS__, __func__, __LINE__); } while (0)

#define debug_log_critical(out, ...)\
        do { if (DEBUG_LVL_5) fprintf(stderr, "CRITICAL %s %s %d %s()\n", __VA_ARGS__, __FILE__,\
                                        __LINE__, __func__); } while (0)

/* OpenGL debug macro function. */
#define debug_log_OpenGL()\
        if (DEBUG_LVL_4) glErrorReport();
/* DEBUG SECTOR ################################################################################### */

#endif // !FLAGS_H


