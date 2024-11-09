# Minimum CMake verion.
cmake_minimum_required(VERSION 3.20)

# Starting Project.
project(anvil VERSION 1.0)

set(CMAKE_C_STANDARD)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# Creates an executable.
add_executable(anvil "source/main.c" "headers/main.h" "source/shaders/mainShader.c" "headers/shaders/mainShader.h" "source/shaders/displayShader.c" "headers/shaders/displayShader.h" "headers/shaders/testShader.h" "source/shaders/testShader.c" "source/components/rasterization.c" "headers/components/rasterization.h" "headers/flags.h" "headers/structs.h" "headers/global.h" "headers/components/scene.h" "source/components/scene.c" "headers/components/mesh.h" "source/components/mesh.c" "headers/components/logging.h" "headers/components/matrices.h" "headers/components/quaternions.h" "headers/components/vec_math.h" "source/components/logging.c" "source/components/matrices.c" "source/components/vec_math.c" "source/components/quaternions.c" "headers/components/camera.h" "source/components/camera.c" "headers/components/physics.h" "source/components/physics.c" "headers/components/bmp.h" "source/components/bmp.c" "headers/components/terrain.h" "source/components/terrain.c" "headers/components/openGL.h" "source/components/openGL.c" "libraries/sqlite/shell.c" "libraries/sqlite/sqlite3.c" "libraries/sqlite/sqlite3.h" "libraries/sqlite/sqlite3ext.h" "headers/components/dbapi.h" "source/components/dbapi.c")

configure_file("main.h.in" "${PROJECT_SOURCE_DIR}/headers/cmake_variables.h")

# LIBRARIES  ####################################################################################
# Initialize glfw external library!
add_subdirectory("libraries/glfw-3.4/")

# Initialize glew external library and copy dll to the same folder as the .exe file!
add_subdirectory("libraries/glew-2.1.0/build/cmake")
file(COPY "libraries/glew-2.1.0/bin/release/x64/glew32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
# LIBRARIES  ####################################################################################

# LINK TO COMPILE  ##############################################################################
target_link_libraries(anvil PUBLIC "opengl32.lib")
target_link_libraries(anvil PUBLIC "${PROJECT_SOURCE_DIR}/libraries/glfw-3.4/build/src/release/glfw3.lib")
target_link_libraries(anvil PUBLIC "${PROJECT_SOURCE_DIR}/libraries/glew-2.1.0/lib/release/x64/glew32.lib")
# LINK TO COMPILE  ##############################################################################

# PRINT USEFULL VARIABLES  ######################################################################
#include(CMakePrintHelpers)
#cmake_print_properties(TARGETS anvil PROPERTIES LOCATION INTERFACE_INCLUDE_DIRECTORIES)
#cmake_print_variables(PROJECT_BINARY_DIR PROJECT_SOURCE_DIR)
# PRINT USEFULL VARIABLES  ######################################################################

# add directories to compiler include path.  ####################################################
target_include_directories(anvil PUBLIC "${PROJECT_SOURCE_DIR}")
# add directories to compiler include path.  ####################################################

