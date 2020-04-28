//
//  Plane.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __PLANE_H__
#define __PLANE_H__

#include "../core/Entity.h"

class tkPlane : public tkEntity
{
public:
	struct PlaneEquation
	{
		float a, b, c, d;
	};

	tkPlane(void);
	tkPlane(float *pt1, float *pt2, float *pt3);
	~tkPlane(void);

	// Evaluate if a point belongs to the plane
	float Evaluate(float *point);
	float Evaluate(tkVec3 point);
	void CalculateNormal();

	// SET methods
	inline float *getNormal() { return normal; }
	inline PlaneEquation getEquation() { return eq; }
	inline float getWidth() { return width; }
	inline float getLength() { return length; }
	inline float *getPt1() { return Pt1; }
	inline float *getPt2() { return Pt2; }
	inline float *getPt3() { return Pt3; }
	// SET methods
	inline void setWidth(float width) { width = width; }
	inline void setLenght(float length) { length = length; }
	inline void setPt1(float *pt);
	inline void setPt2(float *pt);
	inline void setPt3(float *pt);

private:
	float normal[3];
	// Equation : ax + by + cz + d = 0
	PlaneEquation eq;
	float width;
	float length;
	float Pt1[3], Pt2[3], Pt3[3];
};

#endif // !__PLANE_H__