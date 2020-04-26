//
//  Triangle.h
//  Cloth Simulation Engine
//
//  Created by Angelo Moro on 21/10/2009
//

#pragma once
#include "Particle.h"
#include "Spring.h"

class CTriangle
{
public:
	CTriangle(void);
	~CTriangle(void);
	void CalculateNormal();

	inline void setA(CParticle *p_a){ p_A = p_a; }
	inline void setB(CParticle *p_b){ p_B = p_b; }
	inline void setC(CParticle *p_c){ p_C = p_c; }

	void setSprings();

	inline CParticle *getA() { return p_A; }
	inline CParticle *getB() { return p_B; }
	inline CParticle *getC() { return p_C; }
	inline float GetAreaUV() { return areaUV; }

	inline CSpring *getSprings() { return S; }

private:
	CParticle *p_A, *p_B, *p_C;
	CSpring S[3];
	float n[3];
	float areaUV = 0.03125f;
};