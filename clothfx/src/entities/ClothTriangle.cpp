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
void ClothTriangle::RecalculateNormal()
{
	CalculateNormal(p_A->GetPosition(),
					p_B->GetPosition(),
					p_C->GetPosition());
}
//----------------------------------------------------------------//
void ClothTriangle::setSprings()
{
	float restLength = 5.0f*(float)sqrt(2.0);
	// link S1
	m_Springs[0].setO(p_A);	m_Springs[0].setP(p_B); m_Springs[0].SetLength(restLength);
	// link S2
	m_Springs[1].setO(p_A);	m_Springs[1].setP(p_C); m_Springs[1].SetLength(5.f);
	// link S3
	m_Springs[2].setO(p_B);	m_Springs[2].setP(p_C); m_Springs[2].SetLength(5.f);

	RecalculateNormal();
}
//----------------------------------------------------------------//