#include "headers/components/openGL.h"

/* OpenGL Global veriables. */
GLint mainShaderProgram, displayShaderProgram, testShaderProgram, rigidShaderProgram;
GLuint mainFBO, shadowMapFBO;
static GLuint shadowDepthMap, mainColorMap, mainDepthMap, mainInfoMap;
const GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

const static void createFrameBuffers(void);
void static GLAPIENTRY glErrorReportCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

/* Initializing all rasterizing importand Global components. */
const int initOpenGLComponents(void) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* Initialize Glew and check for Errors. */
    const GLenum err = glewInit();
    if ( GLEW_OK != err ) {
        debug_log_critical(stdout, "glewInit()");
        debug_log_info(stdout, "%s\n", glewGetErrorString(3));
        return -1;
    }

    debug_log_info(stdout, "GLEW Version              : %s\n", glewGetString(GLEW_VERSION));
    debug_log_info(stdout, "openGL VERSION            : %s\n", glGetString(GL_VERSION));
    debug_log_info(stdout, "GLSL VERSION              : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    debug_log_info(stdout, "GL Renderer               : %s\n", glGetString(GL_RENDERER));

    if ( DEBUG_LVL_1 ) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(glErrorReportCallback, 0);
    }

    /* Create user defined framebuffers and framebuffers textures. */
    createFrameBuffers();

    /* Initialize the shaders. */
    mainShaderProgram = initMainShader();
    displayShaderProgram = initDisplayShader();
    testShaderProgram = initTestShader();
    rigidShaderProgram = initRigidShader();

    return 0;
}
/* Initializes user defined framebuffers and framebuffers textures. */
const static void createFrameBuffers(void) {
    /* Create a user specific framebuffer to use it for rendering instead of the default framebuffer.*/
    glGenFramebuffers(1, &mainFBO);

    glGenTextures(1, &mainColorMap);
    printf("mainColorMap: %d\n", mainColorMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainColorMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Create a 2D Texture to use it as the depth buffer for the user created framebuffer. */
    glGenTextures(1, &mainDepthMap);
    printf("mainDepthMap: %d\n", mainDepthMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mainDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Create a INFO texture.*/
    glGenTextures(1, &mainInfoMap);
    printf("mainInfoMap: %d\n", mainInfoMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mainInfoMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32I, WIDTH, HEIGHT, 0, GL_RG_INTEGER, GL_INT, NULL);

    /* Attach the generated 2D Texture to our Shadow Map framebuffer's depth buffer */
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainColorMap, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mainDepthMap, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mainInfoMap, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        debug_log_error(stdout, "glCheckFramebufferStatus()");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /* Create a 2D Texture to use it as the depth buffer for the Shadow Map. */
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    /* Create a user specific framebuffer to use it as our ShadowMap Buffer.*/
    glGenFramebuffers(1, &shadowMapFBO);

    /* Create a 2D Texture to use it as the depth buffer for the Shadow Map. */
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
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        debug_log_error(stdout, "glCheckFramebufferStatus()");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
/* Creating the Vertex Array Object (VAO) to store in the GPU.After this function we can release the vao pointer of the mesh if we want. */
void createMeshVAO(mesh *m) {
    glGenVertexArrays(1, &m->VAO);
    glBindVertexArray(m->VAO);
    glGenBuffers(1, &m->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
    glBufferData(GL_ARRAY_BUFFER, m->vbo_size, m->vbo, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}
/* Creating the Vertex Array Object (VAO) to store in the GPU.After this function we can release the vao pointer of the rigid if we want. */
void createRigidVAO(rigid *r) {
    glGenVertexArrays(1, &r->VAO);
    glBindVertexArray(r->VAO);
    glGenBuffers(1, &r->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, r->VBO);
    glBufferData(GL_ARRAY_BUFFER, r->vbo_size, r->vbo, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}
void releaseOpenGLComponents(void) {
    glDeleteFramebuffers(1, &mainFBO);
    glDeleteTextures(1, &mainColorMap);
    glDeleteTextures(1, &mainDepthMap);
    glDeleteTextures(1, &mainInfoMap);

    glDeleteFramebuffers(1, &shadowMapFBO);
    glDeleteTextures(1, &shadowDepthMap);
}
void static GLAPIENTRY glErrorReportCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf("%s type = %d, severity = %d, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
void glErrorReport(void) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        debug_log_error(stdout, "glGetError()");
    }
}


