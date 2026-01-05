#include "headers/shaders/testShader.h"

const static char *vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 vsPos;\n"
"layout (location = 1) in vec2 vsTexels;\n"
"layout (location = 2) in vec3 vsNormal;\n"

"uniform mat4 vpMatrix;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 meshMatrix;\n"
//"uniform int mesh_id;\n"

//"layout (location = 0) out int id;\n"

"void main() {\n"
"    gl_Position = (vpMatrix * (modelMatrix * meshMatrix)) * vec4(vsPos, 1.f);\n"
//"    id = mesh_id;\n"
"}\n\0";
const static char *fragmentShaderSource = "#version 450 core\n"
//"layout (location = 0) in flat int id;\n"

 //"float near = 0.1f;\n"
 //"float far = 100.f;\n"
 //"float linearizeDepth(float depth) {\n"
 //"    float z = (depth * 2.f) - 1.f;\n"
 //"    return (2.f * near * far) / (far + near - z * (far - near));\n"
 //"};\n"

"layout (location = 0) out vec4 FragColor;\n"

"void main() {\n"
"    FragColor = vec4(1.f, 0.f, 0.5f, 1.f);\n"
 //"    gl_FragDepth = linearizeDepth(gl_FragCoord.z) / far;\n"
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
void testShader(scene *s) {

    glUseProgram(testShaderProgram);

    //glPolygonMode(GL_FRONT, GL_LINE);

    //glViewport(0, 0, s->WIDTH, s->HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    /* Just for testing purposes code. ##################### */
    glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&s->PROJECTION_M);

    for (int i = 0; i < s->model_indexes; i++) {
        // View Frustum Culling implementation.
        if (rigidFrustumCulling(&s->model[i].rigid, &s->PROJECTION_M))
            continue;

        if (s->model[i].visible) {

            glUniformMatrix4fv(1, 1, GL_FALSE, (GLfloat*)&s->model[i].model_matrix);

            for (int x = 0; x < s->model[i].mesh_indexes; x++) {

                glUniformMatrix4fv(2, 1, GL_FALSE, (GLfloat*)&s->model[i].mesh[x].model_matrix);
                //glUniform1i(2, i + 1);
                glBindVertexArray(s->model[i].mesh[x].VAO);
                glDrawArrays(GL_TRIANGLES, 0, s->model[i].mesh[x].vecs_indexes);
            }
        }
    }
    /* Just for testing purposes code. ##################### */

    debug_log_OpenGL();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glPolygonMode(GL_FRONT, GL_FILL);
}


