#include "headers/components/logging.h"

#ifdef VECTORIZED_CODE // #######################################################################################
void logvec4(const vec4 v) {
	printf("%f %f %f %f\n", vec4ExtractX(v), vec4ExtractY(v), vec4ExtractZ(v), vec4ExtractW(v));
}
void logmat4x4(const mat4x4 m) {
	printf("%f %f %f %f\n", vec4ExtractX(m.m[0]), vec4ExtractY(m.m[0]), vec4ExtractZ(m.m[0]), vec4ExtractW(m.m[0]));
	printf("%f %f %f %f\n", vec4ExtractX(m.m[1]), vec4ExtractY(m.m[1]), vec4ExtractZ(m.m[1]), vec4ExtractW(m.m[1]));
	printf("%f %f %f %f\n", vec4ExtractX(m.m[2]), vec4ExtractY(m.m[2]), vec4ExtractZ(m.m[2]), vec4ExtractW(m.m[2]));
	printf("%f %f %f %f\n", vec4ExtractX(m.m[3]), vec4ExtractY(m.m[3]), vec4ExtractZ(m.m[3]), vec4ExtractW(m.m[3]));
}
#else // ITERATIVE_CODE #########################################################################################
void logvec4(const vec4 v) {
	printf("%f %f %f %f\n", v.m128_f32[0], v.m128_f32[1], v.m128_f32[2], v.m128_f32[3]);
}
void logmat4x4(const mat4x4 m) {
	printf("%f %f %f %f\n", m.m[0].m128_f32[0], m.m[0].m128_f32[1], m.m[0].m128_f32[2], m.m[0].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[1].m128_f32[0], m.m[1].m128_f32[1], m.m[1].m128_f32[2], m.m[1].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[2].m128_f32[0], m.m[2].m128_f32[1], m.m[2].m128_f32[2], m.m[2].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[3].m128_f32[0], m.m[3].m128_f32[1], m.m[3].m128_f32[2], m.m[3].m128_f32[3]);
}
#endif // VECTORIZED_CODE #######################################################################################
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
	printf("    *v           : %p\n", r.v);
	printf("    *f           : %p\n", r.f);
	printf("    v_indexes    : %d\n", r.v_indexes);
	printf("    f_indexes    : %d\n", r.f_indexes);
	printf("    state        : %d\n", r.state);
	printf("    grounded     : %d\n", r.grounded);
	printf("    min          : ");
	logvec4(r.min);
	printf("    max          : ");
	logvec4(r.max);
	printf("    velocity     : ");
	logvec4(r.velocity);
	printf("    rot_angle    : %f\n", r.rot_angle);
	printf("    falling_time : %f\n", r.falling_time);
	printf("    q            : ");
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
	printf("quadInit       : %d\n", m.quad_init);
	printf("quadIndex      : %d\n", m.quad_index);
	printf("quadIndex      : %d\n", m.quad_face);
	logrigid(m.rigid);
}
void logscene(const scene s) {
	printf("mesh_indexes         : %d\n", s.mesh_indexes);
	printf("terrain quad_indexes : %d\n", s.t.quad_indexes);
	printf("terrain vec_width    : %d\n", s.t.vec_width);
	printf("terrain vec_height   : %d\n", s.t.vec_height);
	printf("terrain quad_rows    : %d\n", s.t.quad_rows);
	printf("terrain quad_cols    : %d\n", s.t.quad_cols);
}


