//
//  Spring.h
//  Cloth Simulation Engine
//
//  Created by Angelo Moro on 21/10/2009
//

#pragma once

#include "Particle.h"

class CSpring
{
public:
	CSpring(void);
	CSpring(CParticle *o, CParticle *p, float length);
	~CSpring(void);
	inline void setLength(float l) { m_Length = l; }
	inline void setEllasticK(float Ke) { m_Ke = Ke; }
	inline void setDampK(float Kd) { m_Kd = Kd; }

	inline CParticle* getO() { return O; }
	inline CParticle* getP() { return P; }

	inline void setO(CParticle* p_O) { O = p_O; }
	inline void setP(CParticle* p_P) { P = p_P; }

	inline float getLength() { return m_Length; }
	inline float getEllasticK() { return m_Ke; }
	inline float getDampK() { return m_Kd; }

	// is the spring already checked?
	bool checked;

private:
	/*
	O                P
	|--/\/\/\/\/\/\--|
	|--------L-------|
	*/
	CParticle* O;
	CParticle* P;
	float m_Length;	// rest length
	float m_Ke;		// constante de elasticidad
	float m_Kd;		// constante de amortiguamiento
};