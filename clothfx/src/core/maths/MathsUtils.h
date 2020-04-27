//
//  MathUtils.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#include <math.h>
#include <iostream>
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

#define EPSILON 0.001f

namespace MathUtils
{
	static tkQuat& VecMultQuat(const tkVec3& v_lhs, const tkQuat& q_rhs)
	{
		tkQuat q(v_lhs.x, v_lhs.y, v_lhs.z, 0);
		return q*q_rhs;
	}
}