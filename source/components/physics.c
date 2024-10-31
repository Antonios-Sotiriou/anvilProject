#include "headers/components/physics.h"

/* Responsible to apply all the forces that act on the rigid body and orginise the appropriate Collision detection functions steps. */
void applyPhysics(void) {
	for (int i = 0; i < SCENE.mesh_indexes; i++) {
		if (SCENE.mesh[i].rigid.state == ENABLE) {
			SCENE.mesh[i].coords.v[0] = vecAddvec(SCENE.mesh[i].coords.v[0], SCENE.mesh[i].rigid.velocity);

			if (SCENE.mesh[i].rigid.rot_angle) {
				/* At this point apply rotationColision. */
				mat4x4 tr = matfromQuat(SCENE.mesh[i].rigid.q, SCENE.mesh[i].coords.v[0]);
				setvec4arrayMulmat(SCENE.mesh[i].coords.v, 4, tr);
			}
		}
	}
}


