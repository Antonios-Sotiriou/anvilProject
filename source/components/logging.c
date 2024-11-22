#include "headers/components/logging.h"

void logvec4(const vec4 v) {
	printf("%f %f %f %f\n", v.m128_f32[0], v.m128_f32[1], v.m128_f32[2], v.m128_f32[3]);
}
void logmat4x4(const mat4x4 m) {
	printf("%f %f %f %f\n", m.m[0].m128_f32[0], m.m[0].m128_f32[1], m.m[0].m128_f32[2], m.m[0].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[1].m128_f32[0], m.m[1].m128_f32[1], m.m[1].m128_f32[2], m.m[1].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[2].m128_f32[0], m.m[2].m128_f32[1], m.m[2].m128_f32[2], m.m[2].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[3].m128_f32[0], m.m[3].m128_f32[1], m.m[3].m128_f32[2], m.m[3].m128_f32[3]);
}
void logcoords(const coords c) {
	printf("Coords {\n");
	printf("    Pos: ");
	logvec4(c.v[0]);
	printf("    U  : ");
	logvec4(c.v[1]);
	printf("    V  : ");
	logvec4(c.v[2]);
	printf("    N  : ");
	logvec4(c.v[3]);
	printf("}\n");
}
void logrigid(const rigid r) {
	printf("rigid {\n");
	printf("    *v         : %p\n", r.v);
	printf("    *f         : %p\n", r.f);
	printf("    v_indexes  : %d\n", r.v_indexes);
	printf("    f_indexes  : %d\n", r.f_indexes);
	printf("    state      : %d\n", r.state);
	printf("    min        : ");
	logvec4(r.min);
	printf("    max        : ");
	logvec4(r.max);
	printf("    velocity   : ");
	logvec4(r.velocity);
	printf("    rot_angle  : %f\n", r.rot_angle);
	printf("    q          : ");
	logvec4(r.q);
	printf("}\n");
}
void logmesh(const mesh m) {
	logcoords(m.coords);
	printf("Quat           : ");
	logvec4(m.q);
	printf("vbo            : %p\n", m.vbo);
	printf("scale          : %f\n", m.scale);
	printf("outer_radius   : %f\n", m.outer_radius);
	printf("vbo_indexes    : %d\n", m.vbo_indexes);
	printf("faces_indexes  : %d\n", m.faces_indexes);
	printf("vecs_indexes   : %d\n", m.vecs_indexes);
	printf("vbo_size       : %d\n", m.vbo_size);
	printf("VAO            : %d\n", m.VAO);
	printf("VBO            : %d\n", m.VBO);
	printf("pk             : %d\n", m.pk);
	printf("type           : %d\n", m.type);
	printf("quadInit       : %d\n", m.quadInit);
	printf("quadIndex      : %d\n", m.quadIndex);
	logrigid(m.rigid);
}


