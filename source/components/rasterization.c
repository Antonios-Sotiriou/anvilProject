#include "headers/components/rasterization.h"

/* OpenGL Global veriables. */
GLint mainShaderProgram, displayShaderProgram, testShaderProgram;
GLint VBO, shadowDepthMap, shadowMapFBO, mainColorMap, mainDepthMap, mainInfoMap, mainFBO;
GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

/* Creates vertex buffers and vertex attribute Pointers. */
const static void createBuffers(void) {
    /* Main Vertex Buffer Object buffer initiallization. */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    /* Generate a framebuffer object to save the depth values for the shadow Map. */
    glGenFramebuffers(1, &shadowMapFBO);
    glGenFramebuffers(1, &mainFBO);
}
/* Creates user defined framebuffers and framebuffers textures. */
const static void createTextures(void) {
    /* Create a user specific framebuffer to use it for rendering instead of the default framebuffer.*/
    glGenTextures(1, &mainColorMap);
    printf("mainColorMap: %d\n", mainColorMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainColorMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Create a 2D Texture to use it as the depth buffer for the user created framebuffer. */
    glGenTextures(1, &mainDepthMap);
    printf("mainDepthMap: %d\n", mainDepthMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mainDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Create a INFO texture.*/
    glGenTextures(1, &mainInfoMap);
    printf("mainInfoMap: %d\n", mainInfoMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mainInfoMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32I, width, height, 0, GL_RG_INTEGER, GL_INT, NULL);

    /* Attach the generated 2D Texture to our Shadow Map framebuffer's depth buffer */
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainColorMap, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mainDepthMap, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mainInfoMap, 0);
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        perror("Incomplete mainFBO ");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* Create a 2D Texture to use it as the depth buffer for the Shadow Map. */
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    glGenTextures(1, &shadowDepthMap);
    printf("shadowDepthMap: %d\n", shadowDepthMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Attach the generated 2D Texture to our Shadow Map framebuffer's depth buffer */
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        perror("Incomplete shadowMapFBO ");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GLAPIENTRY glErrorReportCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf("%s type = %d, severity = %d, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
/* Initializing all rasterizing importand Global components. */
const void initRasterComponents(void) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* Initialize Glew and check for Errors. */
    const GLenum err = glewInit();
    if ( GLEW_OK != err ) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    printf("GLEW Version              : %s\n", glewGetString(GLEW_VERSION));
    printf("openGL VERSION            : %s\n", glGetString(GL_VERSION));
    printf("GLSL VERSION              : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("GL Renderer               : %s\n", glGetString(GL_RENDERER));

    if ( Debug ) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(glErrorReportCallback, 0);
    }

    /* Create user defined vertex array buffers, vertex Attribute pointers, user defined framebuffers and framebuffers textures. */
    createBuffers();
    createTextures();
    
    /* Initialize the shaders. */
    mainShaderProgram = initMainShader();
    displayShaderProgram = initDisplayShader();
    testShaderProgram = initTestShader();
}
/* Rasterizing Meshes with appropriate shaders. */
const void rasterize(void) {
    //project();
    testShader();
    displayTexture(0);
}


