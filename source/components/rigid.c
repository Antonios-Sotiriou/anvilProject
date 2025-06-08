#include "headers/components/rigid.h"

static char *composemeshRigidPath(mesh *m);

/* Create the rigid file path according to the type of the mesh. Different types of meshes may have diferent file paths. Thats why we must generalize them. */
static char *composemeshRigidPath(mesh *m) {
	char *dynamic_path = { 0 };
	if (m->asset_type == ASSET_TYPE_TERRAIN) {
		int path_length = (strlen(m->cname) * 2) + strlen(anvil_SOURCE_DIR) + 22; // Plus 1 here for the null termination \0.
		dynamic_path = malloc(path_length);
		if (!dynamic_path) {
			debug_log_error(stdout, "dynamic_path = malloc(path_length)");
			debug_log_info(stdout, "%s\n", m->cname);
			return 0;
		}
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        sprintf_s(dynamic_path, path_length, "%s/terrains/%s/%s_rigid.obj", anvil_SOURCE_DIR, m->cname, m->cname);
#elif defined(LINUX) || defined(__linux__)
        snprintf(dynamic_path, path_length, "%s/terrains/%s/%s_rigid.obj", anvil_SOURCE_DIR, m->cname, m->cname);
#endif
	} else {
		int path_length = (strlen(m->cname) * 2) + strlen(anvil_SOURCE_DIR) + 12; // Plus 1 here for the null termination \0.
		dynamic_path = malloc(path_length);
		if (!dynamic_path) {
			debug_log_error(stdout, "dynamic_path = malloc(path_length)");
			debug_log_info(stdout, "%s\n", m->cname);
			return 0;
		}
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        sprintf_s(dynamic_path, path_length, "%s/models/%s/%s_rigid.obj", anvil_SOURCE_DIR, m->cname, m->cname);
#elif defined(LINUX) || defined(__linux__)
        snprintf(dynamic_path, path_length, "%s/models/%s/%s_rigid.obj", anvil_SOURCE_DIR, m->cname, m->cname);
#endif
	}

	return dynamic_path;
}
void loadmeshRigid(mesh *m) {
	OBJ obj;
	char *dynamic_path = composemeshRigidPath(m);
	readOBJ(&obj, dynamic_path);
	free(dynamic_path);
//
//	// Initialize the vectors array from .obj file data.
//	r->v_indexes = (obj.e->v_indexes / 3);
//	r->v = malloc(r->v_indexes * 16);
//	if (!r->v) {
//		debug_log_critical(stdout, "r->v");
//		exit(0);
//	}
//	int v_index = 0;
//	for (int i = 0; i < obj.e->v_indexes; i += 3) {
//		memcpy(&r->v[v_index], &obj.e->v[i], 12);
//		vec4SetW(&r->v[v_index], 1.f);
//		v_index++;
//	}
//
//	// Initialize the normals array from .obj file data.
//	r->n_indexes = (obj.e->n_indexes / 3);
//	r->n = malloc(r->n_indexes * 16);
//	if (!r->n) {
//		debug_log_critical(stdout, "r->n");
//		exit(0);
//	}
//	int n_index = 0;
//	for (int i = 0; i < obj.e->n_indexes; i += 3) {
//		memcpy(&r->n[n_index], &obj.e->n[i], 12);
//		vec4SetW(&r->n[n_index], 0.f);
//		n_index++;
//	}
//
//	// Initialize the faces array from .obj file data.
//	r->vbo_indexes = (obj.e->f_indexes / 9) * 24;
//	r->faces_indexes = r->vbo_indexes / 24;
//	r->vecs_indexes = r->faces_indexes * 3;
//	r->vbo_size = r->vbo_indexes * 4;
//
//	r->vbo = malloc(r->vbo_size);
//	if (!r->vbo) {
//		debug_log_critical(stdout, "r->vbo");
//		return;
//	}
//
//	int index = 0, vpad, tpad;
//	for (int i = 0; i < obj.e->f_indexes; i++) {
//		vpad = obj.e->f[i] * 3;
//		r->vbo[index] = obj.e->v[vpad];
//		r->vbo[index + 1] = obj.e->v[vpad + 1];
//		r->vbo[index + 2] = obj.e->v[vpad + 2];
//		i++;
//		tpad = obj.e->f[i] * 2;
//		r->vbo[index + 3] = obj.e->t[tpad];
//		r->vbo[index + 4] = obj.e->t[tpad + 1];
//		i++;
//		vpad = obj.e->f[i] * 3;
//		r->vbo[index + 5] = obj.e->n[vpad];
//		r->vbo[index + 6] = obj.e->n[vpad + 1];
//		r->vbo[index + 7] = obj.e->n[vpad + 2];
//		index += 8;
//	}
//
//	/* Initialize the world starting position of the rigid body. */
//	mat4x4 qm1 = modelMatfromQST(r->q, m->scale, m->coords.v[0]);
//	setvec4arrayMulmat(r->v, r->v_indexes, qm1);
//	mat4x4 qm2 = matfromQuat(r->q, m->coords.v[0]);
//	setvec4arrayMulmat(m->coords.v, 4, qm2);
//	setvec4arrayMulmat(r->n, r->n_indexes, qm2);
//
//	meshTerrainCollision(m);
//	getmeshRigidLimits(m);
//	initMeshQuadInfo(m);
//
//	createRigidVAO(m);
//	free(r->vbo);
	releaseOBJ(&obj);
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getRigidLimits(rigid *r) {
	r->min = _mm_set_ps1(INT_MAX);
	r->max = _mm_set_ps1(-INT_MAX);
	for (int i = 0; i < r->v_indexes; i++) {
		r->min = _mm_min_ps(r->min, r->v[i]);
		r->max = _mm_max_ps(r->max, r->v[i]);
	}
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getRigidLimits(rigid *r) {
	r->min = setvec4(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
    r->max = setvec4(-INT_MAX, -INT_MAX, -INT_MAX, -INT_MAX);
    for (int i = 0; i < r->v_indexes; i++) {
        if (r->min.m128_f32[0] > r->v[i].m128_f32[0])
            r->min.m128_f32[0] = r->v[i].m128_f32[0];

        if (r->max.m128_f32[0] < r->v[i].m128_f32[0])
            r->max.m128_f32[0] = r->v[i].m128_f32[0];

        if (r->min.m128_f32[1] > r->v[i].m128_f32[1])
            r->min.m128_f32[1] = r->v[i].m128_f32[1];

        if (r->max.m128_f32[1] < r->v[i].m128_f32[1])
            r->max.m128_f32[1] = r->v[i].m128_f32[1];

        if (r->min.m128_f32[2] > r->v[i].m128_f32[2])
            r->min.m128_f32[2] = r->v[i].m128_f32[2];

        if (r->max.m128_f32[2] < r->v[i].m128_f32[2])
            r->max.m128_f32[2] = r->v[i].m128_f32[2];
    }
}
#endif // VECTORIZED_CODE #######################################################################################
void releaseRigid(rigid *r) {
	free(r->v);
	free(r->n);
	glDeleteVertexArrays(1, &r->VAO);
	glDeleteBuffers(1, &r->VBO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


