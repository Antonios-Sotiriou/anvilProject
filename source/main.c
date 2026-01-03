#include "headers/main.h"

/* Window app Global variables. */
int WIDTH = 1000, HEIGHT = 1000, DISPLAY_RIGID = 0;
/* The global matrices which are not change so, or are change after specific input, or window events. */
mat4x4 LOOKAT_M, VIEW_M, PERSPECTIVE_M, PROJECTION_M;

static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    scene *s = glfwGetWindowUserPointer(win);
    switch (key) {
        case GLFW_KEY_W:
        case GLFW_KEY_A:
        case GLFW_KEY_S:
        case GLFW_KEY_D:
        case GLFW_KEY_Q:
        case GLFW_KEY_E:
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_LEFT:
        case GLFW_KEY_UP:
        case GLFW_KEY_DOWN:
            if (action != GLFW_REPEAT)
                movementDispatch(key, action);
            break;
        case GLFW_KEY_L:
            if (action == GLFW_PRESS)
                s->eyePoint = s->eyePoint == camera ? light : camera;
            break;
        case GLFW_KEY_B:
            if (action == GLFW_PRESS)
                DISPLAY_RIGID = DISPLAY_RIGID == 0 ? 1 : 0;
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_PRESS)
                s->model[light].velocity = vecAddvec(s->model[light].velocity, setvec4(0, 1000, 0, 0));
            s->model[light].rigid.grounded = 0;
            break;
    }
}
static void cursor_pos_callback(GLFWwindow *win, double x, double y) {
    scene *s = glfwGetWindowUserPointer(win);
    float radius = 1.f;
    int xoffset = s->lastMouseX - x;
    int yoffset = s->lastMouseY - y;

    if (abs(xoffset) > abs(yoffset)) {
        if (xoffset < 0)
            radius = -1.f;
        s->model[camera].rigid.q = rotationQuat(radius, 0.f, 1.f, 0.f);
    } else {
        if (yoffset < 0)
            radius = -1.f;

        s->model[camera].rigid.q = rotationQuat(radius, vec4ExtractX(s->model[camera].coords.v[1]), vec4ExtractY(s->model[camera].coords.v[1]), vec4ExtractZ(s->model[camera].coords.v[1]));
    }

    s->mouseX = s->lastMouseX = x;
    s->mouseY = s->lastMouseY = y;

    mat4x4 tm = matFromQuat(s->model[camera].rigid.q, s->model[0].mesh[3].coords.v[0]);

    setvec4ArrayMulmat(s->model[camera].coords.v, 4, tm);
    setfacesArrayMulMat(s->model[camera].rigid.f, s->model[camera].rigid.faces_indexes, tm);

    s->model[camera].q = multiplyQuats(s->model[camera].q, s->model[camera].rigid.q);

    //vec4 rad = vec4Normalize(vecSubvec(SCENE.mesh[3].coords.v[0], SCENE.model[camera].coords.v[0]));
    //SCENE.model[camera].rigid.velocity = vecAddvec(SCENE.mesh[3].coords.v[1], rad);
}
static void mouse_callback(GLFWwindow* win, int button, int action, int mods) {
    scene *s = glfwGetWindowUserPointer(win);
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        s->mouseX = (int)x;
        s->mouseY = (int)y;
        printf("x: %d    y: %d\n", s->mouseX, s->mouseY);

        s->model[0].visible = s->model[0].visible == 1 ? 0 : 1;

#if (NETWORK_INTERFACE)
        sendRequest("127.0.0.1", 8080, "GET /test_request");
#endif

        //GLint data[2];
        //glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
        //glReadBuffer(GL_COLOR_ATTACHMENT1);
        //glReadPixels(s->mouseX, HEIGHT - s->mouseY, 1, 1, GL_RG_INTEGER, GL_INT, &data);
        //printf("colour: %d %d\n", data[0], data[1]);

        //GLubyte data[4];
        //glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
        //glReadBuffer(GL_COLOR_ATTACHMENT0);
        //glReadPixels(s->mouseX, HEIGHT - s->mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
        //printf("colour: %d %d %d %d\n", data[0], data[1], data[2], data[3]);
    } else if (button == GLFW_MOUSE_BUTTON_2) {
        if (action == GLFW_PRESS) {
            /* Register a Cursor position callback function. */
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(win, cursor_pos_callback);
        } else {
            /* Unregister a Cursor position callback function. */
            glfwSetCursorPosCallback(win, NULL);
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            s->lastMouseX = s->WIDTH * 0.5f;
            s->lastMouseY = s->HEIGHT * 0.5f;
        }
    }
}
int main(int argc, char *argv[]) {

    scene mainScene = { 0 };
    initScene(&mainScene, WIDTH, HEIGHT);

    if (argc > 1) {
        printf("argc: %d\n", argc);
        if (strncmp(argv[1], "init", 4) == 0) {
            debug_log_info(stdout, "Starting Initialization of terrains ...\n");
            initTerrainsHeightMaps();
        }
    }

    debug_log_info(stdout, "anvil Version Major       : %d\n", anvil_VERSION_MAJOR);
    debug_log_info(stdout, "anvil Version Minor       : %d\n", anvil_VERSION_MINOR);
    debug_log_info(stdout, "Client Operating System   : %s\n", CLIENT_OS);

    /* Initialize the library */
    if ( !glfwInit() ) {
        debug_log_critical(stdout, "glfwInit()");
        return -1;
    }

    int vers[3];
    glfwGetVersion(&vers[0], &vers[1], &vers[2]);
    debug_log_info(stdout, "GLFW Compile time config  : %s\n", glfwGetVersionString());
    debug_log_info(stdout, "GLFW Version Major        : %d\n", vers[0]);
    debug_log_info(stdout, "GLFW Version Minor        : %d\n", vers[1]);
    debug_log_info(stdout, "GLFW Version revision     : %d\n", vers[2]);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(mainScene.WIDTH, mainScene.HEIGHT, "anvil", NULL, NULL);
    if ( !window ) {
        debug_log_critical(stdout, "glfwCreateWindow()");
        glfwTerminate();
        return -1;
    }

    /* Get window's dimensions. */
    //glfwGetWindowSize(window, &mainScene.WIDTH, &mainScene.HEIGHT);

    /* Pass a pointer to the window for retrieving in the key_callback. */
    glfwSetWindowUserPointer(window, &SCENE);

    /* Register a keyboarb callback function. */
    glfwSetKeyCallback(window, key_callback);

    /* Register a mouse click callback function. */
    glfwSetMouseButtonCallback(window, mouse_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize openGL Rasterization components. (framebuffers, user defined textures, shaders) */
    if ( initOpenGLComponents() ) {
        debug_log_critical(stdout, "initOpenGLComponents()");
        return -1;
    }

    /* Initialize timing and frame rate metrics. */
    metrics mtr = { 0 };

    /* Create and initialize the GLOBAL SCENE. */
    createScene();

    /* Create the Perspective Matrix which is in most cases constant. Changes only with window resize events. */
    PERSPECTIVE_M = perspectiveMatrix(45.f, mainScene.WIDTH / (float)mainScene.HEIGHT, 10.f, INT32_MAX);

    initTimeCounter(&mtr);
    // float time_diff;

    /* Enable NETWORK_INTERFACE */
#if (NETWORK_INTERFACE)
    enableNetworkInterface();
#endif

    /* Enable PYTHON_INTERFACE */
#if (PYTHON_INTERFACE)
    enablePythonAPI();
#endif

    //float time_diff = 0;
    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose(window) ) {
        /* Timing functions. */
        updateTimeCounter(&mtr);
        calculateFPS(&mtr);
        //printf("DeltaTime    : %f\n", mtr.deltaTime);
        //printf("FPS          : %4.1f\n", mtr.FPS);

        /* Apply physics */
        applyPhysics(&mtr);

        /* Animate models */
        animateModels();

        /* Draw the Global SCENE. */
        rasterize();

        /* Poll for and process events */
        glfwPollEvents();

        glfwSwapBuffers(window);

        /* Sleep so much as we need to keep us at 60 fps. */
        //time_diff = deltaTime > 0.016666 ? 0 : (0.016666 - deltaTime) * 100000;
        //usleep(time_diff);
    }

#if (NETWORK_INTERFACE)
    startTCPClient(0);
    /* Disable NETWORK_INTERFACE */
    disableNetworkInterface();
#endif

#if (PYTHON_INTERFACE)
    /* Disable PYTHON_INTERFACE */
    disablePythonAPI();
#endif

    glfwTerminate();

    /* Releases GLOBAL SCENE resources. */
    releaseScene();
    releaseOpenGLComponents();

    return 0;
}


