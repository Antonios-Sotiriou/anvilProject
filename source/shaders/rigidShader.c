#include "headers/shaders/rigidShader.h"

const static char *vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 vsPos;\n"

"uniform mat4 vpMatrix;\n"
"uniform mat4 modelMatrix;\n"

"void main() {\n"
"    gl_Position = (vpMatrix * modelMatrix) * vec4(vsPos, 1.f);\n"
"}\n\0";
const static char *fragmentShaderSource = "#version 450 core\n"

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

    return shaderProgram;
}
void rigidShader(void) {

    glUseProgram(rigidShaderProgram);

    glPolygonMode(GL_FRONT, GL_LINE);

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    //glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&PROJECTION_M);

    mat4x4 modelMatrix;
    for (int i = 0; i < SCENE.mesh_indexes; i++) {

        if (SCENE.mesh[i].rigid.state == ENABLE) {
            modelMatrix = modelMatfromQST(SCENE.mesh[i].q, SCENE.mesh[i].scale, SCENE.mesh[i].coords.v[0]);

            glUniformMatrix4fv(1, 1, GL_FALSE, (GLfloat*)&modelMatrix);

            glBindVertexArray(SCENE.mesh[i].VAO);
            //glBufferSubData(GL_ARRAY_BUFFER, 0, SCENE.mesh[i].rigid.f_indexes * 36, arr);
            glDrawArrays(GL_TRIANGLES, 0, SCENE.mesh[i].rigid.vecs_indexes);
        }
    }

    debug_log_OpenGL();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPolygonMode(GL_FRONT, GL_FILL);
    //glEnable(GL_DEPTH_TEST);
}


