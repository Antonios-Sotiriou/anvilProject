#include "headers/components/mesh.h"

//void createMesh(mesh *m, const char name[]) {
//    int path_length = (strlen(name) * 2) + 13; // Plus 1 here for the null termination \0.
//    char* dynamic_path = malloc(path_length);
//    if (!dynamic_path) {
//        debug_log_error(stdout, "malloc()");
//        debug_log_info(stdout, "%s\n", name);
//        return;
//    }
//#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
//    sprintf_s(dynamic_path, path_length, "meshes/%s/%s.obj", name, name);
//#elif defined(LINUX) || defined(__linux__)
//    snprintf(dynamic_path, path_length, "meshes/%s/%s.obj", name, name);
//#endif
//
//    OBJ obj;
//    readOBJ(&obj, dynamic_path);
//    free(dynamic_path);
//
//    m->vbo_indexes = (obj.e->f_indexes / 9) * 24;
//    m->faces_indexes = m->vbo_indexes / 24;
//    m->vecs_indexes = m->faces_indexes * 3;
//    m->vbo_size = m->vbo_indexes * 4;
//
//    m->vbo = malloc(m->vbo_size);
//    if (!m->vbo) {
//        printf("Could not create mesh vbo. loadMesh() - malloc()\n");
//        return;
//    }
//
//    int index = 0, vpad, tpad;
//    for (int i = 0; i < obj.e->f_indexes; i++) {
//        vpad = obj.e->f[i] * 3;
//        m->vbo[index] = obj.e->v[vpad];
//        m->vbo[index + 1] = obj.e->v[vpad + 1];
//        m->vbo[index + 2] = obj.e->v[vpad + 2];
//        i++;
//        tpad = obj.e->f[i] * 2;
//        m->vbo[index + 3] = obj.e->t[tpad];
//        m->vbo[index + 4] = obj.e->t[tpad + 1];
//        i++;
//        vpad = obj.e->f[i] * 3;
//        m->vbo[index + 5] = obj.e->n[vpad];
//        m->vbo[index + 6] = obj.e->n[vpad + 1];
//        m->vbo[index + 7] = obj.e->n[vpad + 2];
//        index += 8;
//    }
//
//    createMeshVAO(m);
//    free(m->vbo);
//    releaseOBJ(&obj);
//}
/* Releases allocated ressources of a mesh. */
void releaseMesh(mesh *m) {
    glDeleteVertexArrays(1, &m->VAO);
    glDeleteBuffers(1, &m->VBO);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    if (m->rigid.state)
        releaseRigid(&m->rigid);
}


