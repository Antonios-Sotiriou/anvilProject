#include "headers/components/physics.h"

 const static vec4 gravity_epicenter = { 0.f, -1.f, 0.f, 0.f };

/* Responsible to apply all the forces that act on the rigid body and orginise the appropriate Collision detection functions steps. */
void applyPhysics(scene *s) {

	for (int i = 0; i < s->model_indexes; i++) {

		if (s->model[i].owns_rigid == ENABLED) {

			if ((s->model[i].model_type != MODEL_TYPE_TERRAIN && !s->model[i].rigid.grounded) || (checkAllZeros(s->model[i].velocity) || s->model[i].rotate)) {

				initModelQuadInfo(s, &s->model[i]);
				// retrieveNearbyColliders(s, &s->model[i]);
				//logTerrainQuad(s, 0);
				// Compute air resistance (drag).
				// Fd = 0.5f * p * v^2 * Cd * A;
				// p = Density of the air.At sea level with 15 Celcius is approximatly 1.225 kg/m^3.
				// v = the velocity of the object.
				// Cd = Drag coeficient.(aerodynamics friendly shapes have less)
				// A = the area of the object facing the direction opposite to wind resistance.

				// Cd = (2 * fluid_resistance) / (fluid_mass_density velocity^2 * object_area)
				// A cube has a Cd of 1.05f

				float g_accelaration = 0.f;
				if (!s->model[i].rigid.grounded) {
					s->model[i].rigid.falling_time += s->mtr.deltaTime;
					g_accelaration = 0.0081f * (s->model[i].rigid.falling_time * s->model[i].rigid.falling_time);
				}
				s->model[i].velocity = vecAddvec(vecMulf32(gravity_epicenter, g_accelaration), s->model[i].velocity);

				/* 1st Collision Detection lvl. */
				if (staticOuterRadiusCollision(s, &s->model[i])) {
					/* 2nd Collision Detection lvl. */
					int collide = 1;
					while (collide) {
						sortCollisions(s, &s->model[i]);
						collide = sweptAABBCollision(s, &s->model[i]);
					}
				}

				/* 3rd Collision Detection lvl. */
				//if (s->model[i].pk == camera) {
				//	staticOBBCollision(&s->model[i], 3);
				//}

				/* 4th Collision Detection lvl. */
				/* At this point apply rotationColision. */
			}

			if (s->model[i].rotate) {
				rotationCollision(s, &s->model[i]);
				//setvec4RotateQuat(s->model[i].rigid.q, &s->model[i].coords.v[0]);
				setvec4RotateQuat(s->model[i].rigid.q, &s->model[i].coords.v[1]);
				setvec4RotateQuat(s->model[i].rigid.q, &s->model[i].coords.v[2]);
				setvec4RotateQuat(s->model[i].rigid.q, &s->model[i].coords.v[3]);

				s->model[i].q = multiplyQuats(s->model[i].q, s->model[i].rigid.q);
				setfacesArrayMulMat(s->model[i].rigid.f, s->model[i].rigid.faces_indexes, matFromQuat(s->model[i].rigid.q, s->model[i].coords.v[0]));
			}

			if (s->model[i].model_type != MODEL_TYPE_TERRAIN) {
				modelTerrainCollision(s, &s->model[i]);
			}

			mat4x4 tm = translationMatrix(vec4ExtractX(s->model[i].velocity), vec4ExtractY(s->model[i].velocity), vec4ExtractZ(s->model[i].velocity));
			setvec4ArrayMulmat(s->model[i].coords.v, 4, tm);
			setfacesArrayMulMat(s->model[i].rigid.f, s->model[i].rigid.faces_indexes, tm);
		}
	}
}


