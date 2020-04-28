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
	:width(5.0f), length(10.0f)
{
}
//----------------------------------------------------------------//
tkPlane::tkPlane(float *pt1, float *pt2, float *pt3)
	: width(5.0f), length(10.0f)
{
	setPt1(pt1);
	setPt2(pt2);
	setPt3(pt3);

	CalculateNormal();
}
//----------------------------------------------------------------//
tkPlane::~tkPlane(void)
{
}
//----------------------------------------------------------------//
void tkPlane::CalculateNormal()
{
	float V1[3], V2[3], nLen;
	int i;
	for (i = 0; i < 3; i++)
	{
		V1[i] = Pt1[i] - Pt2[i];
		V2[i] = Pt1[i] - Pt3[i];
	}
	/*
						  | i   j   k |
		n = V1 x V2 = det |V1x V1y V1z| = i(V1y.V2z - V2y.V1z) - j(V1x.V2z - V2x.V1z) + k(V1x.V2y - V2x.V1y)
						  |V2x V2y V2z|
	*/

	normal[0] = V1[1] * V2[2] - V2[1] * V1[2];
	normal[1] = -(V1[0] * V2[2] - V2[0] * V1[2]);
	normal[2] = V1[0] * V2[1] - V2[0] * V1[1];

	// Normalizing
	nLen = (float)sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	for (i = 0; i < 3; i++)
		normal[i] = normal[i] / nLen;

	// Found d, satisfying the eq. : n x pt1 = -ds
	eq.a = normal[0];	//a
	eq.b = normal[1];	//b
	eq.c = normal[2];	//c
	eq.d = -(normal[0] * Pt1[0] + normal[1] * Pt1[1] + normal[2] * Pt1[2]);	//d
}
//----------------------------------------------------------------//
void tkPlane::setPt1(float *pt)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		Pt1[i] = pt[i];
	}
}
//----------------------------------------------------------------//
void tkPlane::setPt2(float *pt)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		Pt2[i] = pt[i];
	}
}
//----------------------------------------------------------------//
void tkPlane::setPt3(float *pt)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		Pt3[i] = pt[i];
	}
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