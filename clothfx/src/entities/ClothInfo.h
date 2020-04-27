//
//  ClothInfo.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once
#include "ClothTriangle.h"

class ClothInfo
{
public:
	ClothInfo(void)
		:m_kStretch(4.0f), m_kShear(0.1f), m_kBend(1.0f), m_nTriangles(32), m_kDrag(0.0f)
	{
	}
	~ClothInfo(void) {}

	ClothTriangle *getTris() { return T; }
	inline float GetStretch() { return m_kStretch; }
	inline float GetShear() { return m_kShear; }
	inline float GetBend() { return m_kBend; }
	inline float GetDrag() { return m_kDrag; }
	inline int TriangleCount() { return m_nTriangles; }

private:
	ClothTriangle T[32];	// Triangle array
	int m_nTriangles;	// Number of  cloth triangles
	float m_kStretch;	// Stretch energy constant
	float m_kShear;		// Shear energy constant
	float m_kBend;		// Bend energy constant
	float m_kDrag;
};