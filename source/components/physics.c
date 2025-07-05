#include "headers/components/physics.h"

 const static vec4 gravity_epicenter = { 0.f, -1.f, 0.f, 0.f };

/* Responsible to apply all the forces that act on the rigid body and orginise the appropriate Collision detection functions steps. */
void applyPhysics(void) {

	for (int i = 0; i < SCENE.model_indexes; i++) {

		if (SCENE.model[i].owns_rigid == ENABLED) {

			if ((SCENE.model[i].model_type != MODEL_TYPE_TERRAIN && !SCENE.model[i].rigid.grounded) || (checkAllZeros(SCENE.model[i].velocity) || SCENE.model[i].rotate)) {

				initModelQuadInfo(&SCENE.model[i]);

				float g_accelaration = 0.f;
				if (!SCENE.model[i].rigid.grounded) {
					SCENE.model[i].rigid.falling_time += deltaTime;
					g_accelaration = 9.81f * (SCENE.model[i].rigid.falling_time * SCENE.model[i].rigid.falling_time);
				}
				SCENE.model[i].velocity = vecAddvec(vecMulf32(gravity_epicenter, g_accelaration), SCENE.model[i].velocity);

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
				//if (SCENE.model[i].pk == camera) {
				//	staticOBBCollision(&SCENE.model[i], 3);
				//}

				/* 4th Collision Detection lvl. */
				/* At this point apply rotationColision. */
			}

			if (SCENE.model[i].rotate) {
				//setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[0]);
				setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[1]);
				setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[2]);
				setvec4RotateQuat(SCENE.model[i].rigid.q, &SCENE.model[i].coords.v[3]);

				SCENE.model[i].q = multiplyQuats(SCENE.model[i].q, SCENE.model[i].rigid.q);
			}

			mat4x4 tm = translationMatrix(vec4ExtractX(SCENE.model[i].velocity), vec4ExtractY(SCENE.model[i].velocity), vec4ExtractZ(SCENE.model[i].velocity));
			setvec4ArrayMulmat(SCENE.model[i].coords.v, 4, tm);
			setfacesArrayMulMat(SCENE.model[i].rigid.f, SCENE.model[i].rigid.faces_indexes, tm);

			if (SCENE.model[i].model_type != MODEL_TYPE_TERRAIN) {
				modelTerrainCollision(&SCENE.model[i]);
				getRigidLimits(&SCENE.model[i].rigid);    // Probably should be moves to another more appropriate place.
			}
		}
	}
}


