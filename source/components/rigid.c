#include "headers/components/rigid.h"

void loadRigid(mesh *m, const char path[]) {
	OBJ obj;
	readOBJ(&obj, path);

	m->rigid.v_indexes = (obj.v_indexes / 3);
	m->rigid.v = malloc(m->rigid.v_indexes * 16);
	int v_index = 0;
	for (int i = 0; i < obj.v_indexes; i += 3) {

		memcpy(&m->rigid.v[v_index], &obj.v[i], 12);
		m->rigid.v[v_index].m128_f32[3] = 1.f;
		v_index++;
	}

	m->rigid.f_indexes = (obj.f_indexes / 9);
	m->rigid.f = malloc(m->rigid.f_indexes * sizeof(face));
	int f_index = 0, fpad = 0, npad, tpad;
	for (int i = 0; i < obj.f_indexes; i += 9) {
		for (int j = 0; j < 3; j++) {

			memcpy(&m->rigid.f[f_index].v[j], &m->rigid.v[obj.f[fpad]], 16);

			tpad = obj.f[fpad + 1] * 2;
			memcpy(&m->rigid.f[f_index].vt[j], &obj.t[tpad], 8);

			npad = obj.f[fpad + 2] * 3;
			memcpy(&m->rigid.f[f_index].vn[j], &obj.n[npad], 12);
			m->rigid.f[f_index].v[j].m128_f32[3] = 0.f;

			fpad += 3;
		}
		f_index++;
	}

	releaseOBJ(&obj);
}
void releaseRigid(mesh *m) {
	free(m->rigid.v);
	free(m->rigid.f);

}


