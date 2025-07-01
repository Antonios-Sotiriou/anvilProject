#include "headers/components/rigid.h"

void loadModelRigid(model *m) {
	char path[100] = { 0 };
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	sprintf_s(path, 100, "%s/models/%s/%s_rigid.obj\0", anvil_SOURCE_DIR, m->cname, m->cname);
#elif defined(LINUX) || defined(__linux__)
	snprintf(path, 100, "%s/models/%s/%s_rigid.obj\0", anvil_SOURCE_DIR, m->cname, m->cname);
#endif

	OBJ obj = { 0 };
	readOBJ(&obj, path);

	// Iterate throught all the obj file entries to find the model rigid entry.
	for (int i = 0; i < obj.e_indexes; i++) {
		if (strncmp(m->cname, obj.e[i].cname, strlen(m->cname)) == 0) {
			printf("Entry found : %s\n", obj.e[i].cname);

			// Initialize the faces array from .obj file data.
			m->rigid.vbo_indexes = (obj.e[i].f_indexes / 9) * 24;
			m->rigid.faces_indexes = m->rigid.vbo_indexes / 24;
			m->rigid.vecs_indexes = m->rigid.faces_indexes * 3;
			m->rigid.vbo_size = m->rigid.vbo_indexes * 4;

			m->rigid.vbo = malloc(m->rigid.vbo_size);
			m->rigid.f = malloc(m->rigid.faces_indexes * 48);
			if (!m->rigid.vbo) {
				debug_log_critical(stdout, "m->rigid.vbo");
				return;
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
	mat4x4 qm1 = modelMatfromQST(m->rigid.q, m->scale, m->coords.v[0]);
	setfacearrayMulmat(m->rigid.f, m->rigid.faces_indexes, qm1);

	getRigidLimits(&m->rigid);
	//if (m->model_type != MODEL_TYPE_TERRAIN) {
	//	modelTerrainCollision(m);
	//	initModelQuadInfo(m);
	//}

	createRigidVAO(&m->rigid);
	free(m->rigid.vbo);
	releaseOBJ(&obj);
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
	r->min = setvec4(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
    r->max = setvec4(-INT_MAX, -INT_MAX, -INT_MAX, -INT_MAX);
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
	glDeleteVertexArrays(1, &r->VAO);
	glDeleteBuffers(1, &r->VBO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


