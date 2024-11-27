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

	m->rigid.f_indexes = (obj.f_indexes / 9);
	m->rigid.f = malloc(m->rigid.f_indexes * sizeof(face));
	if (!m->rigid.f) {
		debug_log_critical(stdout, "m->rigid.f");
		exit(0);
	}
	int f_index = 0, fpad = 0, npad, tpad;
	for (int i = 0; i < obj.f_indexes; i += 9) {
		for (int j = 0; j < 3; j++) {

			memcpy(&m->rigid.f[f_index].v[j], &m->rigid.v[obj.f[fpad]], 16);

			tpad = obj.f[fpad + 1] * 2;
			memcpy(&m->rigid.f[f_index].vt[j], &obj.t[tpad], 8);

			npad = obj.f[fpad + 2] * 3;
			memcpy(&m->rigid.f[f_index].vn[j], &obj.n[npad], 12);
			vec4SetW(&m->rigid.f[f_index].vn[j], 0.f);

			fpad += 3;
		}
		f_index++;
	}

	/* Initialize the world starting position of the rigid body. */
	mat4x4 qm1 = modelMatfromQST(m->rigid.q, m->scale, m->coords.v[0]);
	setfacearrayMulmat(m->rigid.f, m->rigid.f_indexes, qm1);
	mat4x4 qm2 = matfromQuat(m->rigid.q, m->coords.v[0]);
	setvec4arrayMulmat(m->coords.v, 4, qm2);

	getmeshRigidLimits(m);
	initMeshQuadInfo(m);

	releaseOBJ(&obj);
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getmeshRigidLimits(mesh *m) {
	m->rigid.min = _mm_set_ps1(INT_MAX);
	m->rigid.max = _mm_set_ps1(-INT_MAX);
	//for (int i = 0; i < m->rigid.v_indexes; i++) {
	//	m->rigid.min = _mm_min_ps(m->rigid.min, m->rigid.v[i]);
	//	m->rigid.max = _mm_max_ps(m->rigid.max, m->rigid.v[i]);
	//}
	for (int i = 0; i < m->rigid.f_indexes; i++) {
		for (int j = 0; j < 3; j++) {
			m->rigid.min = _mm_min_ps(m->rigid.min, m->rigid.f[i].v[j]);
			m->rigid.max = _mm_max_ps(m->rigid.max, m->rigid.f[i].v[j]);
		}
	}
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the meshe's Rigid vectors array extends. Populate with values the (min) and (max) Rigid vec4 values */
void getmeshRigidLimits(mesh *m) {
	m->rigid.min = setvec4(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
    m->rigid.max = setvec4(-INT_MAX, -INT_MAX, -INT_MAX, -INT_MAX);
    //for (int i = 0; i < m->rigid.v_indexes; i++) {
    //    if (m->rigid.min.m128_f32[0] > m->rigid.v[i].m128_f32[0])
    //        m->rigid.min.m128_f32[0] = m->rigid.v[i].m128_f32[0];

    //    if (m->rigid.max.m128_f32[0] < m->rigid.v[i].m128_f32[0])
    //        m->rigid.max.m128_f32[0] = m->rigid.v[i].m128_f32[0];

    //    if (m->rigid.min.m128_f32[1] > m->rigid.v[i].m128_f32[1])
    //        m->rigid.min.m128_f32[1] = m->rigid.v[i].m128_f32[1];

    //    if (m->rigid.max.m128_f32[1] < m->rigid.v[i].m128_f32[1])
    //        m->rigid.max.m128_f32[1] = m->rigid.v[i].m128_f32[1];

    //    if (m->rigid.min.m128_f32[2] > m->rigid.v[i].m128_f32[2])
    //        m->rigid.min.m128_f32[2] = m->rigid.v[i].m128_f32[2];

    //    if (m->rigid.max.m128_f32[2] < m->rigid.v[i].m128_f32[2])
    //        m->rigid.max.m128_f32[2] = m->rigid.v[i].m128_f32[2];
    //}
	for (int i = 0; i < m->rigid.f_indexes; i++) {
		for (int j = 0; j < 3; j++) {
			if (m->rigid.min.m128_f32[0] > m->rigid.f[i].v[j].m128_f32[0])
				m->rigid.min.m128_f32[0] = m->rigid.f[i].v[j].m128_f32[0];

			if (m->rigid.max.m128_f32[0] < m->rigid.f[i].v[j].m128_f32[0])
				m->rigid.max.m128_f32[0] = m->rigid.f[i].v[j].m128_f32[0];

			if (m->rigid.min.m128_f32[1] > m->rigid.f[i].v[j].m128_f32[1])
				m->rigid.min.m128_f32[1] = m->rigid.f[i].v[j].m128_f32[1];

			if (m->rigid.max.m128_f32[1] < m->rigid.f[i].v[j].m128_f32[1])
				m->rigid.max.m128_f32[1] = m->rigid.f[i].v[j].m128_f32[1];

			if (m->rigid.min.m128_f32[2] > m->rigid.f[i].v[j].m128_f32[2])
				m->rigid.min.m128_f32[2] = m->rigid.f[i].v[j].m128_f32[2];

			if (m->rigid.max.m128_f32[2] < m->rigid.f[i].v[j].m128_f32[2])
				m->rigid.max.m128_f32[2] = m->rigid.f[i].v[j].m128_f32[2];
		}
	}
}
#endif // VECTORIZED_CODE #######################################################################################
void releasemeshRigid(mesh *m) {
	free(m->rigid.v);
	free(m->rigid.f);

}


