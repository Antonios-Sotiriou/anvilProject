#include "headers/shaders/displayShader.h"

const static char *displayVertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 vsPos;\n"
"layout (location = 1) in vec2 vsTexels;\n"

"layout (location = 0) out vec2 fsTexels;\n"

"void main() {\n"

"    gl_Position = vec4(vsPos, 1.f);\n"
"    fsTexels = vsTexels;\n"
"}\n\0";
const static char *displayFragmentShaderSource = "#version 450 core\n"
"layout (location = 0) in vec2 fsTexels;\n"

"uniform sampler2D displayTexture;\n"

"layout (location = 0) out vec4 FragColor;\n"

"void main() {\n"
"    FragColor = vec4(texture(displayTexture, fsTexels).rgb, 1.f);\n"
//"    FragColor = vec4(1.f, 0.f, 0.f, 1.f);\n"
"}\n\0";

const int initDisplayShader(void) {
    int success, vertexShader, fragmentShader, shaderProgram;
    char infoLog[512];
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &displayVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        debug_log_error(stdout, infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &displayFragmentShaderSource, NULL);
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


