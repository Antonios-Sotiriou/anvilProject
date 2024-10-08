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

    /* immintrin Testing... ############################################################### */
    //clock_t start = clock(), diff;
    //typedef union vec16i {
    //    __m256i m256;
    //    __m128i m128[2];
    //    int i8[8];
    //    int16_t i16[16];
    //} vec16i;
    //int16_t test_values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    ////__m256 vec = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    //vec16i vec;
    //vec.m256 = _mm256_loadu_epi16(&test_values);
    //vec.m256 = _mm256_adds_epu16(vec.m256, _mm256_set1_epi16(10));

    ////printf("result: %f %f %f %f %f %f %f %f\n", 
    ////    vec.m256_f32[0], vec.m256_f32[1], vec.m256_f32[2], vec.m256_f32[3],
    ////    vec.m256_f32[4], vec.m256_f32[5], vec.m256_f32[6], vec.m256_f32[7]
    ////);
    //printf("result: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
    //    vec.i16[0], vec.i16[1], vec.i16[2], vec.i16[3],
    //    vec.i16[4], vec.i16[5], vec.i16[6], vec.i16[7],
    //    vec.i16[8], vec.i16[9], vec.i16[10], vec.i16[11],
    //    vec.i16[12], vec.i16[13], vec.i16[14], vec.i16[15]
    //);

    //diff = clock() - start;
    //float secs = diff / CLOCKS_PER_SEC;
    //printf("CPU time taken : %f secs\n", secs);
    /* immintrin Testing... ############################################################### */

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