# Minimum CMake verion.
cmake_minimum_required(VERSION 3.20)

# Starting Project.
project(anvil VERSION 1.0)

set(CMAKE_C_STANDARD)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# Whether or not compile with networking enabled.
set(NETWORK_INTERFACE 0)
# Whether or not compile with embeded python enabled.
set(PYTHON_INTERFACE 0)


if (NETWORK_INTERFACE)
    set(Network_Files "source/network/c_interface/TCPServer.c" "source/network/c_interface/TCPClient.c" "source/network/requests/requests.c")
endif()

if (PYTHON_INTERFACE)
    set(Python_Files "source/pythonAPI/anvilPy.c")
endif()

# Creates an executable.
if (LINUX)
    set(CMAKE_C_FLAGS "-Wall -Werror -msse4.1 -g" CACHE STRING "" FORCE)
    add_executable(anvil "${Network_Files}" "${Python_Files}" "source/main.c" "source/shaders/mainShader.c" "source/shaders/displayShader.c" "source/shaders/testShader.c" "source/components/rasterization.c" "source/components/scene.c" "source/components/mesh.c" "source/components/logging.c" "source/components/matrices.c" "source/components/vec_math.c" "source/components/quaternions.c" "source/components/camera.c" "source/components/physics.c" "source/components/bmp.c" "source/components/terrain.c" "source/components/openGL.c" "libraries/sqlite/sqlite3.c" "source/components/dbapi.c" "source/components/obj.c" "source/components/collisions.c" "source/components/rigid.c" "source/components/general.c" "source/shaders/rigidShader.c" "source/components/model.c" "source/components/animations.c" "source/components/animText.c")
elseif (WIN32)
    set(CMAKE_C_FLAGS "" CACHE STRING "" FORCE)
    add_executable(anvil "${Network_Files}" "${Python_Files}" "source/main.c" "source/shaders/mainShader.c" "source/shaders/displayShader.c" "source/shaders/testShader.c" "source/components/rasterization.c" "source/components/scene.c" "source/components/mesh.c" "source/components/logging.c" "source/components/matrices.c" "source/components/vec_math.c" "source/components/quaternions.c" "source/components/camera.c" "source/components/physics.c" "source/components/bmp.c" "source/components/terrain.c" "source/components/openGL.c" "libraries/sqlite/shell.c" "libraries/sqlite/sqlite3.c" "source/components/dbapi.c" "source/components/obj.c" "source/components/collisions.c" "source/components/rigid.c" "source/components/general.c" "source/shaders/rigidShader.c" "source/components/model.c" "source/components/animations.c" "source/components/animText.c" "source/components/relations.c")
endif()

# LIBRARIES  ####################################################################################
# Initialize glfw external library!
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("libraries/glfw-3.4/")

# Initialize glew external library and copy dll to the same folder as the .exe file!
add_subdirectory("libraries/glew-2.1.0/build/cmake")
if (WIN32)
    file(COPY "libraries/glew-2.1.0/bin/release/x64/glew32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
endif()
# LIBRARIES  ####################################################################################

# LINK TO COMPILE  ##############################################################################
if (WIN32)
    target_link_libraries(anvil PUBLIC "opengl32.lib")
    target_link_libraries(anvil PUBLIC "${PROJECT_SOURCE_DIR}/libraries/glfw-3.4/build/src/release/glfw3.lib")
    target_link_libraries(anvil PUBLIC "${PROJECT_SOURCE_DIR}/libraries/glew-2.1.0/lib/release/x64/glew32.lib")
    if(NETWORK_INTERFACE)
        target_link_libraries(anvil PUBLIC "Ws2_32.lib")
    endif()
elseif (LINUX)
    target_link_libraries(anvil PUBLIC OpenGL)
    target_link_libraries(anvil PUBLIC GLX)
    target_link_libraries(anvil PUBLIC glfw)
    target_link_libraries(anvil PUBLIC glew)
    #list(REMOVE_ITEM anvil "${PROJECT_SOURCE_DIR}/libraries/sqlite/shell.c")
endif()
# LINK TO COMPILE  ##############################################################################

# PRINT USEFULL VARIABLES  ######################################################################
#include(CMakePrintHelpers)
#cmake_print_properties(TARGETS anvil PROPERTIES LOCATION INTERFACE_INCLUDE_DIRECTORIES)
#cmake_print_variables(PROJECT_BINARY_DIR PROJECT_SOURCE_DIR)
# PRINT USEFULL VARIABLES  ######################################################################

# add directories to compiler include path.  ####################################################
target_include_directories(anvil PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(anvil PUBLIC "${PROJECT_SOURCE_DIR}/headers")
# add directories to compiler include path.  ####################################################

if (NETWORK_INTERFACE)
    message(STATUS "Network Enabled  :    ${NETWORK_INTERFACE}")
    message(STATUS "NETWORK_INTERFACE:    ${NETWORK_INTERFACE}")
    message(STATUS "Network_files    :    ${Network_Files}")
endif()

if (PYTHON_INTERFACE)
    find_package (Python COMPONENTS Interpreter Development)
    if (Python_FOUND)
        file(COPY "${Python_RUNTIME_LIBRARY_DIRS}/python313.dll" DESTINATION "${PROJECT_BINARY_DIR}")

        target_include_directories(anvil PUBLIC "${Python_INCLUDE_DIRS}")
        target_link_libraries(anvil PUBLIC "${Python_LIBRARIES}")
    endif()
endif()

configure_file("main.h.in" "${PROJECT_SOURCE_DIR}/headers/cmake_variables.h")


