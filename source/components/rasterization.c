#include "headers/components/rasterization.h"

/* Rasterizing Meshes with appropriate shaders. */
const void rasterize(scene *s) {

    s->LOOKAT_M = lookAtMatrix(
        s->model[s->eyePoint].coords.v[0],
        s->model[s->eyePoint].coords.v[1],
        s->model[s->eyePoint].coords.v[2],
        s->model[s->eyePoint].coords.v[3]
    );
    s->VIEW_M = inverseMatrix(s->LOOKAT_M);
    s->PROJECTION_M = matMulMat(s->VIEW_M, s->PERSPECTIVE_M);

    //mainShader(s);
    testShader(s);
    if (s->DISPLAY_RIGID)
        rigidShader(s);

    drawOnSceneCanvas(&s->canvas, s->activeTexture);
}
const int rigidFrustumCulling(rigid *r, mat4x4 *PROJECTION_M) {
    rigid temp = { 0 };
    int f_bytes = r->faces_indexes * sizeof(face);
    temp.f = malloc(f_bytes);
    if (!temp.f) {
        debug_log_info("if (!temp.f)");
        return 1;
    }

    temp.faces_indexes = r->faces_indexes;
    memcpy(temp.f, r->f, f_bytes);

    setfacesArrayMulMat(temp.f, temp.faces_indexes, *PROJECTION_M);
    float w = 0;
    for (int i = 0; i < temp.faces_indexes; i++) {
        for (int y = 0; y < 3; y++) {
            w = vec4ExtractW(temp.f[i].v[y]);
            if (w > 0)
                temp.f[i].v[y] = vecDivf32(temp.f[i].v[y], w);
        }
    }

    getRigidLimits(&temp);

    vec4 min = temp.min;
    vec4 max = temp.max;

    free(temp.f);

    if (((vec4ExtractX(min) > 1.f) || (vec4ExtractX(max) < -1.f)) ||
        ((vec4ExtractY(min) > 1.f) || (vec4ExtractY(max) < -1.f)) ||
        ((vec4ExtractZ(min) > 1.f) || (vec4ExtractZ(max) < 0))) {
        return 1;
    }
    return 0;
}


