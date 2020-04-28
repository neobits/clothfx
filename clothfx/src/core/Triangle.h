//
//  Triangle.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "maths/Vector.h"

class tkTriangle
{
public:
	tkTriangle(){}
	~tkTriangle(){}

	inline void CalculateNormal(tkVec3 pA, tkVec3 pB, tkVec3 pC)
	{
		tkVec3 Vec1, Vec2, pn;
		Vec1 = pA - pB;
		Vec2 = pA - pC;

		/*
			Find normal:

							  | i   j   k |
			n = V1 x V2 = det |V1x V1y V1z|
							  |V2x V2y V2z|

			n = i(V1y.V2z - V2y.V1z) - j(V1x.V2z - V2x.V1z) + k(V1x.V2y - V2x.V1y)
		*/

		pn.x = Vec1.y * Vec2.z - Vec2.y * Vec1.z;
		pn.y = -(Vec1.x * Vec2.z - Vec2.x * Vec1.z);
		pn.z = Vec1.x * Vec2.y - Vec2.x * Vec1.y;

		// normalizing
		normal = pn / pn.Magnitude();
	}

	inline tkVec3 GetNormal() { return normal; }

protected:
	tkVec3 normal;
};

#endif