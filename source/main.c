#include "headers/main.h"

/* Window app Global variables. */
GLFWwindow *window;
int width, height, Debug = 1;

int main(int argc, char *argv[]) {

    printf("anvil Version Major       : %d\n", anvil_VERSION_MAJOR);
    printf("anvil Version Minor       : %d\n", anvil_VERSION_MINOR);
    printf("Client Operating System   : %s\n", CLIENT_OS);

    /* Initialize the library */
    if ( !glfwInit() )
        return -1;

    printf("GLFW Compile time config  : % s\n", glfwGetVersionString());
    int vers[3];
    glfwGetVersion(&vers[0], &vers[1], &vers[2]);
    printf("GLFW Version Major        : %d\n", vers[0]);
    printf("GLFW Version Minor        : %d\n", vers[1]);
    printf("GLFW Version revision     : %d\n", vers[2]);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "anvil", NULL, NULL);
    if ( !window ) {
        glfwTerminate();
        return -1;
    }
    glfwGetWindowSize(window, &width, &height);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize Rasterization components. (framebuffers, user defined textures, shaders) */
    initRasterComponents();

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose(window) ) {
        /* Render here */
        rasterize();

        /* Poll for and process events */
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}