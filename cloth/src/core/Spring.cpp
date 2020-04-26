#include "Spring.h"

CSpring::CSpring(void)
	:m_Ke(1.0f), m_Kd(10.0f)
{
	checked = false;
}

CSpring::CSpring(CParticle *o, CParticle *p, float length)
{
	O = o;
	P = p;
	m_Length = length;
	m_Ke = 1.0f;
	m_Kd = 10.0f;
}

CSpring::~CSpring(void)
{
}