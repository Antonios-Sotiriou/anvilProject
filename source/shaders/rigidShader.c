#include "headers/shaders/rigidShader.h"
#include <string.h>
static GLint rigidVBO, rigidVAO;
static void createRigidVAO(void);

const static char *vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 vsPos;\n"

"uniform mat4 vpMatrix;\n"

"void main() {\n"
"    gl_Position = vpMatrix * vec4(vsPos, 1.f);\n"
"}\n\0";
const static char *fragmentShaderSource = "#version 460 core\n"

"layout (location = 0) out vec4 FragColor;\n"

"void main() {\n"
"    FragColor = vec4(0.f, 1.f, 0.f, 1.f);\n"
"}\n\0";

const int initRigidShader(void) {
    int success, vertexShader, fragmentShader, shaderProgram;
    char infoLog[512];
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        debug_log_error(stdout, infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        debug_log_error(stdout, infoLog);
    }

    /* Shaders programm creation and linking. */
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        debug_log_error(stdout, infoLog);
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    createRigidVAO();

    return shaderProgram;
}
static void createRigidVAO(void) {
    /* Main Vertex Buffer Object buffer initiallization. */
    glGenVertexArrays(1, &rigidVAO);
    glBindVertexArray(rigidVAO);
    glGenBuffers(1, &rigidVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rigidVBO);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);
}
void rigidShader(void) {

    glUseProgram(rigidShaderProgram);

    glPolygonMode(GL_FRONT, GL_LINE);

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glDisable(GL_DEPTH_TEST);
    //glClear(GL_DEPTH_BUFFER_BIT);

    GLfloat vpMatrix[16];
    memcpy(&vpMatrix, &PROJECTION_M, 64);
    glUniformMatrix4fv(0, 1, GL_FALSE, vpMatrix);

    //glBindVertexArray(rigidVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    for (int i = 0; i < SCENE.mesh_indexes; i++) {

        if (SCENE.mesh[i].rigid.state == ENABLE) {

            int len = SCENE.mesh[i].rigid.f_indexes * 3, inx = 0;
            float *arr = malloc(len * 12);
            if (!arr)
                continue;

            for (int x = 0; x < SCENE.mesh[i].rigid.f_indexes; x++) {

                memcpy(&arr[inx], &SCENE.mesh[i].rigid.f[x].v[0], 12);
                inx += 3;
                memcpy(&arr[inx], &SCENE.mesh[i].rigid.f[x].v[1], 12);
                inx += 3;
                memcpy(&arr[inx], &SCENE.mesh[i].rigid.f[x].v[2], 12);
                inx += 3;
            }

            glBufferData(GL_ARRAY_BUFFER, SCENE.mesh[i].rigid.f_indexes * 36, arr, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, SCENE.mesh[i].rigid.f_indexes * 3);
            free(arr);
        }
    }

    debug_log_OpenGL();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisableVertexAttribArray(0);

    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_DEPTH_TEST);
}


