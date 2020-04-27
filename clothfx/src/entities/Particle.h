//
//  Particle.cpp
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "../core/Entity.h"

class tkParticle : public tkEntity
{
public:
	tkParticle(void)
	{
		mass = 1.0f;

		for (int i = 0; i < 3; i++)
			Position[i] = Velocity[i] = F[i] = 0.0f;

		UV[0] = UV[1] = 0.0f;
	}
	virtual ~tkParticle(void) {}

	// SET methods
	inline void SetMass(float m) { mass = m; }
	inline void SetRadius(float r) { radius = r; }
	inline void SetPosition(float *p) {
		for (int i = 0; i < 3; i++) { Position[i] = p[i]; }
	}
	inline void SetUV(float *uv) {
		for (int i = 0; i < 2; i++) { UV[i] = uv[i]; }
	}
	inline void SetVelovity(float *v) {
		for (int i = 0; i < 3; i++) { Velocity[i] = v[i]; }
	}
	inline void SetForce(float *f) {
		for (int i = 0; i < 3; i++) { F[i] = f[i]; }
	}
	inline void AddVelocity(float *dv) {
		for (int i = 0; i < 3; i++) { Velocity[i] += dv[i]; }
	}

	void AddForce(float *df) {
		for (int i = 0; i < 3; i++) { F[i] += df[i]; }
	}

	inline float* GetPosition() { return Position; }
	inline float* GetUV() { return UV; }
	inline float* GetVelocity() { return Velocity; }
	inline float* GetForce() { return F; }
	inline float GetMass() { return mass; }
	inline float GetRadius() { return radius; }

private:
	float Position[3];	// Global coordinates
	float UV[2];		// UV coordinates
	float Velocity[3];	// Vl
	float F[3];			// Forces applied
	float mass;			//
	float radius;		//
};

#endif // !__PARTICLE_H__