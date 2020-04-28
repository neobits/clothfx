//
//  Plane.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __PLANE_H__
#define __PLANE_H__

#include "../core/Triangle.h"
#include "../core/Entity.h"

class tkPlane : public tkEntity, public tkTriangle
{
public:
	///<summary>Equation : ax + by + cz + d = 0</summary>
	struct PlaneEquation
	{
		float a, b, c, d;
	};

	tkPlane(void);
	tkPlane(tkVec3 pt1, tkVec3 pt2, tkVec3 pt3);
	~tkPlane(void);

	// Evaluate if a point belongs to the plane
	float Evaluate(float *point);
	float Evaluate(tkVec3 point);
	void RecalculateNormal();

	// GET methods
	inline PlaneEquation getEquation() { return eq; }
	inline float getWidth() { return m_Width; }
	inline float getLength() { return m_Length; }
	inline tkVec3 getPt1() { return m_pt1; }
	inline tkVec3 getPt2() { return m_pt2; }
	inline tkVec3 getPt3() { return m_pt3; }
	// SET methods
	inline void setWidth(float width) { m_Width = width; }
	inline void setLenght(float length) { m_Length = length; }

private:
	PlaneEquation eq;
	float m_Width;
	float m_Length;
	tkVec3 m_pt1, m_pt2, m_pt3;
};

#endif // !__PLANE_H__