#include "headers/components/rigid.h"

static char* composemeshRigidPath(mesh* m, const char name[]);

/* Create the rigid file path according to the type of the mesh. Different types of meshes may have diferent file paths. Thats why we must generalize them. */
static char *composemeshRigidPath(mesh *m, const char name[]) {
	char* dynamic_path = { 0 };
	if (m->type == MESH_TYPE_TERRAIN) {
		int path_length = (strlen(name) * 2) + 21; // Plus 1 here for the null termination \0.
		dynamic_path = malloc(path_length);
		if (!dynamic_path) {
			debug_log_error(stdout, "malloc()");
			debug_log_info(stdout, "%s\n", name);
			return 0;
		}
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        sprintf_s(dynamic_path, path_length, "terrains/%s/%s_rigid.obj", name, name);
#elif defined(LINUX) || defined(__linux__)
        snprintf(dynamic_path, path_length, "terrains/%s/%s_rigid.obj", name, name);
#endif
	} else {
		int path_length = (strlen(name) * 2) + 19; // Plus 1 here for the null termination \0.
		dynamic_path = malloc(path_length);
		if (!dynamic_path) {
			debug_log_error(stdout, "malloc()");
			debug_log_info(stdout, "%s\n", name);
			return 0;
		}
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        sprintf_s(dynamic_path, path_length, "meshes/%s/%s_rigid.obj", name, name);
#elif defined(LINUX) || defined(__linux__)
        snprintf(dynamic_path, path_length, "meshes/%s/%s_rigid.obj", name, name);
#endif
	}

	return dynamic_path;
}

void loadmeshRigid(mesh *m, const char name[]) {
	OBJ obj;
	char *dynamic_path = composemeshRigidPath(m, name);
	readOBJ(&obj, dynamic_path);
	free(dynamic_path);

	// Initialize the vectors array from .obj file data.
	m->rigid.v_indexes = (obj.v_indexes / 3);
	m->rigid.v = malloc(m->rigid.v_indexes * 16);
	if (!m->rigid.v) {
		debug_log_critical(stdout, "m->rigid.v");
		exit(0);
	}
	int v_index = 0;
	for (int i = 0; i < obj.v_indexes; i += 3) {
		memcpy(&m->rigid.v[v_index], &obj.v[i], 12);
		vec4SetW(&m->rigid.v[v_index], 1.f);
		v_index++;
	}

	// Initialize the normals array from .obj file data.
	m->rigid.n_indexes = (obj.n_indexes / 3);
	m->rigid.n = malloc(m->rigid.n_indexes * 16);
	if (!m->rigid.n) {
		debug_log_critical(stdout, "m->rigid.n");
		exit(0);
	}
	int n_index = 0;
	for (int i = 0; i < obj.n_indexes; i += 3) {
		memcpy(&m->rigid.n[n_index], &obj.n[i], 12);
		vec4SetW(&m->rigid.n[n_index], 0.f);
		n_index++;
	}

	// Initialize the faces array from .obj file data.
	m->rigid.vbo_indexes = (obj.f_indexes / 9) * 24;
	m->rigid.faces_indexes = m->rigid.vbo_indexes / 24;
	m->rigid.vecs_indexes = m->rigid.faces_indexes * 3;
	m->rigid.vbo_size = m->rigid.vbo_indexes * 4;

	m->rigid.vbo = malloc(m->rigid.vbo_size);
	if (!m->rigid.vbo) {
		debug_log_critical(stdout, "m->rigid.vbo");
		return;
	}

	int index = 0, vpad, tpad;
	for (int i = 0; i < obj.f_indexes; i++) {
		vpad = obj.f[i] * 3;
		m->rigid.vbo[index] = obj.v[vpad];
		m->rigid.vbo[index + 1] = obj.v[vpad + 1];
		m->rigid.vbo[index + 2] = obj.v[vpad + 2];
		i++;
		tpad = obj.f[i] * 2;
		m->rigid.vbo[index + 3] = obj.t[tpad];
		m->rigid.vbo[index + 4] = obj.t[tpad + 1];
		i++;
		vpad = obj.f[i] * 3;
		m->rigid.vbo[index + 5] = obj.n[vpad];
		m->rigid.vbo[index + 6] = obj.n[vpad + 1];
		m->rigid.vbo[index + 7] = obj.n[vpad + 2];
		index += 8;
	}

	/* Initialize the world starting position of the rigid body. */
	mat4x4 qm1 = modelMatfromQST(m->rigid.q, m->scale, m->coords.v[0]);
	setvec4arrayMulmat(m->rigid.v, m->rigid.v_indexes, qm1);
	mat4x4 qm2 = matfromQuat(m->rigid.q, m->coords.v[0]);
	setvec4arrayMulmat(m->coords.v, 4, qm2);
	setvec4arrayMulmat(m->rigid.n, m->rigid.n_indexes, qm2);

	meshTerrainCollision(m);
	getmeshRigidLimits(m);
	initMeshQuadInfo(m);

	createRigidVAO(m);
	free(m->rigid.vbo);
	releaseOBJ(&obj);
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getmeshRigidLimits(mesh *m) {
	m->rigid.min = _mm_set_ps1(INT_MAX);
	m->rigid.max = _mm_set_ps1(-INT_MAX);
	for (int i = 0; i < m->rigid.v_indexes; i++) {
		m->rigid.min = _mm_min_ps(m->rigid.min, m->rigid.v[i]);
		m->rigid.max = _mm_max_ps(m->rigid.max, m->rigid.v[i]);
	}
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getmeshRigidLimits(mesh *m) {
	m->rigid.min = setvec4(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
    m->rigid.max = setvec4(-INT_MAX, -INT_MAX, -INT_MAX, -INT_MAX);
    for (int i = 0; i < m->rigid.v_indexes; i++) {
        if (m->rigid.min.m128_f32[0] > m->rigid.v[i].m128_f32[0])
            m->rigid.min.m128_f32[0] = m->rigid.v[i].m128_f32[0];

        if (m->rigid.max.m128_f32[0] < m->rigid.v[i].m128_f32[0])
            m->rigid.max.m128_f32[0] = m->rigid.v[i].m128_f32[0];

        if (m->rigid.min.m128_f32[1] > m->rigid.v[i].m128_f32[1])
            m->rigid.min.m128_f32[1] = m->rigid.v[i].m128_f32[1];

        if (m->rigid.max.m128_f32[1] < m->rigid.v[i].m128_f32[1])
            m->rigid.max.m128_f32[1] = m->rigid.v[i].m128_f32[1];

        if (m->rigid.min.m128_f32[2] > m->rigid.v[i].m128_f32[2])
            m->rigid.min.m128_f32[2] = m->rigid.v[i].m128_f32[2];

        if (m->rigid.max.m128_f32[2] < m->rigid.v[i].m128_f32[2])
            m->rigid.max.m128_f32[2] = m->rigid.v[i].m128_f32[2];
    }
}
#endif // VECTORIZED_CODE #######################################################################################
void releaseRigid(mesh *m) {
	free(m->rigid.v);
	free(m->rigid.n);
	glDeleteVertexArrays(1, &m->VAO);
	glDeleteBuffers(1, &m->VBO);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//if (m->rigid.vbo)
	//    free(m->rigid.vbo);
}


