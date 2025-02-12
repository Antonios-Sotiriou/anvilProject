#include "headers/components/quaternions.h"

#ifdef VECTORIZED_CODE // #######################################################################################
/* Usefull Global variables fro used in this file, to increase some calculations performance. */
// Useful for eulerToQuat;
const static quat etqor = { 0.f, -0.f, 0.f, -0.f };
// Useful for multiplyQuats;
const static quat mqor1 = { -0.f, 0.f, -0.f, 0.f };
const static quat mqor2 = { -0.f, 0.f, 0.f, -0.f };
const static quat mqor3 = { -0.f, -0.f, 0.f, 0.f };

/* Useful for matfromQuat */
const static vec4 twos = { 2.f, 2.f, 2.f, 0.f };

const static vec4 ones1 = { 1.f, 0.f, 0.f, 0.f };
const static vec4 mfqor1 = { 0.f, -0.f, 0.f, 0.f };

const static vec4 ones2 = { 0.f, 1.f, 0.f, 0.f };
const static vec4 mfqor2 = { 0.f, 0.f, -0.f, 0.f };

const static vec4 ones3 = { 0.f, 0.f, 1.f, 0.f };
const static vec4 mfqor3 = { -0.f, 0.f, 0.f, 0.f };

// Useful for slerp;
const static quat slhalf = { 0.5f, 0.5f, 0.5f, 0.5f };

