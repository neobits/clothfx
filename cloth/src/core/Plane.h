//
//  Plane.h
//  Cloth Simulation Engine
//
//  Created by Angelo Moro on 21/10/2009
//

#pragma once

class CPlane
{
public:

	struct PlaneEquation
	{
		float a, b, c, d;
	};

	CPlane(void);
	CPlane(float *pt1, float *pt2, float *pt3);
	void setPlane(float *pt1, float *pt2, float *pt3);
	~CPlane(void);

	// Evaluate if a point belongs to the plane
	float evaluatePoint(float *P);

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
	inline void setNormal(float *pn);
	inline void setPt1(float *pt);
	inline void setPt2(float *pt);
	inline void setPt3(float *pt);

private:
	float normal[3];
	// ecuación: ax + by + cz + d = 0
	PlaneEquation eq;
	float width;
	float length;
	float Pt1[3], Pt2[3], Pt3[3];
};