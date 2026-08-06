#include "headers/components/mesh.h"
#include "headers/components/internal_libraries/matrices.h"
#include "headers/components/internal_libraries/vec_math.h"
#include "headers/components/internal_libraries/quaternions.h"
#include "headers/flags.h"

#include <stdio.h>

void createMesh(mesh *m, ENTRY obj) {
    m->vbo_indexes = (obj.f_indexes / 9) * 24;
    m->faces_indexes = m->vbo_indexes / 24;
    m->vecs_indexes = m->faces_indexes * 3;
    m->vbo_size = m->vbo_indexes * 4;

    /* Allocating size for the vertex buffer object which will be realeased after we upload it on the GPU. */
    m->vbo = malloc(m->vbo_size);
    if (!m->vbo) {
        debug_log_error(stdout, "m->vbo = malloc(m->vbo_size)");
        return;
    }
    /* Allocating space for the mesh identification, null terminating, string name. +1 for the null terminating char. */
    m->cname = malloc(obj.cname_length + 1);
    if (!m->cname) {
        debug_log_error(stdout, "m->cname = malloc(obj.c_indexes)");
        return;
    }
    memcpy(m->cname, obj.cname, obj.cname_length + 1);
    m->cname_length = obj.cname_length;
    m->model_matrix = identityMatrix();
    m->coords.v[0] = setvec4(0.f, 0.f, 0.f, 1.f);
    m->q = unitQuat();
    m->scale = setvec4(1, 1, 1, 0);
    m->asset_type = ASSET_TYPE_MESH;

    int index = 0, vpad, tpad;
    for (int i = 0; i < obj.f_indexes; i++) {
        vpad = obj.f[i] * 3;
        m->vbo[index] = obj.v[vpad];
        m->vbo[index + 1] = obj.v[vpad + 1];
        m->vbo[index + 2] = obj.v[vpad + 2];
        i++;
        tpad = obj.f[i] * 2;
        m->vbo[index + 3] = obj.t[tpad];
        m->vbo[index + 4] = obj.t[tpad + 1];
        i++;
        vpad = obj.f[i] * 3;
        m->vbo[index + 5] = obj.n[vpad];
        m->vbo[index + 6] = obj.n[vpad + 1];
        m->vbo[index + 7] = obj.n[vpad + 2];
        index += 8;
    }

    createMeshVAO(m);
    free(m->vbo);
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
/* Releases allocated ressources of a mesh. */
void releaseMesh(mesh *m) {
    free(m->cname);
    free(m->children);

    releaseMeshVAO(m);

    if (m->owns_rigid == ENABLED)
        releaseRigid(&m->rigid);

    // if (m->owns_anim == ENABLED)
        releaseAnimations(&m->anim);
}


