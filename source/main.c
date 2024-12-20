#include "headers/main.h"

/* Window app Global variables. */
int WIDTH, HEIGHT, EYEPOINT = camera, DISPLAY_RIGID = 0;
/* The global matrices which are not change so, or are change after specific input, or window events. */
mat4x4 LOOKAT_M, VIEW_M, PERSPECTIVE_M, PROJECTION_M;

/* Display usefull measurements. */
float			        TimeCounter = 0, LastFrameTimeCounter = 0, deltaTime = 0, prevTime = 0.0, FPS = 0;
struct timeval		    tv, tv0;
int			            Frame = 0;

static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {

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
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS) {
                moveUp(action);
            } else if (action == GLFW_RELEASE) {
                moveUp(action);
            }
            break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) {
                moveDown(action);
            } else if (action == GLFW_RELEASE) {
                moveDown(action);
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
        case GLFW_KEY_C:
            if (action == GLFW_PRESS)
                EYEPOINT = EYEPOINT == camera ? light : camera;
            break;
        case GLFW_KEY_B:
            if (action == GLFW_PRESS)
                DISPLAY_RIGID = DISPLAY_RIGID == 0 ? 1 : 0;
            break;
    }
}
static void mouse_callback(GLFWwindow* win, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        printf("x: %d    y: %d\n", (int)x, (int)y);

        //GLint data[2];
        //glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
        //glReadBuffer(GL_COLOR_ATTACHMENT1);
        //glReadPixels(x, HEIGHT - y, 1, 1, GL_RG_INTEGER, GL_INT, &data);
        //printf("colour: %d %d\n", data[0], data[1]);

        //GLubyte data[4];
        //glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
        //glReadBuffer(GL_COLOR_ATTACHMENT0);
        //glReadPixels(x, HEIGHT - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
        //printf("colour: %d %d %d %d\n", data[0], data[1], data[2], data[3]);
    }
}
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // ################
const static uint64_t epoch = ((uint64_t)116444736000000000ULL);
void usleep(const int usec) {
    HANDLE timer = 0;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec); // Convert to 100 nanoseconds interval. Negative value indicates relative time.

    timer = CreateWaitableTimerA(NULL, TRUE, NULL);
    if (!timer)
        return;
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
/* Initializes the given timeval struct, to the time of the day, at the moment, at which this function was called. */
void gettimeofday(struct timeval *tp, struct timezone *tzp) {
    FILETIME file_time;
    SYSTEMTIME system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (uint64_t)((ularge.QuadPart - epoch) / 10000000L);
    tp->tv_usec = (uint64_t)(system_time.wMilliseconds * 1000);
}
#endif // !WIN32 ###########################################################
/* Initializes the global timeval struct, to the time of the day, at the moment, at which this function was called. */
static void initTimeCounter(void) {
    gettimeofday(&tv0, NULL);
}
/* Counts how much time has passed since the first time that we casted gettimeofday(). */
static void updateTimeCounter(void) {
    LastFrameTimeCounter = TimeCounter;
    gettimeofday(&tv, NULL);
    TimeCounter = (float)(tv.tv_sec - tv0.tv_sec) + 0.000001 * ((float)(tv.tv_usec - tv0.tv_usec));
    deltaTime = TimeCounter - LastFrameTimeCounter;
}
static void calculateFPS(void) {
    Frame++;
    if ((Frame % 30) == 0)
        FPS = 1.f / deltaTime;
    prevTime = TimeCounter;
}
int main(int argc, char *argv[]) {

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "anvil", NULL, NULL);
    if ( !window ) {
        debug_log_critical(stdout, "glfwCreateWindow()");
        glfwTerminate();
        return -1;
    }
    /* Get window's dimensions. */
    glfwGetWindowSize(window, &WIDTH, &HEIGHT);

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

    /* Create and initialize the GLOBAL SCENE. */
    createScene();

    /* Create the Perspective Matrix which is in most cases constant. Changes only with window resize events. */
    PERSPECTIVE_M = perspectiveMatrix(45.f, WIDTH / (float)HEIGHT, 100.f, INT32_MAX);

    initTimeCounter();
    float time_diff;

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose(window) ) {
        /* Timing functions. */
        updateTimeCounter();
        calculateFPS();
        //printf("DeltaTime    : %f\n", deltaTime);
        //printf("FPS          : %4.1f\n", FPS);

        /* Apply physics */
        applyPhysics();

        /* Draw the Global SCENE. */
        rasterize();

        /* Poll for and process events */
        glfwPollEvents();

        glfwSwapBuffers(window);

        /* Sleep so much as we need to keep us at 60 fps. */
        //time_diff = deltaTime > 0.016666 ? 0 : (0.016666 - deltaTime) * 100000;
        //usleep(time_diff);
    }

    glfwTerminate();

    /* Releases GLOBAL SCENE resources. */
    releaseScene();
    releaseOpenGLComponents();

    return 0;
}


