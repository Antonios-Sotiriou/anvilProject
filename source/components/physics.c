#include "headers/components/physics.h"

const static vec4 gravity_epicenter = { 0.f, -1.f, 0.f, 0.f };

/* Responsible to apply all the forces that act on the rigid body and orginise the appropriate Collision detection functions steps. */
void applyPhysics(void) {

	for (int i = 0; i < SCENE.mesh_indexes; i++) {
		if (SCENE.mesh[i].rigid.state == ENABLE && (checkAllZeros(SCENE.mesh[i].rigid.velocity) || SCENE.mesh[i].rigid.rot_angle)) {

			initMeshQuadInfo(&SCENE.mesh[i]);

			//float g_accelaration = 0.f;
			//if (!SCENE.mesh[i].rigid.grounded) {
			//	SCENE.mesh[i].rigid.falling_time += DeltaTime;
			//	g_accelaration = (98.1f * (SCENE.mesh[i].rigid.falling_time * 2));
			//}
			//SCENE.mesh[i].rigid.velocity = vecAddvec(vecMulf32(gravity_epicenter, g_accelaration), SCENE.mesh[i].rigid.velocity);

			/* 1st Collision Detection lvl. */
			//if (SCENE.mesh[i].pk == camera)
			//    staticOuterRadiusCollision(&SCENE.mesh[i]);

			/* 2nd Collision Detection lvl. */
			//int collide = 1;
			//while (collide) {
			//	sortCollisions(&SCENE.mesh[i]);
			//	int colls[1] = { 3 };
			//	collide = sweptAABBCollision(&SCENE.mesh[i], colls);
			//}

			/* 3rd Collision Detection lvl. */
			if (SCENE.mesh[i].pk == camera) {
				staticOBBCollision(&SCENE.mesh[i], 3);
				//staticOBBCollision(&SCENE.mesh[i], 4);
			}

			/* 4th Collision Detection lvl. */
			if (SCENE.mesh[i].rigid.rot_angle) {
				/* At this point apply rotationColision. */
				mat4x4 tr = matfromQuat(SCENE.mesh[i].rigid.q, SCENE.mesh[i].coords.v[0]);
				//setvec4arrayMulmat(SCENE.mesh[i].coords.v, 4, tr);
				setvec4arrayMulmat(SCENE.mesh[i].rigid.v, SCENE.mesh[i].rigid.v_indexes, tr);
				setvec4arrayMulmat(SCENE.mesh[i].rigid.n, SCENE.mesh[i].rigid.n_indexes, tr);

				//setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[0]);
				setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[1]);
				setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[2]);
				setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[3]);

				SCENE.mesh[i].q = multiplyQuats(SCENE.mesh[i].q, SCENE.mesh[i].rigid.q);
				//SCENE.mesh[i].q = SCENE.mesh[i].rigid.q;
			}

			mat4x4 tm = translationMatrix(vec4ExtractX(SCENE.mesh[i].rigid.velocity), vec4ExtractY(SCENE.mesh[i].rigid.velocity), vec4ExtractZ(SCENE.mesh[i].rigid.velocity));
			setvec4arrayMulmat(SCENE.mesh[i].coords.v, 4, tm);
			setvec4arrayMulmat(SCENE.mesh[i].rigid.v, SCENE.mesh[i].rigid.v_indexes, tm);
			setvec4arrayMulmat(SCENE.mesh[i].rigid.n, SCENE.mesh[i].rigid.n_indexes, tm);

			if (!SCENE.mesh[i].rigid.grounded || SCENE.mesh[i].type != MESH_TYPE_CAMERA)
				meshTerrainCollision(&SCENE.mesh[i]);
		}
	}
}


