#include "headers/components/openGL.h"

/* OpenGL Global veriables. */
GLint mainShaderProgram, displayShaderProgram, testShaderProgram, rigidShaderProgram, shadowShaderProgram;

void static GLAPIENTRY glErrorReportCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

/* Initializing all rasterizing importand Global components. */
void initOpenGLComponents(void) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* Initialize Glew and check for Errors. */
    const GLenum err = glewInit();
    if ( GLEW_OK != err ) {
        debug_log_critical(stdout, "glewInit()");
        debug_log_info(stdout, "%s\n", glewGetErrorString(3));
        return;
    }

    debug_log_info(stdout, "GLEW Version              : %s\n", glewGetString(GLEW_VERSION));
    debug_log_info(stdout, "openGL VERSION            : %s\n", glGetString(GL_VERSION));
    debug_log_info(stdout, "GLSL VERSION              : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    debug_log_info(stdout, "GL Renderer               : %s\n", glGetString(GL_RENDERER));

    if ( DEBUG_LVL_1 ) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(glErrorReportCallback, 0);
    }

    /* Initialize the shaders. */
    mainShaderProgram = initMainShader();
    displayShaderProgram = initDisplayShader();
    testShaderProgram = initTestShader();
    rigidShaderProgram = initRigidShader();
    shadowShaderProgram = initShadowShader();
}
/* Initializes user defined framebuffers and framebuffers textures. */
void createSceneFrameBuffers(scene *s) {
    /* Create a user specific framebuffer to use it for rendering instead of the default framebuffer.*/
    glGenFramebuffers(1, &s->buffers.mainFrameBuffer);

    glGenTextures(1, &s->textures.mainColorTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s->textures.mainColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, s->WIDTH, s->HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Create a 2D Texture to use it as the depth buffer for the user created framebuffer. */
    glGenTextures(1, &s->textures.mainDepthStencilTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, s->textures.mainDepthStencilTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, s->WIDTH, s->HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Create a INFO texture.*/
    glGenTextures(1, &s->textures.mainInfoTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, s->textures.mainInfoTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32I, s->WIDTH, s->HEIGHT, 0, GL_RG_INTEGER, GL_INT, NULL);

    /* Attach the generated 2D Texture to our Shadow Map framebuffer's depth buffer */
    glBindFramebuffer(GL_FRAMEBUFFER, s->buffers.mainFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s->textures.mainColorTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s->textures.mainDepthStencilTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, s->textures.mainInfoTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        debug_log_error(stdout, "glCheckFramebufferStatus()");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /* Create a 2D Texture to use it as the depth buffer for the Shadow Map. */
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    /* Create a user specific framebuffer to use it as our ShadowMap Buffer.*/
    glGenFramebuffers(1, &s->buffers.shadowFrameBuffer);

    /* Create a 2D Texture to use it as the depth buffer for the Shadow Map. */
    glGenTextures(1, &s->textures.shadowDepthTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, s->textures.shadowDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Attach the generated 2D Texture to our Shadow Map framebuffer's depth buffer */
    glBindFramebuffer(GL_FRAMEBUFFER, s->buffers.shadowFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s->textures.shadowDepthTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        debug_log_error(stdout, "glCheckFramebufferStatus()");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    s->buffers.drawBuffers[0] = GL_COLOR_ATTACHMENT0;
    s->buffers.drawBuffers[1] = GL_COLOR_ATTACHMENT1;
    s->textures.totalTextures = 4;
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
void releaseMeshVAO(mesh *m) {
    glDeleteBuffers(1, &m->VBO);
    glDeleteVertexArrays(1, &m->VAO);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
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
void releaseRigidVAO(rigid *r) {
    glDeleteBuffers(1, &r->VBO);
    glDeleteVertexArrays(1, &r->VAO);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
void releaseSceneFrameBuffers(scene *s) {
    glDeleteFramebuffers(1, &s->buffers.mainFrameBuffer);
    glDeleteTextures(1, &s->textures.msaaColorTexture);
    glDeleteTextures(1, &s->textures.mainDepthStencilTexture);
    glDeleteTextures(1, &s->textures.mainInfoTexture);

    glDeleteFramebuffers(1, &s->buffers.shadowFrameBuffer);
    glDeleteTextures(1, &s->textures.shadowDepthTexture);
}
void static GLAPIENTRY glErrorReportCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf("%s type = %d, severity = %d, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}
GLenum glCheckError_(const char *file, int line) {
    const char *errorString = { 0 };
    GLenum errorCode = { 0 };
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        switch (errorCode) {
            case GL_INVALID_ENUM:                  errorString = "INVALID_ENUM__________________"; break;
            case GL_INVALID_VALUE:                 errorString = "INVALID_VALUE_________________"; break;
            case GL_INVALID_OPERATION:             errorString = "INVALID_OPERATION_____________"; break;
            case GL_STACK_OVERFLOW:                errorString = "STACK_OVERFLOW________________"; break;
            case GL_STACK_UNDERFLOW:               errorString = "STACK_UNDERFLOW_______________"; break;
            case GL_OUT_OF_MEMORY:                 errorString = "OUT_OF_MEMORY_________________"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "INVALID_FRAMEBUFFER_OPERATION_"; break;
        }
        fprintf(stderr, "Error OpenGL: %s: File %s: Line %d\n", errorString, file, line);
    }
    return errorCode;
}


