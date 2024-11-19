#include "headers/components/physics.h"

/* Responsible to apply all the forces that act on the rigid body and orginise the appropriate Collision detection functions steps. */
void applyPhysics(void) {

	for (int i = 0; i < SCENE.mesh_indexes; i++) {
		if (SCENE.mesh[i].rigid.state == ENABLE && (checkAllZeros(SCENE.mesh[i].rigid.velocity) || SCENE.mesh[i].rigid.rot_angle)) {

			initMeshQuadInfo(&SCENE.mesh[i]);

			/* 1st Collision Detection lvl. */
			//if (SCENE.mesh[i].pk == camera)
			//    outerRadiusCollision(&SCENE.mesh[i]);

			/* 3rd Collision Detection lvl. */
			if (SCENE.mesh[i].pk == camera)
			    checkOBBCollision(&SCENE.mesh[i]);

			/* 4th Collision Detection lvl. */
			if (SCENE.mesh[i].rigid.rot_angle) {
				/* At this point apply rotationColision. */
				mat4x4 tr = matfromQuat(SCENE.mesh[i].rigid.q, SCENE.mesh[i].coords.v[0]);
				//setvec4arrayMulmat(SCENE.mesh[i].coords.v, 4, tr);
				setfacearrayMulmat(SCENE.mesh[i].rigid.f, SCENE.mesh[i].rigid.f_indexes, tr);

				//setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[0]);
				setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[1]);
				setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[2]);
				setvec4RotateQuat(SCENE.mesh[i].rigid.q, &SCENE.mesh[i].coords.v[3]);

				SCENE.mesh[i].q = multiplyQuats(SCENE.mesh[i].q, SCENE.mesh[i].rigid.q);
				//SCENE.mesh[i].q = SCENE.mesh[i].rigid.q;
			}

			SCENE.mesh[i].coords.v[0] = vecAddvec(SCENE.mesh[i].coords.v[0], SCENE.mesh[i].rigid.velocity);
		}
	}
}


