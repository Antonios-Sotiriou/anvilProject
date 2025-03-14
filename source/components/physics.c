#include "headers/components/physics.h"

const static vec4 gravity_epicenter = { 0.f, -1.f, 0.f, 0.f };

/* Responsible to apply all the forces that act on the rigid body and orginise the appropriate Collision detection functions steps. */
void applyPhysics(void) {

	for (int i = 0; i < SCENE.model_indexes; i++) {
		if (SCENE.model[i].rigid.state == ENABLED && (checkAllZeros(SCENE.model[i].rigid.velocity) || SCENE.model[i].rigid.rot_angle)) {

			initModelQuadInfo(&SCENE.model[i]);
			printf("reached this spot\n");
			//float g_accelaration = 0.f;
			//if (!SCENE.model[i].rigid.grounded) {
			//	SCENE.model[i].rigid.falling_time += DeltaTime;
			//	g_accelaration = (98.1f * (SCENE.model[i].rigid.falling_time * 2));
			//}
			//SCENE.model[i].rigid.velocity = vecAddvec(vecMulf32(gravity_epicenter, g_accelaration), SCENE.model[i].rigid.velocity);

			/* 1st Collision Detection lvl. */
			//if (SCENE.model[i].pk == camera)
			//    staticOuterRadiusCollision(&SCENE.model[i]);

			/* 2nd Collision Detection lvl. */
			//int collide = 1;
			//while (collide) {
			//	sortCollisions(&SCENE.model[i]);
			//	int colls[1] = { 3 };
			//	collide = sweptAABBCollision(&SCENE.model[i], colls);
			//}

			/* 3rd Collision Detection lvl. */
			if (SCENE.model[i].pk == camera) {
				staticOBBCollision(&SCENE.model[i], 3);
				//staticOBBCollision(&SCENE.model[i], 4);
			}

			/* 4th Collision Detection lvl. */
			if (SCENE.model[i].rigid.rot_angle) {
				/* At this point apply rotationColision. */
				mat4x4 tr = matfromQuat(SCENE.model[i].rigid.q, SCENE.model[i].coords.v[0]);
				//setvec4arrayMulmat(SCENE.model[i].coords.v, 4, tr);
				setvec4arrayMulmat(SCENE.model[i].rigid.v, SCENE.model[i].rigid.v_indexes, tr);
				setvec4arrayMulmat(SCENE.model[i].rigid.n, SCENE.model[i].rigid.n_indexes, tr);

				//setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[0]);
				setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[1]);
				setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[2]);
				setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[3]);

				SCENE.model[i].q = multiplyQuats(SCENE.model[i].q, SCENE.model[i].rigid.q);
				//SCENE.model[i].q = SCENE.model[i].rigid.q;
			}

			mat4x4 tm = translationMatrix(vec4ExtractX(SCENE.model[i].rigid.velocity), vec4ExtractY(SCENE.model[i].rigid.velocity), vec4ExtractZ(SCENE.model[i].rigid.velocity));
			setvec4arrayMulmat(SCENE.model[i].coords.v, 4, tm);
			setvec4arrayMulmat(SCENE.model[i].rigid.v, SCENE.model[i].rigid.v_indexes, tm);
			setvec4arrayMulmat(SCENE.model[i].rigid.n, SCENE.model[i].rigid.n_indexes, tm);

			if (!SCENE.model[i].rigid.grounded || SCENE.model[i].type != MODEL_TYPE_CAMERA)
				modelTerrainCollision(&SCENE.model[i]);
		}
	}
}


