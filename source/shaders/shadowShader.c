#include "headers/shaders/shadowShader.h"

const static char *shadowVertexShaderSource = "#version 450 core\n"
    "layout (location = 0) in vec3 vsPos;\n"

    "uniform mat4 lightSpaceMatrix;\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 meshMatrix;\n"

    "void main() {\n"
    "    gl_Position = (lightSpaceMatrix * (modelMatrix * meshMatrix)) * vec4(vsPos, 1.f);\n"
    "}\n\0";
const static char *shadowFragmentShaderSource = "#version 450 core\n"
    "void main() {\n"
    // "    gl_FragDepth = gl_FragCoord.z;\n"
    "}\n\0";

const int initShadowShader(void) {
    int success, vertexShader, fragmentShader, shaderProgram;
    char infoLog[512];
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shadowVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED.\n%s\n", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shadowFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED.\n%s\n", infoLog);
    }

    /* Shaders programm creation and linking. */
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAMM::LINKING_FAILED.\n%s\n", infoLog);
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
void shadowShader(scene *s) {

    glUseProgram(shadowShaderProgram);

    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, s->buffers.shadowFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    mat4x4 lookAtMat = lookAtMatrix(s->model[light].coords.v[0], s->model[light].coords.v[1], s->model[light].coords.v[2], s->model[light].coords.v[3]);
    mat4x4 viewMatrix = inverseMatrix(lookAtMat);
    mat4x4 lightMatrix = matMulMat(viewMatrix, s->ORTHOGRAPHIC_M);

    glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&lightMatrix);

    for (int i = 0; i < s->model_indexes; i++) {

        if (s->model[i].visible) {

            glUniformMatrix4fv(1, 1, GL_FALSE, (GLfloat*)&s->model[i].model_matrix);

            for (int x = 0; x < s->model[i].mesh_indexes; x++) {

                glUniformMatrix4fv(2, 1, GL_FALSE, (GLfloat*)&s->model[i].mesh[x].model_matrix);

                glBindVertexArray(s->model[i].mesh[x].VAO);
                glDrawArrays(GL_TRIANGLES, 0, s->model[i].mesh[x].vecs_indexes);
            }
        }
    }
    glViewport(0, 0, s->WIDTH, s->HEIGHT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}