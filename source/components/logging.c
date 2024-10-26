#include "headers/components/logging.h"

#ifdef VECTORIZED_CODE
void logvec4(const vec4 v) {
	printf("%f %f %f %f\n", v.m128_f32[0], v.m128_f32[1], v.m128_f32[2], v.m128_f32[3]);
}
void logmat4x4(const mat4x4 m) {
	printf("%f %f %f %f\n", m.m[0].m128_f32[0], m.m[0].m128_f32[1], m.m[0].m128_f32[2], m.m[0].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[1].m128_f32[0], m.m[1].m128_f32[1], m.m[1].m128_f32[2], m.m[1].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[2].m128_f32[0], m.m[2].m128_f32[1], m.m[2].m128_f32[2], m.m[2].m128_f32[3]);
	printf("%f %f %f %f\n", m.m[3].m128_f32[0], m.m[3].m128_f32[1], m.m[3].m128_f32[2], m.m[3].m128_f32[3]);
}
#else
void logvec4(const vec4 v) {
	printf("%f %f %f %f\n", v.f32[0], v.f32[1], v.f32[2], v.f32[3]);
}
void logmat4x4(const mat4x4 m) {
	printf("%f %f %f %f\n", m.m[0].f32[0], m.m[0].f32[1], m.m[0].f32[2], m.m[0].f32[3]);
	printf("%f %f %f %f\n", m.m[1].f32[0], m.m[1].f32[1], m.m[1].f32[2], m.m[1].f32[3]);
	printf("%f %f %f %f\n", m.m[2].f32[0], m.m[2].f32[1], m.m[2].f32[2], m.m[2].f32[3]);
	printf("%f %f %f %f\n", m.m[3].f32[0], m.m[3].f32[1], m.m[3].f32[2], m.m[3].f32[3]);
}
#endif // VECTORIZED_CODE

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
void logmesh(const mesh m) {
	logcoords(m.coords);
	printf("vec4        :%p\n", m.vao);
	printf("vec4_indexes:%d\n", m.vao_indexes);
}


