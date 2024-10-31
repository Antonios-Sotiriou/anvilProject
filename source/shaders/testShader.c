#include "headers/shaders/testShader.h"
#include "headers/components/matrices.h"
#include "headers/components/quaternions.h"

const static char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 vsPos;\n"
"layout (location = 1) in vec2 vsTexels;\n"
"layout (location = 2) in vec3 vsNormal;\n"

"uniform mat4 vpMatrix;\n"
"uniform mat4 modelMatrix;\n"
//"uniform int mesh_id;\n"

//"layout (location = 0) out int id;\n"

"void main() {\n"
"    gl_Position = (vpMatrix * modelMatrix) * vec4(vsPos, 1.f);\n"
//"    id = mesh_id;\n"
"}\n\0";
const static char* fragmentShaderSource = "#version 460 core\n"
//"layout (location = 0) in flat int id;\n"

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

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    /* Just for testing purposes code. ##################### */

    GLfloat vpMatrix[16], modelMatrix[16];
    LOOKAT_M = lookatMatrix(SCENE.mesh[camera].coords.v[0], SCENE.mesh[camera].coords.v[1], SCENE.mesh[camera].coords.v[2], SCENE.mesh[camera].coords.v[3]);
    VIEW_M = inverseMatrix(LOOKAT_M);
    PROJECTION_M = matMulmat(VIEW_M, PERSPECTIVE_M);
    memcpy(&vpMatrix, &PROJECTION_M, 64);
    glUniformMatrix4fv(0, 1, GL_FALSE, vpMatrix);

    for (int i = 0; i < SCENE.mesh_indexes; i++) {
        mat4x4 qm = modelMatfromQST(SCENE.mesh[i].q, SCENE.mesh[i].scale, SCENE.mesh[i].coords.v[0]);
        memcpy(&modelMatrix, qm, 64);
        glUniformMatrix4fv(1, 1, GL_FALSE, modelMatrix);
        //glUniform1i(2, i + 1);

        glBufferData(GL_ARRAY_BUFFER, SCENE.mesh[i].vao_size, SCENE.mesh[i].vao, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, SCENE.mesh[i].vecs_indexes);
    }
    /* Just for testing purposes code. ##################### */

    //GLubyte data[4];
    //glReadBuffer(GL_COLOR_ATTACHMENT0);
    //glReadPixels(320, HEIGHT - 240, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
    //printf("colour: %d %d %d %d\n", data[0], data[1], data[2], data[3]);

    //GLenum err;
    //while ((err = glGetError()) != GL_NO_ERROR) {
    //    fprintf(stderr, "project < %d >  ", err);
    //    perror("OpenGL ERROR: ");
    //}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}