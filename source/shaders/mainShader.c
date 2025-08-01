#include "headers/shaders/mainShader.h"

const static char* vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 vsPos;\n"
"layout (location = 1) in vec2 vsTexels;\n"
"layout (location = 2) in vec3 vsNormal;\n"

"uniform mat4 vpMatrix;\n"
"uniform mat4 lightSpaceMatrix;\n"
"uniform mat4 modelMatrix;\n"
"uniform int mesh_id;\n"

"out VS_OUT {\n"
"    vec4 fsPos;\n"
"    vec3 fragPosWS;\n"
"    vec2 fsTexels;\n"
"    vec3 fsNormal;\n"
"    vec4 fsPosLightSpace;\n"
"    int id;\n"
"} vs_out;\n"

"void main() {\n"

"    vs_out.fsPos = (vpMatrix * modelMatrix) * vec4(vsPos, 1.f);\n"
"    vs_out.fragPosWS = vec3(modelMatrix * vec4(vsPos, 1.f));\n"
"    vs_out.fsTexels = vsTexels;\n"
"    vs_out.fsNormal = mat3(modelMatrix) * vsNormal;\n"
"    vs_out.fsPosLightSpace = (lightSpaceMatrix * modelMatrix) * vec4(vsPos, 1.f);\n"

"    gl_Position = vs_out.fsPos;\n"
"    vs_out.id = mesh_id;\n"
"}\n\0";
const static char* fragmentShaderSource = "#version 450 core\n"
"in VS_OUT {\n"
"    vec4 fsPos;\n"
"    vec3 fragPosWS;\n"
"    vec2 fsTexels;\n"
"    vec3 fsNormal;\n"
"    vec4 fsPosLightSpace;\n"
"    flat int id;\n"
"} fs_in;\n"

"uniform vec3 lightPos;\n"
"uniform vec3 cameraPos;\n"
"uniform sampler2D shadowDepthMap;\n"
"uniform sampler2D tex_index;\n"

"layout (location = 0) out vec4 FragColor;\n"
"layout (location = 1) out ivec2 mesh_info;\n"

// "float near = 0.1f;\n"
// "float far = 100.f;\n"
// "float linearizeDepth(float depth) {\n"
// "    float z = (depth * 2.f) - 1.f;\n"
// "    return (2.f * near * far) / (far + near - z * (far - near));\n"
// "};\n"

"float shadowCalculation(vec4 fsPosLightSpace) {\n"
// Perform Perspective Divide.
"    vec3 projCoords = fsPosLightSpace.xyz / fsPosLightSpace.w;\n"
// Transform to 0.f - 1.f range.
"    projCoords = (projCoords * 0.5f) + 0.5f;\n"
// Get closest depth value from light's perspective (using [0, 1] range fsPosLightSpace as coords).
"    float closestDepth = texture(shadowDepthMap, projCoords.xy).r;\n"
// Get depth of current fragment from light's perspective.
"    float currentDepth = projCoords.z;\n"
// Calculate bias (based on shadowDepthMap resolution and slope).
"    vec3 normal = normalize(fs_in.fsNormal);\n"
"    vec3 lightDir = normalize(lightPos - fs_in.fragPosWS);\n"
"    float bias = max(0.05f * (1.f - dot(normal, lightDir)), 0.005f);\n"
// Check whethercurrent fragment position is in shadow.
// PCF.
"    float shadow = 0.f;\n"
"    vec2 texelSize = 1.f / textureSize(shadowDepthMap, 0);\n"
"    for (int x = -1; x <= 1; x++) {\n"
"        for (int y = -1; y <= 1; y++) {\n"
"            float pcfDepth = texture(shadowDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
"            shadow += currentDepth - bias > pcfDepth ? 1.f : 0.f;\n"
"        }\n"
"    }\n"
"    shadow /= 9.f;\n"
// Keep the shadow at 0.f when outside the far_plane region of the light's frustum.
"    if ( projCoords.z > 1.f )\n"
"        shadow = 0.f;\n"

"    return shadow;\n"
"}\n"

"void main() {\n"

"    float ambientStr = 0.1;\n"
"    vec3 ambient = ambientStr * vec3(1.f, 1.f, 1.f);\n"

"    vec3 norm = normalize(fs_in.fsNormal);\n"
"    vec3 lightDir = normalize(lightPos - fs_in.fragPosWS);\n"
"    float diff = max(dot(norm, lightDir), 0.f);\n"
"    vec3 diffuse = diff * vec3(1.f, 1.f, 1.f);\n"

"    float specularStr = 0.5;\n"
"    vec3 viewDir = normalize(cameraPos - fs_in.fragPosWS);\n"

// "    vec3 reflectDir = normalize(lightDir + viewDir);\n"            // Blinn-Phong
"    vec3 reflectDir = reflect(-lightDir, norm);\n"              // Phong

// "    float spec = pow(max(dot(norm, reflectDir), 0.f), 32);\n"        // Blinn-Phong
"    float spec = pow(max(dot(viewDir, reflectDir), 0.f), 32);\n"  // Phong
"    vec3 specular = specularStr * spec * vec3(1.f, 1.f, 1.f);\n"

"    float shadow = shadowCalculation(fs_in.fsPosLightSpace);\n"
"    vec3 result = ((ambient + (1.f - shadow)) * (diffuse + specular)) * texture(tex_index, fs_in.fsTexels).bgr;\n"

"    FragColor = vec4(result, 1.f);\n"
"    mesh_info = ivec2(fs_in.id, gl_PrimitiveID);\n"
// "    gl_FragDepth = linearizeDepth(gl_FragCoord.z) / far;\n"
"}\n\0";

const int initMainShader(void) {
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
void mainShader(void) {

    glUseProgram(mainShaderProgram);

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
    glDrawBuffers(2, drawBuffers);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&PROJECTION_M);

    mat4x4 modelMatrix;
    for (int i = 0; i < SCENE.model_indexes; i++) {
        if (SCENE.model[i].visible) {
            modelMatrix = modelMatFromQST(SCENE.model[i].q, SCENE.model[i].scale, SCENE.model[i].coords.v[0]);
            glUniformMatrix4fv(1, 1, GL_FALSE, (GLfloat*)&modelMatrix);

            for (int x = 0; x < SCENE.model[i].mesh_indexes; x++) {
                glUniform1i(3, SCENE.model[i].mesh[x].pk);
                glBindVertexArray(SCENE.model[i].mesh[x].VAO);
                glDrawArrays(GL_TRIANGLES, 0, SCENE.model[i].mesh[x].vecs_indexes);
            }
        }
    }

    debug_log_OpenGL();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


