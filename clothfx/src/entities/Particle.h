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
	}
	virtual ~tkParticle(void) {}

	// SET methods
	inline void SetMass(float m) { mass = m; }
	inline void SetRadius(float r) { radius = r; }
	inline void SetPosition(tkVec3 p) {m_Position = p; }
	inline void SetPosition(float *p) {
		m_Position.x = p[0]; m_Position.y = p[1]; m_Position.z = p[2];
	}
	inline void SetUV(tkVec2 uv) { m_uv = uv; }
	inline void SetUV(float *uv) { m_uv.x = uv[0]; m_uv.y = uv[1]; }

	inline void SetVelovity(tkVec3 v) { m_Velocity = v; }
	inline void SetVelovity(float *v) {
		m_Velocity.x = v[0]; m_Velocity.y = v[1]; m_Velocity.z = v[2];
	}

	inline void SetForce(tkVec3 f) { m_F = f; }
	inline void SetForce(float *f) {
		m_F.x = f[0]; m_F.y = f[1]; m_F.z = f[2];
	}

	inline void AddVelocity(tkVec3 dv) { m_Velocity += dv; }
	//inline void AddVelocity(float *dv) {
	//	for (int i = 0; i < 3; i++) { Velocity[i] += dv[i]; }
	//}

	void AddForce(tkVec3 df) { m_F += df; }
	
	void AddForce(float *df) {
		m_F.x += df[0]; m_F.y += df[1]; m_F.z += df[2];
	}

	inline tkVec3 GetPosition() { return m_Position; }
	inline tkVec2 GetUV() { return m_uv; }
	inline tkVec3 GetVelocity() { return m_Velocity; }
	inline tkVec3 GetForce() { return m_F; }
	inline float GetMass() { return mass; }
	inline float GetRadius() { return radius; }

private:
	tkVec3 m_Position;	// Global coordinates
	tkVec3 m_Velocity;	// Current velocity
	tkVec2 m_uv;		// UV coordinates
	tkVec3 m_F;			// Forces applied
	float mass;			//
	float radius;		//
};

#endif // !__PARTICLE_H__