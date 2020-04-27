//
//  ClothTriangle.cpp
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#include "ClothTriangle.h"
#include "math.h"

//----------------------------------------------------------------//
ClothTriangle::ClothTriangle(void)
{

}
//----------------------------------------------------------------//
ClothTriangle::~ClothTriangle(void)
{
}
//----------------------------------------------------------------//
void ClothTriangle::CalculateNormal()
{
	float V1[3], V2[3], pn[3];

	int i;
	for (i = 0; i < 3; i++)
	{
		V1[i] = p_A->GetPosition()[i] - p_B->GetPosition()[i];
		V2[i] = p_A->GetPosition()[i] - p_C->GetPosition()[i];
	}

	/*
		Find normal:

						  | i   j   k |
		n = V1 x V2 = det |V1x V1y V1z|
						  |V2x V2y V2z|

		n = i(V1y.V2z - V2y.V1z) - j(V1x.V2z - V2x.V1z) + k(V1x.V2y - V2x.V1y)
	*/

	pn[0] = V1[1] * V2[2] - V2[1] * V1[2];
	pn[1] = -(V1[0] * V2[2] - V2[0] * V1[2]);
	pn[2] = V1[0] * V2[1] - V2[0] * V1[1];

	// normalizing
	float len = (float)sqrt(pn[0] * pn[0] + pn[1] * pn[1] + pn[2] * pn[2]);
	for (i = 0; i < 3; i++)
		normal[i] = pn[i] / len;
}
//----------------------------------------------------------------//
void ClothTriangle::setSprings()
{
	float restLength = 5.0f*(float)sqrt(2.0);
	// link S1
	S[0].setO(p_A);	S[0].setP(p_B); S[0].SetLength(restLength);
	// link S2
	S[1].setO(p_A);	S[1].setP(p_C); S[1].SetLength(5.f);
	// link S3
	S[2].setO(p_B);	S[2].setP(p_C); S[2].SetLength(5.f);
}
//----------------------------------------------------------------//