#include "headers/components/logging.h"

void logvec4(const vec4 v) {
	printf("%f %f %f %f\n", vec4ExtractX(v), vec4ExtractY(v), vec4ExtractZ(v), vec4ExtractW(v));
}
void logmat4x4(const mat4x4 m) {
	printf("%f %f %f %f\n", vec4ExtractX(m.m[0]), vec4ExtractY(m.m[0]), vec4ExtractZ(m.m[0]), vec4ExtractW(m.m[0]));
	printf("%f %f %f %f\n", vec4ExtractX(m.m[1]), vec4ExtractY(m.m[1]), vec4ExtractZ(m.m[1]), vec4ExtractW(m.m[1]));
	printf("%f %f %f %f\n", vec4ExtractX(m.m[2]), vec4ExtractY(m.m[2]), vec4ExtractZ(m.m[2]), vec4ExtractW(m.m[2]));
	printf("%f %f %f %f\n", vec4ExtractX(m.m[3]), vec4ExtractY(m.m[3]), vec4ExtractZ(m.m[3]), vec4ExtractW(m.m[3]));
}
void loganim(const animation an) {
	printf("animation {\n");
	printf("    frames              : %d\n", an.frames);
	printf("    location            : %p\n", &an.lc);
	printf("    rotation_quaternion : %p\n", &an.rq);
	printf("    scale               : %p\n", &an.sc);
	printf("}\n");
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
	printf("    min           : ");
	logvec4(r.min);
	printf("    max           : ");
	logvec4(r.max);
	printf("    q             : ");
	logvec4(r.q);

	printf("    vbo           : %p\n", r.vbo);
	printf("    falling_time  : %f\n", r.falling_time);

	printf("    vbo_indexes   : %d\n", r.vbo_indexes);
	printf("    faces_indexes : %d\n", r.faces_indexes);
	printf("    vecs_indexes  : %d\n", r.vecs_indexes);
	printf("    vbo_size      : %d\n", r.vbo_size);
	printf("    VAO           : %d\n", r.VAO);
	printf("    VBO           : %d\n", r.VBO);
	printf("    grounded      : %d\n", r.grounded);
	printf("    faces:        : %p\n", r.f);
	printf("}\n");
}
void logmesh(const mesh m) {
	logcoords(m.coords);
	printf("Quat           : ");
	logvec4(m.q);
	printf("scale          : ");
	logvec4(m.scale);
	printf("cname          : %s\n", m.cname);
	printf("cname_length   : %d\n", m.cname_length);
	printf("vbo            : %p\n", m.vbo);
	printf("outer_radius   : %f\n", m.outer_radius);
	printf("vbo_indexes    : %d\n", m.vbo_indexes);
	printf("faces_indexes  : %d\n", m.faces_indexes);
	printf("vecs_indexes   : %d\n", m.vecs_indexes);
	printf("vbo_size       : %d\n", m.vbo_size);
	printf("VAO            : %d\n", m.VAO);
	printf("VBO            : %d\n", m.VBO);
	printf("pk             : %d\n", m.pk);
	printf("asset_type     : %d\n", m.asset_type);
	printf("mesh_type      : %d\n", m.mesh_type);
	printf("visible        : %d\n", m.visible);
	printf("number_of_children : %d\n", m.number_of_children);
	printf("children       : %p\n", &m.children);
	printf("parent         : %p\n", &m.parent);
	printf("owns_rigid     : %d\n", m.owns_rigid);
	if (m.owns_rigid)
	    logrigid(m.rigid);
	printf("owns_anim      : %d\n", m.owns_anim);
	if (m.owns_anim)
		loganim(m.anim);
}
void logmodel(const model m) {
	logcoords(m.coords);
	printf("Quat           : ");
	logvec4(m.q);
	printf("velocity       : ");
	logvec4(m.velocity);
	printf("scale          : ");
	logvec4(m.scale);
	printf("cname          : %s\n", m.cname);
	printf("cname_length   : %d\n", m.cname_length);
	printf("outer_radius   : %f\n", m.outer_radius);
	printf("rotate         : %f\n", m.rotate);
	printf("mesh           : %p\n", &m.mesh);
	printf("mesh_indexes   : %d\n", m.mesh_indexes);
	printf("cname_length   : %d\n", m.cname_length);
	printf("pk             : %d\n", m.pk);
	printf("asset_type     : %d\n", m.asset_type);
	printf("model_type     : %d\n", m.model_type);
	printf("visible        : %d\n", m.visible);
	printf("quadInit       : %d\n", m.quad_init);
	printf("quadIndex      : %d\n", m.quad_index);
	printf("quadIndex      : %d\n", m.quad_face);
	printf("owns_rigid     : %d\n", m.owns_rigid);
	if (m.owns_rigid)
		logrigid(m.rigid);
	printf("owns_anim      : %d\n", m.owns_anim);
	if (m.owns_anim)
		loganim(m.anim);
}
void logscene(const scene s) {
	printf("mesh_indexes         : %d\n", s.model_indexes);
	printf("terrain quad_indexes : %d\n", s.t.quad_indexes);
	printf("terrain vec_width    : %d\n", s.t.vec_width);
	printf("terrain vec_height   : %d\n", s.t.vec_height);
	printf("terrain quad_rows    : %d\n", s.t.quad_rows);
	printf("terrain quad_cols    : %d\n", s.t.quad_cols);
}


