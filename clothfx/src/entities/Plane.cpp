//
//  Plane.cpp
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#include "Plane.h"
#include <math.h>

//----------------------------------------------------------------//
tkPlane::tkPlane(void)
	:m_Width(5.0f), m_Length(10.0f)
{
}
//----------------------------------------------------------------//
tkPlane::tkPlane(tkVec3 pt1, tkVec3 pt2, tkVec3 pt3)
	: m_Width(5.0f), m_Length(10.0f)
{
	m_pt1 = pt1;
	m_pt2 = pt2;
	m_pt3 = pt3;

	RecalculateNormal();
}
//----------------------------------------------------------------//
tkPlane::~tkPlane(void)
{
}
//----------------------------------------------------------------//
void tkPlane::RecalculateNormal()
{
	CalculateNormal(m_pt1, m_pt2, m_pt3);

	// Found d, satisfying the eq. : n x pt1 = -d
	eq.a = normal.x;	//a
	eq.b = normal.y;	//b
	eq.c = normal.z;	//c
	eq.d = -(normal.x * m_pt1.x + normal.y * m_pt1.y + normal.z * m_pt1.z);
}
//----------------------------------------------------------------//
float tkPlane::Evaluate(float *point)
{
	// solve H(P) = a*Px + b*Py + c*Pz + d
	return eq.a * point[0] + eq.b * point[1] + eq.c * point[2] + eq.d;
}
//----------------------------------------------------------------//
float tkPlane::Evaluate(tkVec3 point)
{
	// solve H(P) = a*Px + b*Py + c*Pz + d
	return eq.a * point.x + eq.b * point.y + eq.c * point.z + eq.d;
}
//----------------------------------------------------------------//