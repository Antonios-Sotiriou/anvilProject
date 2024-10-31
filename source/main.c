#include "headers/main.h"

/* Window app Global variables. */
int WIDTH, HEIGHT, DEBUG = 1;
/* The global matrices which are not change so, or are change after specific input, or window events. */
mat4x4 LOOKAT_M, VIEW_M, PERSPECTIVE_M, PROJECTION_M;

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {

    switch (key) {
        case GLFW_KEY_W :
            if (action == GLFW_PRESS) {
                moveForward(action);
            } else if (action == GLFW_RELEASE) {
                moveForward(action);
            }
            break;
        case GLFW_KEY_S :
            if (action == GLFW_PRESS) {
                moveBackward(action);
            } else if (action == GLFW_RELEASE) {
                moveBackward(action);
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                moveRight(action);
            } else if (action == GLFW_RELEASE) {
                moveRight(action);
            }
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                moveLeft(action);
            } else if (action == GLFW_RELEASE) {
                moveLeft(action);
            }
            break;
        case GLFW_KEY_D:
            if (action == GLFW_PRESS) {
                lookRight(action);
            } else if (action == GLFW_RELEASE) {
                lookRight(action);
            }
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS) {
                lookLeft(action);
            } else if (action == GLFW_RELEASE) {
                lookLeft(action);
            }
            break;
    }
}
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
    GLFWwindow* window = glfwCreateWindow(640, 480, "anvil", NULL, NULL);
    if ( !window ) {
        glfwTerminate();
        return -1;
    }
    /* Get window's dimensions. */
    glfwGetWindowSize(window, &WIDTH, &HEIGHT);

    /* Register a keyboarb callback function. */
    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize openGL Rasterization components. (framebuffers, user defined textures, shaders) */
    initRasterComponents();

    /* Create and initialize the GLOBAL SCENE. */
    createScene();

    /* Create the Perspective Matrix which is in most cases constant. Changes only with window resize events. */
    PERSPECTIVE_M = perspectiveMatrix(45.f, WIDTH / (float)HEIGHT, 100.f, _CRT_INT_MAX);

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose(window) ) {

        /* Apply physics */
        applyPhysics();

        /* Draw the Global SCENE. */
        rasterize();

        /* Poll for and process events */
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    /* Releases GLOBAL SCENE resources. */
    releaseScene();
    return 0;
}


