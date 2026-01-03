# Importand dependencies to build in a Linux system.
# sudo apt install gcc
# sudo apt install g++
# sudo apt install xorg-dev
# sudo apt install libwayland-dev
# sudo apt install libxkbcommon-dev

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
    set(CMAKE_C_FLAGS "-Wall -Werror -Wno-missing-braces -Wno-stringop-overread -msse4.1 -O3 -DGLEW_NO_GLU" CACHE STRING "" FORCE)
    add_executable(anvil "${Network_Files}" "${Python_Files}" "source/main.c" "source/shaders/mainShader.c" "source/shaders/displayShader.c" "source/shaders/testShader.c" "source/components/rasterization.c" "source/components/scene.c" "source/components/mesh.c" "source/components/logging.c" "source/components/matrices.c" "source/components/vec_math.c" "source/components/quaternions.c" "source/components/camera.c" "source/components/physics.c" "source/components/bmp.c" "source/components/terrain.c" "source/components/openGL.c" "source/components/dbapi.c" "source/components/obj.c" "source/components/collisions.c" "source/components/rigid.c" "source/components/general.c" "source/shaders/rigidShader.c" "source/components/model.c" "source/components/animations.c" "source/components/animText.c")
elseif (WIN32)
    set(CMAKE_C_FLAGS "" CACHE STRING "" FORCE)
    add_executable(anvil "${Network_Files}" "${Python_Files}" "source/main.c" "source/shaders/mainShader.c" "source/shaders/displayShader.c" "source/shaders/testShader.c" "source/components/rasterization.c" "source/components/scene.c" "source/components/mesh.c" "source/components/logging.c" "source/components/matrices.c" "source/components/vec_math.c" "source/components/quaternions.c" "source/components/camera.c" "source/components/physics.c" "source/components/bmp.c" "source/components/terrain.c" "source/components/openGL.c" "source/components/dbapi.c" "source/components/obj.c" "source/components/collisions.c" "source/components/rigid.c" "source/components/general.c" "source/shaders/rigidShader.c" "source/components/model.c" "source/components/animations.c" "source/components/animText.c" "source/components/relations.c")
endif()

# LIBRARIES  ####################################################################################
# Fix the Legacy OpenGL warning on Linux when building.
set(OpenGL_GL_PREFERENCE GLVND)

# Initialize GLFW external library!
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_WIN32 OFF CACHE BOOL "" FORCE)
add_subdirectory("libraries/glfw-3.4/")
target_link_libraries(anvil PUBLIC glfw)

# Initialize GLEW external library and copy dll to the same folder as the .exe file!
set(GLEW_CUSTOM_OUTPUT_DIRS 1)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory("libraries/glew-2.1.0/build/cmake")
target_link_libraries(anvil PUBLIC glew)
if (WIN32)
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/glew32.dll")
        file(COPY "libraries/glew-2.1.0/bin/release/x64/glew32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
    endif()
endif()

# Initialize SQLITE3 library.
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/sqlite-amalgamation/build")
    execute_process(COMMAND cmake -B "${PROJECT_SOURCE_DIR}/sqlite-amalgamation/build"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/sqlite-amalgamation/"
        COMMAND_ECHO STDOUT
    )
endif()
add_subdirectory("libraries/sqlite-amalgamation")
target_link_libraries(anvil PUBLIC SQLite3)

# LIBRARIES  ####################################################################################

# LINK TO COMPILE  ##############################################################################
if (WIN32)
    if(NETWORK_INTERFACE)
        target_link_libraries(anvil PUBLIC "Ws2_32.lib")
    endif()
endif()
# LINK TO COMPILE  ##############################################################################

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


