//
//  ClothTriangle.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once
#include "../core/Triangle.h"
#include "Particle.h"
#include "Spring.h"

class ClothTriangle : public tkTriangle
{
public:
	ClothTriangle(void);
	~ClothTriangle(void);
	void RecalculateNormal();

	inline void setA(tkParticle *p_a){ p_A = p_a; }
	inline void setB(tkParticle *p_b){ p_B = p_b; }
	inline void setC(tkParticle *p_c){ p_C = p_c; }

	void setSprings();

	inline tkParticle *getA() { return p_A; }
	inline tkParticle *getB() { return p_B; }
	inline tkParticle *getC() { return p_C; }
	inline float GetAreaUV() { return areaUV; }

	inline tkSpring *GetSprings() { return m_Springs; }

private:
	tkParticle *p_A, *p_B, *p_C;
	tkSpring m_Springs[3];

	float areaUV = 0.03125f;
};