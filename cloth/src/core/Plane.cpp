#include "Plane.h"
#include <math.h>

//----------------------------------------------------------------//
CPlane::CPlane(void)
	:width(5.0f), length(10.0f)
{
}
//----------------------------------------------------------------//
CPlane::CPlane(float *pt1, float *pt2, float *pt3)
	: width(5.0f), length(10.0f)
{
	float V1[3], V2[3], nLen;
	int i;
	for (i = 0; i < 3; i++)
	{
		V1[i] = pt1[i] - pt2[i];
		V2[i] = pt1[i] - pt3[i];
	}
	/*
		Encontramos n tal que:

						  | i   j   k |
		n = V1 x V2 = det |V1x V1y V1z| = i(V1y.V2z - V2y.V1z) - j(V1x.V2z - V2x.V1z) + k(V1x.V2y - V2x.V1y)
						  |V2x V2y V2z|
	*/

	normal[0] = V1[1] * V2[2] - V2[1] * V1[2];
	normal[1] = -(V1[0] * V2[2] - V2[0] * V1[2]);
	normal[2] = V1[0] * V2[1] - V2[0] * V1[1];

	//ahora normalicemos
	nLen = (float)sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	for (i = 0; i < 3; i++)
		normal[i] = normal[i] / nLen;

	//Encontramos la constante d, de la ecuación n . pt1 = -d
	eq.a = normal[0];	//a
	eq.b = normal[1];	//b
	eq.c = normal[2];	//c
	eq.d = -(normal[0] * pt1[0] + normal[1] * pt1[1] + normal[2] * pt1[2]);	//d
	setPt1(pt1);
	setPt2(pt2);
	setPt3(pt3);
}
//----------------------------------------------------------------//
CPlane::~CPlane(void)
{
}
//----------------------------------------------------------------//
void CPlane::setNormal(float *pn)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		normal[i] = pn[i];
	}
}
//----------------------------------------------------------------//
void CPlane::setPt1(float *pt)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		Pt1[i] = pt[i];
	}
}
//----------------------------------------------------------------//
void CPlane::setPt2(float *pt)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		Pt2[i] = pt[i];
	}
}
//----------------------------------------------------------------//
void CPlane::setPt3(float *pt)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		Pt3[i] = pt[i];
	}
}
//----------------------------------------------------------------//
void CPlane::setPlane(float *pt1, float *pt2, float *pt3)
{
	float V1[3], V2[3], pn[3], nLen;
	int i;
	for (i = 0; i < 3; i++)
	{
		V1[i] = pt1[i] - pt2[i];
		V2[i] = pt1[i] - pt3[i];
	}

	/*
						  | i   j   k |
		n = V1 x V2 = det |V1x V1y V1z| = i(V1y.V2z - V2y.V1z) - j(V1x.V2z - V2x.V1z) + k(V1x.V2y - V2x.V1y)
						  |V2x V2y V2z|
	*/

	pn[0] = V1[1] * V2[2] - V2[1] * V1[2];
	pn[1] = -(V1[0] * V2[2] - V2[0] * V1[2]);
	pn[2] = V1[0] * V2[1] - V2[0] * V1[1];

	// Normalizing
	nLen = (float)sqrt(pn[0] * pn[0] + pn[1] * pn[1] + pn[2] * pn[2]);
	for (i = 0; i < 3; i++)
		pn[i] = pn[i] / nLen;

	// Found d, satisfying the eq. : n x pt1 = -d
	eq.a = pn[0];
	eq.b = pn[1];
	eq.c = pn[2];
	eq.d = -(pn[0] * pt1[0] + pn[1] * pt1[1] + pn[2] * pt1[2]);

	setNormal(pn);
	setPt1(pt1);
	setPt2(pt2);
	setPt3(pt3);
}
//----------------------------------------------------------------//
float CPlane::evaluatePoint(float *P)
{
	// solve H(P) = a*Px + b*Py + c*Pz + d
	return eq.a * P[0] + eq.b * P[1] + eq.c * P[2] + eq.d;
}
//----------------------------------------------------------------//