/* Creates an initialized unit quaternion. */
quat unitQuat(void) {
    return _mm_set_ss(1.f);
}
/* Sets a quat's parameters, creating a new quat. */
quat setQuat(const float angle, const float x, const float y, const float z) {
    return _mm_setr_ps(angle, x, y, z);
}
/* Computes the magnitude aka length of a given quat. */
float magnitudeQuat(const quat q) {
    quat r = _mm_mul_ps(q, q);
    return _mm_cvtss_f32(
        _mm_sqrt_ps(
            _mm_add_ps(
                _mm_add_ps(r, _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 1))),
                _mm_add_ps(_mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 2)), _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 3)))
            )
        )
    );
}
/* Normalizes given quat if its not already. */
void normalizeQuat(quat* q) {
    vec4 r = _mm_mul_ps(*q, *q);
    const float check = _mm_cvtss_f32(
        _mm_add_ps(
            _mm_add_ps(r, _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 1))),
            _mm_add_ps(_mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 2)), _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 3)))
        )
    );
    if (check > 1.000001f) {
        *q = _mm_div_ps(*q, _mm_sqrt_ps(_mm_set_ps1(check)));
    }
}
/* Conjugate quat's vector part, aka( switching the sign ). */
quat conjugateQuat(const quat q) {
    quat r = _mm_setr_ps(0.f, -0.f, -0.f, -0.f);
    return _mm_xor_ps(q, r);
}
/* Creates a rotation quaternion with angle W and rotation axis X Y Z: */
quat rotationQuat(const float angle, const float x, const float y, const float z) {
    const float radius = (angle * (3.14159f / 180.0f)) * 0.5f;
    const float sn = sinf(radius);
    const float cs = cosf(radius);
    return _mm_mul_ps(_mm_setr_ps(cs, x, y, z), _mm_setr_ps(1.f, sn, sn, sn));
}
/* Rotates vector v by the given quaternion.Returns a new vector. */
vec4 vec4RotateQuat(const quat q, const vec4 v) {
    quat r = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3));
    r = multiplyQuats(multiplyQuats(conjugateQuat(q), r), q);
    return _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 3, 2, 1));
}
/* Rotates vector v by the given quaternion. */
void setvec4RotateQuat(const quat q, vec4 *v) {
    quat r = _mm_shuffle_ps(*v, *v, _MM_SHUFFLE(2, 1, 0, 3));
    r = multiplyQuats(multiplyQuats(conjugateQuat(q), r), q);
    *v = _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 3, 2, 1));
}
/* Adds quats q1 and q2 together returning a new quat. */
quat addQuats(const quat q1, const quat q2) {
    return _mm_add_ps(q1, q2);
}
/* Creates a quaternion from the given euler angles. */
quat eulertoQuat(const float roll, const float yaw, const float pitch) {
    const float half_roll = roll * 0.5f;
    const float half_yaw = yaw * 0.5f;
    const float half_pitch = pitch * 0.5f;

    const float cy = cosf(half_roll);
    const float sy = sinf(half_roll);
    const float cp = cosf(half_yaw);
    const float sp = sinf(half_yaw);
    const float cr = cosf(half_pitch);
    const float sr = sinf(half_pitch);

    quat crsrcrcr = _mm_setr_ps(cr, sr, cr, cr);
    quat cpcpspcp = _mm_setr_ps(cp, cp, sp, cp);
    quat cycycysy = _mm_setr_ps(cy, cy, cy, sy);

    quat left = _mm_mul_ps(_mm_mul_ps(crsrcrcr, cpcpspcp), cycycysy);
    quat right = _mm_mul_ps(
        _mm_mul_ps(_mm_shuffle_ps(crsrcrcr, crsrcrcr, _MM_SHUFFLE(1, 1, 0, 1)), _mm_shuffle_ps(cpcpspcp, cpcpspcp, _MM_SHUFFLE(2, 0, 2, 2))),
        _mm_shuffle_ps(cycycysy, cycycysy, _MM_SHUFFLE(0, 3, 3, 3))
    );

    return _mm_add_ps(left, _mm_xor_ps(etqor, right));
}
/* Multiplies two quats(q1, q2) with each other returning a new quat. */
quat multiplyQuats(const quat q1, const quat q2) {
    quat w = _mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(0, 0, 0, 0)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(3, 2, 1, 0)));
    quat x = _mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(1, 1, 1, 1)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(2, 3, 0, 1)));
    quat y = _mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(2, 2, 2, 2)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(1, 0, 3, 2)));
    quat z = _mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(3, 3, 3, 3)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 2, 3)));

    return _mm_add_ps(_mm_add_ps(_mm_add_ps(w, _mm_xor_ps(mqor1, x)), _mm_xor_ps(mqor2, y)), _mm_xor_ps(mqor3, z));
}
/* Creates a matrix from a given quaternion with translation x, y, z. */
mat4x4 matfromQuat(const quat q, const vec4 t) {
    mat4x4 m;
    vec4 r1 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 1, 1, 0)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 2, 0)));
    vec4 r2 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 0, 1)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 2, 3, 1)));
    m.m[0] = _mm_sub_ps(_mm_mul_ps(_mm_add_ps(r1, _mm_xor_ps(mfqor1, r2)), twos), ones1);

    r1 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 2, 0, 1)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 0, 2)));
    r2 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 1, 2, 3)));
    m.m[1] = _mm_sub_ps(_mm_mul_ps(_mm_add_ps(r1, _mm_xor_ps(mfqor2, r2)), twos), ones2);

    r1 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 2, 1)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 3, 3)));
    r2 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 0, 0)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 1, 2)));
    m.m[2] = _mm_sub_ps(_mm_mul_ps(_mm_add_ps(r1, _mm_xor_ps(mfqor3, r2)), twos), ones3);

    vec4 x1 = _mm_mul_ps(_mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 0, 0)), m.m[0]);
    vec4 y1 = _mm_mul_ps(_mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 1, 1, 1)), m.m[1]);
    vec4 z1 = _mm_mul_ps(_mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 2, 2)), m.m[2]);
    m.m[3] = _mm_sub_ps(_mm_sub_ps(_mm_sub_ps(t, x1), y1), z1);

    return m;
}
/* Creates a model matrix from a given quaternion (q), s scale value (s) and a translation vector (t). */
mat4x4 modelMatfromQST(const quat q, const float s, const vec4 t) {
    mat4x4 m;
    vec4 scale = _mm_set_ps1(s);
    vec4 r1 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 1, 1, 0)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 2, 0)));
    vec4 r2 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 0, 1)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 2, 3, 1)));
    m.m[0] = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_add_ps(r1, _mm_xor_ps(mfqor1, r2)), twos), ones1), scale);

    r1 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 2, 0, 1)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 0, 2)));
    r2 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 1, 2, 3)));
    m.m[1] = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_add_ps(r1, _mm_xor_ps(mfqor2, r2)), twos), ones2), scale);

    r1 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 2, 1)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 0, 3, 3)));
    r2 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 0, 0)), _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 1, 2)));
    m.m[2] = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_add_ps(r1, _mm_xor_ps(mfqor3, r2)), twos), ones3), scale);

    m.m[3] = t;

    return m;
}
/* Spherical interpolates between two quaternions at coefficient t. */
quat slerp(const quat q1, const quat q2, const float t) {
    // Calculate angle between q1 and q2.(Dot Product).
    quat r = _mm_mul_ps(q1, q2);
    const float cosHalfTheta = _mm_cvtss_f32(
        _mm_add_ps(
            _mm_add_ps(r, _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 1))),
            _mm_add_ps(_mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 2)), _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 0, 0, 3)))
        )
    );

    // if q1 = q2 or q1 = -q2 then theta = 0 and we can return q1;
    if (fabs(cosHalfTheta) >= 1.f)
        return q1;

    // Calculate temporary values.
    const float sinHalfTheta = sqrtf(1.f - (cosHalfTheta * cosHalfTheta));
    // if theta = 180 degrees then result is not fully defined.We could rotate arround any axis normal to q1 or q2.
    if (fabs(sinHalfTheta) < 0.001f) {
        return _mm_add_ps(_mm_mul_ps(q1, slhalf), _mm_mul_ps(q2, slhalf));
    }

    const float halfTheta = acosf(cosHalfTheta);
    const quat ratioA = _mm_set_ps1(sinf((1.f - t) * halfTheta) / sinHalfTheta);
    const quat ratioB = _mm_set_ps1(sinf(t * halfTheta) / sinHalfTheta);

    return _mm_add_ps(_mm_mul_ps(q1, ratioA), _mm_mul_ps(q2, ratioB));
}
/* Linearly interpolates between two quaternions at coefficient t. */
quat lerp(const quat q1, const quat q2, const float t) {
    const quat scale = _mm_set_ps1(1.f - t);
    const quat tval = _mm_set_ps1(t);
    return _mm_add_ps(_mm_mul_ps(q1, scale), _mm_mul_ps(q2, tval));
}
#else // ITERATIVE_CODE #########################################################################################
/* Creates an initialized unit quaternion. */
quat unitQuat(void) {
    return (quat) { 1.f, 0.f, 0.f, 0.f };
}
/* Sets a quat's parameters, creating a new quat. */
quat setQuat(const float angle, const float x, const float y, const float z) {
    return (quat) { angle, x, y, z };
}
/* Computes the magnitude aka length of a given quat. */
float magnitudeQuat(const quat q) {
    quat r = { q.m128_f32[0] * q.m128_f32[0], q.m128_f32[1] * q.m128_f32[1], q.m128_f32[2] * q.m128_f32[2], q.m128_f32[3] * q.m128_f32[3] };
    return sqrtf(r.m128_f32[0] + r.m128_f32[1] + r.m128_f32[2] + r.m128_f32[3]);
}
/* Normalizes given quat if its not already. */
void normalizeQuat(quat* q) {
    float check = (q->m128_f32[0] * q->m128_f32[0]) + (q->m128_f32[1] * q->m128_f32[1]) + (q->m128_f32[2] * q->m128_f32[2]) + (q->m128_f32[3] * q->m128_f32[3]);
    if (check > 1.000001f) {
        float magnitude = sqrtf(check);
        q->m128_f32[0] /= magnitude;
        q->m128_f32[1] /= magnitude;
        q->m128_f32[2] /= magnitude;
        q->m128_f32[3] /= magnitude;
    }
}
/* Conjugate quat's vector part, aka( negating the sign ). */
quat conjugateQuat(const quat q) {
    return (quat) {
        q.m128_f32[0],
        -q.m128_f32[1],
        -q.m128_f32[2],
        -q.m128_f32[3]
    };
}
/* Creates a rotation quaternion with angle W and rotation axis X Y Z: */
quat rotationQuat(const float angle, const float x, const float y, const float z) {
    const float radius = (angle * (3.14159f / 180.0f)) * 0.5f;
    const float sn = sinf(radius);
    return (quat) {
        cosf(radius),
        x * sn,
        y * sn,
        z * sn
    };
}
/* Rotates vector v by the given quaternion.Returns a new vector. */
vec4 vec4RotateQuat(const quat q, const vec4 v) {
    quat r = setQuat(0.f, v.m128_f32[0], v.m128_f32[1], v.m128_f32[2]);
    r = multiplyQuats(multiplyQuats(conjugateQuat(q), r), q);
    return (vec4) { r.m128_f32[1], r.m128_f32[2], r.m128_f32[3], v.m128_f32[3] };
}
/* Rotates vector v by the given quaternion. */
void setvec4RotateQuat(const quat q, vec4* v) {
    quat r = setQuat(0.f, v->m128_f32[0], v->m128_f32[1], v->m128_f32[2]);
    r = multiplyQuats(multiplyQuats(conjugateQuat(q), r), q);
    v->m128_f32[0] = r.m128_f32[1];
    v->m128_f32[1] = r.m128_f32[2];
    v->m128_f32[2] = r.m128_f32[3];
}
/* Adds quats q1 and q2 together returning a new quat. */
quat addQuats(const quat q1, const quat q2) {
    return (quat) {
        q1.m128_f32[0] + q2.m128_f32[0],
        q1.m128_f32[1] + q2.m128_f32[1],
        q1.m128_f32[2] + q2.m128_f32[2],
        q1.m128_f32[3] + q2.m128_f32[3]
    };
}
/* Creates a quaternion from the given euler angles. */
quat eulertoQuat(const float roll, const float yaw, const float pitch) {
    const float half_roll = roll * 0.5f;
    const float half_yaw = yaw * 0.5f;
    const float half_pitch = pitch * 0.5f;

    const float cy = cosf(half_roll);
    const float sy = sinf(half_roll);
    const float cp = cosf(half_yaw);
    const float sp = sinf(half_yaw);
    const float cr = cosf(half_pitch);
    const float sr = sinf(half_pitch);
    const float cpcy = cp * cy;
    const float spsy = sp * sy;
    const float spcy = sp * cy;
    const float cpsy = cp * sy;

    return (quat) {
        (cr * cpcy) + (sr * spsy),
        (sr * cpcy) - (cr * spsy),
        (cr * spcy) + (sr * cpsy),
        (cr * cpsy) - (sr * spcy),
    };
}
/* Multiplies two quats(q1, q2) with each other returning a new quat. */
quat multiplyQuats(const quat q1, const quat q2) {
    return (quat) {
        (q1.m128_f32[0] * q2.m128_f32[0]) - (q1.m128_f32[1] * q2.m128_f32[1]) - (q1.m128_f32[2] * q2.m128_f32[2]) - (q1.m128_f32[3] * q2.m128_f32[3]),
        (q1.m128_f32[0] * q2.m128_f32[1]) + (q1.m128_f32[1] * q2.m128_f32[0]) + (q1.m128_f32[2] * q2.m128_f32[3]) - (q1.m128_f32[3] * q2.m128_f32[2]),
        (q1.m128_f32[0] * q2.m128_f32[2]) - (q1.m128_f32[1] * q2.m128_f32[3]) + (q1.m128_f32[2] * q2.m128_f32[0]) + (q1.m128_f32[3] * q2.m128_f32[1]),
        (q1.m128_f32[0] * q2.m128_f32[3]) + (q1.m128_f32[1] * q2.m128_f32[2]) - (q1.m128_f32[2] * q2.m128_f32[1]) + (q1.m128_f32[3] * q2.m128_f32[0])
    };
}
/* Creates a matrix from a given quaternion with translation vector t. */
mat4x4 matfromQuat(const quat q, const vec4 t) {
    mat4x4 m;
    m.m[0].m128_f32[0] = (2.0f * ((q.m128_f32[0] * q.m128_f32[0]) + (q.m128_f32[1] * q.m128_f32[1])) - 1.0f);
    m.m[0].m128_f32[1] = (2.0f * ((q.m128_f32[1] * q.m128_f32[2]) - (q.m128_f32[0] * q.m128_f32[3])));
    m.m[0].m128_f32[2] = (2.0f * ((q.m128_f32[1] * q.m128_f32[3]) + (q.m128_f32[0] * q.m128_f32[2])));
    m.m[0].m128_f32[3] = 0.0f;

    m.m[1].m128_f32[0] = (2.0f * ((q.m128_f32[1] * q.m128_f32[2]) + (q.m128_f32[0] * q.m128_f32[3])));
    m.m[1].m128_f32[1] = (2.0f * ((q.m128_f32[0] * q.m128_f32[0]) + (q.m128_f32[2] * q.m128_f32[2])) - 1.0);
    m.m[1].m128_f32[2] = (2.0f * ((q.m128_f32[2] * q.m128_f32[3]) - (q.m128_f32[0] * q.m128_f32[1])));
    m.m[1].m128_f32[3] = 0.0f;

    m.m[2].m128_f32[0] = (2.0f * ((q.m128_f32[1] * q.m128_f32[3]) - (q.m128_f32[0] * q.m128_f32[2])));
    m.m[2].m128_f32[1] = (2.0f * ((q.m128_f32[2] * q.m128_f32[3]) + (q.m128_f32[0] * q.m128_f32[1])));
    m.m[2].m128_f32[2] = (2.0f * ((q.m128_f32[0] * q.m128_f32[0]) + (q.m128_f32[3] * q.m128_f32[3])) - 1.0);
    m.m[2].m128_f32[3] = 0.0f;

    m.m[3].m128_f32[0] = t.m128_f32[0] - t.m128_f32[0] * m.m[0].m128_f32[0] - t.m128_f32[1] * m.m[1].m128_f32[0] - t.m128_f32[2] * m.m[2].m128_f32[0];
    m.m[3].m128_f32[1] = t.m128_f32[1] - t.m128_f32[0] * m.m[0].m128_f32[1] - t.m128_f32[1] * m.m[1].m128_f32[1] - t.m128_f32[2] * m.m[2].m128_f32[1];
    m.m[3].m128_f32[2] = t.m128_f32[2] - t.m128_f32[0] * m.m[0].m128_f32[2] - t.m128_f32[1] * m.m[1].m128_f32[2] - t.m128_f32[2] * m.m[2].m128_f32[2];
    m.m[3].m128_f32[3] = 1.0f;

    return m;
}
/* Creates a model matrix from a given quaternion (q), s scale value (s) and a translation vector (t). */
mat4x4 modelMatfromQST(const quat q, const float s, const vec4 t) {
    mat4x4 m;
    m.m[0].m128_f32[0] = (2.0f * ((q.m128_f32[0] * q.m128_f32[0]) + (q.m128_f32[1] * q.m128_f32[1])) - 1.0f) * s;
    m.m[0].m128_f32[1] = (2.0f * ((q.m128_f32[1] * q.m128_f32[2]) - (q.m128_f32[0] * q.m128_f32[3]))) * s;
    m.m[0].m128_f32[2] = (2.0f * ((q.m128_f32[1] * q.m128_f32[3]) + (q.m128_f32[0] * q.m128_f32[2]))) * s;
    m.m[0].m128_f32[3] = 0.0f;

    m.m[1].m128_f32[0] = (2.0f * ((q.m128_f32[1] * q.m128_f32[2]) + (q.m128_f32[0] * q.m128_f32[3]))) * s;
    m.m[1].m128_f32[1] = (2.0f * ((q.m128_f32[0] * q.m128_f32[0]) + (q.m128_f32[2] * q.m128_f32[2])) - 1.0) * s;
    m.m[1].m128_f32[2] = (2.0f * ((q.m128_f32[2] * q.m128_f32[3]) - (q.m128_f32[0] * q.m128_f32[1]))) * s;
    m.m[1].m128_f32[3] = 0.0f;

    m.m[2].m128_f32[0] = (2.0f * ((q.m128_f32[1] * q.m128_f32[3]) - (q.m128_f32[0] * q.m128_f32[2]))) * s;
    m.m[2].m128_f32[1] = (2.0f * ((q.m128_f32[2] * q.m128_f32[3]) + (q.m128_f32[0] * q.m128_f32[1]))) * s;
    m.m[2].m128_f32[2] = (2.0f * ((q.m128_f32[0] * q.m128_f32[0]) + (q.m128_f32[3] * q.m128_f32[3])) - 1.0) * s;
    m.m[2].m128_f32[3] = 0.0f;

    m.m[3].m128_f32[0] = t.m128_f32[0];
    m.m[3].m128_f32[1] = t.m128_f32[1];
    m.m[3].m128_f32[2] = t.m128_f32[2];
    m.m[3].m128_f32[3] = 1.0f;

    return m;
}
/* Creates a quaternion from a rotation Matrix. Scale is not taken in account. */
quat quatFromMat(mat4x4 m) {
    float trace = m.m[0].m128_f32[0] + m.m[1].m128_f32[1] + m.m[2].m128_f32[2];
    if (trace > 0) {
        float s = 0.5f / sqrtf(trace + 1.f);
        return (quat) {
            0.25f / s,
            (m.m[2].m128_f32[1] - m.m[1].m128_f32[2]) * s,
            (m.m[0].m128_f32[2] - m.m[2].m128_f32[0]) * s,
            (m.m[1].m128_f32[0] - m.m[0].m128_f32[1]) * s
        };
    } else {
        if ((m.m[0].m128_f32[0] > m.m[1].m128_f32[1]) && (m.m[0].m128_f32[0]) > m.m[2].m128_f32[2]) {
            float s = 2.f * sqrtf(1.f + m.m[0].m128_f32[0] - m.m[1].m128_f32[1] - m.m[2].m128_f32[2]);
            return (quat) {
                (m.m[2].m128_f32[1] - m.m[1].m128_f32[2]) / s,
                0.25f * s,
                (m.m[0].m128_f32[1] + m.m[1].m128_f32[0]) / s,
                (m.m[0].m128_f32[2] + m.m[2].m128_f32[0]) / s
            };
        } else if (m.m[1].m128_f32[1] > m.m[2].m128_f32[2]) {
            float s = 2.f * sqrtf(1.f + m.m[1].m128_f32[1] - m.m[0].m128_f32[0] - m.m[2].m128_f32[2]);
            return (quat) {
                (m.m[0].m128_f32[2] - m.m[2].m128_f32[0]) / s,
                (m.m[0].m128_f32[1] + m.m[1].m128_f32[0]) / s,
                0.25f * s,
                (m.m[1].m128_f32[2] + m.m[2].m128_f32[1]) / s
            };
        } else {
            float s = 2.f * sqrtf(1.f + m.m[2].m128_f32[2] - m.m[0].m128_f32[0] - m.m[1].m128_f32[1]);
            return (quat) {
                (m.m[1].m128_f32[0] - m.m[0].m128_f32[1]) / s,
                (m.m[0].m128_f32[2] + m.m[2].m128_f32[0]) / s,
                (m.m[1].m128_f32[2] + m.m[2].m128_f32[1]) / s,
                0.25f * s
            };
        }
    }
}
/* Spherical interpolates between two quaternions at coefficient t. */
quat slerp(const quat q1, const quat q2, const float t) {
    // Calculate angle between q1 and q2.
    const float cosHalfTheta = (q1.m128_f32[0] * q2.m128_f32[0]) + (q1.m128_f32[1] * q2.m128_f32[1]) + (q1.m128_f32[2] * q2.m128_f32[2]) + (q1.m128_f32[3] * q2.m128_f32[3]);
    // if q1 = q2 or q1 = -q2 then theta = 0 and we can return q1;
    if (fabs(cosHalfTheta) >= 1.f)
        return q1;

    // Calculate temporary values.
    const float sinHalfTheta = sqrtf(1.f - (cosHalfTheta * cosHalfTheta));
    // if theta = 180 degrees then result is not fully defined.We could rotate arround any axis normal to q1 or q2.
    if (fabs(sinHalfTheta) < 0.001f) {
        return (quat) {
            (q1.m128_f32[0] * 0.5f) + (q2.m128_f32[0] * 0.5f),
            (q1.m128_f32[1] * 0.5f) + (q2.m128_f32[1] * 0.5f),
            (q1.m128_f32[2] * 0.5f) + (q2.m128_f32[2] * 0.5f),
            (q1.m128_f32[3] * 0.5f) + (q2.m128_f32[3] * 0.5f)
        };
    }

    const float halfTheta = acosf(cosHalfTheta);
    const float ratioA = sinf((1.f - t) * halfTheta) / sinHalfTheta;
    const float ratioB = sinf(t * halfTheta) / sinHalfTheta;
    // Calculate the quaternion.
    return (quat) {
        (q1.m128_f32[0] * ratioA) + (q2.m128_f32[0] * ratioB),
        (q1.m128_f32[1] * ratioA) + (q2.m128_f32[1] * ratioB),
        (q1.m128_f32[2] * ratioA) + (q2.m128_f32[2] * ratioB),
        (q1.m128_f32[3] * ratioA) + (q2.m128_f32[3] * ratioB)
    };
}
/* Linearly interpolates between two quaternions at coefficient t. */
quat lerp(const quat q1, const quat q2, const float t) {
    const float scale = 1.f - t;
    return (quat) {
        (q1.m128_f32[0] * scale) + (q2.m128_f32[0] * t),
        (q1.m128_f32[1] * scale) + (q2.m128_f32[1] * t),
        (q1.m128_f32[2] * scale) + (q2.m128_f32[2] * t),
        (q1.m128_f32[3] * scale) + (q2.m128_f32[3] * t)
    };
}
#endif // VECTORIZED_CODE #######################################################################################


