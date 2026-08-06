#include "headers/components/rigid.h"
#include "headers/components/internal_libraries/obj.h"
#include "headers/components/internal_libraries/matrices.h"
#include "headers/components/internal_libraries/vec_math.h"
#include "headers/components/internal_libraries/quaternions.h"
#include "headers/flags.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

void loadModelRigid(model *m) {
	char path[100] = { 0 };

	anvil_snprintf(path, 100, "%s/models/%s/%s_rigid.obj", anvil_SOURCE_DIR, m->cname, m->cname);

	OBJ obj = { 0 };
	readOBJ(&obj, path);

	// Iterate throught all the obj file entries to find the model rigid entry.
	for (int i = 0; i < obj.e_indexes; i++) {
		if (strncmp(m->cname, obj.e[i].cname, strlen(m->cname)) == 0) {

			// Initialize the faces array from .obj file data.
			m->rigid.vbo_indexes = (obj.e[i].f_indexes / 9) * 24;
			m->rigid.faces_indexes = m->rigid.vbo_indexes / 24;
			m->rigid.vecs_indexes = m->rigid.faces_indexes * 3;
			m->rigid.vbo_size = m->rigid.vbo_indexes * 4;

			m->rigid.vbo = malloc(m->rigid.vbo_size);
			if (!m->rigid.vbo) {
				debug_log_critical(stdout, "m->rigid.vbo");
				exit(EXIT_FAILURE);				
			}
			m->rigid.f = malloc(m->rigid.faces_indexes * 48);
			if (!m->rigid.f) {
				debug_log_critical(stdout, "m->rigid.f");
				exit(EXIT_FAILURE);
			}

			// Initialize the rigid body VBO which we send to the GPU.
			int index = 0, vpad, tpad;
			for (int x = 0; x < obj.e[i].f_indexes; x++) {
				vpad = obj.e[i].f[x] * 3;
				m->rigid.vbo[index] = obj.e[i].v[vpad];
				m->rigid.vbo[index + 1] = obj.e[i].v[vpad + 1];
				m->rigid.vbo[index + 2] = obj.e[i].v[vpad + 2];
				x++;

				tpad = obj.e[i].f[x] * 2;
				m->rigid.vbo[index + 3] = obj.e[i].t[tpad];
				m->rigid.vbo[index + 4] = obj.e[i].t[tpad + 1];
				x++;

				vpad = obj.e[i].f[x] * 3;
				m->rigid.vbo[index + 5] = obj.e[i].n[vpad];
				m->rigid.vbo[index + 6] = obj.e[i].n[vpad + 1];
				m->rigid.vbo[index + 7] = obj.e[i].n[vpad + 2];
				index += 8;
			}
		}
	}

	// Initialize the rigid body faces array which we use for collision detection.
	int index = 0;
	for (int x = 0; x < m->rigid.faces_indexes; x++) {

		for (int y = 0; y < 3; y++) {

			memcpy(&m->rigid.f[x].v[y], &m->rigid.vbo[index], 12);
			vec4SetW(&m->rigid.f[x].v[y], 1.f);
			index += 8;
		}
	}

	/* Initialize the world starting position of the rigid body. */
	mat4x4 qm1 = modelMatFromQST(m->rigid.q, m->scale, m->coords.v[0]);
	setfacesArrayMulMat(m->rigid.f, m->rigid.faces_indexes, qm1);

	getRigidLimits(&m->rigid);

	//if (m->model_type != MODEL_TYPE_TERRAIN) {
	//	modelTerrainCollision(m);
	//	initModelQuadInfo(m);
	//}

	createRigidVAO(&m->rigid);
	free(m->rigid.vbo);
	releaseOBJ(&obj);
}
/* Creating the Vertex Array Object (VAO) to store in the GPU.After this function we can release the vao pointer of the rigid if we want. */
void createRigidVAO(rigid *r) {
	glGenVertexArrays(1, &r->VAO);
	glBindVertexArray(r->VAO);
	glGenBuffers(1, &r->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, r->VBO);
	glBufferData(GL_ARRAY_BUFFER, r->vbo_size, r->vbo, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}
void releaseRigidVAO(rigid *r) {
	glDeleteBuffers(1, &r->VBO);
	glDeleteVertexArrays(1, &r->VAO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getRigidLimits(rigid *r) {
	r->min = _mm_set_ps1(INT_MAX);
	r->max = _mm_set_ps1(-INT_MAX);
	for (int x = 0; x < r->faces_indexes; x++) {
		for (int y = 0; y < 3; y++) {
			r->min = _mm_min_ps(r->min, r->f[x].v[y]);
			r->max = _mm_max_ps(r->max, r->f[x].v[y]);
		}
	}
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getRigidLimits(rigid *r) {
	r->min = setvec4(INT_MAX, INT_MAX, INT_MAX, 1.f);
    r->max = setvec4(-INT_MAX, -INT_MAX, -INT_MAX, 1.f);
    for (int x = 0; x < r->faces_indexes; x++) {
		for (int y = 0; y < 3; y++) {
			if (r->min.m128_f32[0] > r->f[x].v[y].m128_f32[0])
				r->min.m128_f32[0] = r->f[x].v[y].m128_f32[0];

			if (r->max.m128_f32[0] < r->f[x].v[y].m128_f32[0])
				r->max.m128_f32[0] = r->f[x].v[y].m128_f32[0];

			if (r->min.m128_f32[1] > r->f[x].v[y].m128_f32[1])
				r->min.m128_f32[1] = r->f[x].v[y].m128_f32[1];

			if (r->max.m128_f32[1] < r->f[x].v[y].m128_f32[1])
				r->max.m128_f32[1] = r->f[x].v[y].m128_f32[1];

			if (r->min.m128_f32[2] > r->f[x].v[y].m128_f32[2])
				r->min.m128_f32[2] = r->f[x].v[y].m128_f32[2];

			if (r->max.m128_f32[2] < r->f[x].v[y].m128_f32[2])
				r->max.m128_f32[2] = r->f[x].v[y].m128_f32[2];
		}
    }
}
#endif // VECTORIZED_CODE #######################################################################################
void releaseRigid(rigid *r) {
	free(r->f);
    releaseRigidVAO(r);
}


