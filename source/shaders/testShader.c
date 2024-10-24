#include "headers/shaders/testShader.h"

const static char* vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 vsPos;\n"
"layout (location = 1) in vec2 vsTexels;\n"
"layout (location = 2) in vec3 vsNormal;\n"

"void main() {\n"
"    gl_Position = vec4(vsPos, 1.f);\n"
"}\n\0";
const static char* fragmentShaderSource = "#version 450 core\n"
"layout (location = 0) out vec4 FragColor;\n"

"void main() {\n"
"    FragColor = vec4(1.f, 0.f, 0.5f, 1.f);\n"
"}\n\0";

const int initTestShader(void) {
    int success, vertexShader, fragmentShader, shaderProgram;
    char infoLog[512];
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED.\n%s\n", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED.\n%s\n", infoLog);
    }

    /* Shaders programm creation and linking. */
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAMM::LINKING_FAILED.\n%s\n", infoLog);
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
void testShader(void) {

    glUseProgram(testShaderProgram);

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    GLfloat tri[24] = {
        0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        -1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f
    };

    glBufferData(GL_ARRAY_BUFFER, 24 * 4, tri, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    GLubyte data[4];
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(320, height - 240, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
    printf("colour: %d %d %d %d\n", data[0], data[1], data[2], data[3]);

    //GLenum err;
    //while ((err = glGetError()) != GL_NO_ERROR) {
    //    fprintf(stderr, "project < %d >  ", err);
    //    perror("OpenGL ERROR: ");
    //}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}