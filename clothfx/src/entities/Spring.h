//
//  Spring.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#include "Particle.h"

class tkSpring
{
public:
	tkSpring(void) :m_Ke(1.0f), m_Kd(10.0f)
	{
		checked = false;
	}
	tkSpring(tkParticle *o, tkParticle *p, float length)
	{
		O = o;
		P = p;
		m_Length = length;
		m_Ke = 1.0f;
		m_Kd = 10.0f;
	}
	~tkSpring(void) {}
	inline void SetLength(float len) { m_Length = len; }
	inline void SetElasticity(float Ke) { m_Ke = Ke; }
	inline void SetDamping(float Kd) { m_Kd = Kd; }

	inline tkParticle* getO() { return O; }
	inline tkParticle* getP() { return P; }

	inline void setO(tkParticle* p_O) { O = p_O; }
	inline void setP(tkParticle* p_P) { P = p_P; }

	inline float GetLength() { return m_Length; }
	inline float GetElastic() { return m_Ke; }
	inline float GetDamp() { return m_Kd; }

	// is the spring already checked?
	bool checked;

private:
	/*
	O                P
	|--/\/\/\/\/\/\--|
	|--------L-------|
	*/
	tkParticle* O;
	tkParticle* P;
	float m_Length;	// Rest length
	float m_Ke;		// Elastic constant
	float m_Kd;		// Damp constant
};