//
//  Quaternion.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __MATH_QUATERNION_H__
#define __MATH_QUATERNION_H__

#include <math.h>
#include <iostream>
#include <assert.h>
#include "Matrix.h"

/* Quaternion */
class tkQuat
{
public:
	float x;
	float y;
	float z;
	float w;

	tkQuat() { x = 0; y = 0; z = 0; w = 1; }
	tkQuat(float px, float py, float pz, float pw)
		:x(px), y(py), z(pz), w(pw) {}
	virtual ~tkQuat() {}

	/*inline Vector3& operator*(const Vector3 &vec)
	{
		Quaternion q = Quaternion(this->x, this->y, this->z, this->w);
		Matrix3x3 m = Quaternion::QuatToMatrix(q);

		return *(new Vector3());
	}*/

	inline tkQuat& operator*(const tkQuat& q_rhs)
	{
		float w = this->w*q_rhs.w - this->x*q_rhs.x - this->y*q_rhs.y - this->z*q_rhs.z;
		float x = this->w*q_rhs.x + this->x*q_rhs.w + this->y*q_rhs.z - this->z*q_rhs.y;
		float y = this->w*q_rhs.y - this->x*q_rhs.z + this->y*q_rhs.w + this->z*q_rhs.x;
		float z = this->w*q_rhs.z + this->x*q_rhs.y - this->y*q_rhs.x + this->z*q_rhs.w;
		return *(new tkQuat(x, y, z, w));
	}

	inline tkQuat& operator*(const float k)
	{
		float x = k*this->x;
		float y = k*this->y;
		float z = k*this->z;
		float w = k*this->w;
		return *(new tkQuat(x, y, z, w));
	}

	float Magnitude(void)
	{
		float sq = x*x + y*y + z*z + w*w;
		assert(sq > 0.1f);
		return sqrt(sq);
	}

	void Normalize()
	{
		float magnitude = Magnitude();
		this->w /= magnitude;
		this->x /= magnitude;
		this->y /= magnitude;
		this->z /= magnitude;
	}

	static tkVec3& EulerAngles(const tkQuat& q)
	{
		return *(new tkVec3(roll(q), pitch(q), yaw(q)));
	}

	static float roll(const tkQuat& q)
	{
		return (atan2(2 * (q.w*q.x + q.y*q.z), 1 - 2 * (q.x*q.x - q.y*q.y)));
	}

	static float pitch(const tkQuat& q)
	{
		return (asin(2 * (q.w * q.y - q.z * q.x)));
	}

	static float yaw(const tkQuat& q)
	{
		return (atan2(2 * (q.w*q.z + q.x*q.y), 1 - 2 * (q.y*q.y - q.z*q.z)));
	}

	static tkQuat Conjugate(tkQuat q)
	{
		return tkQuat(-q.x, -q.y, -q.z, q.w);
	}

	static tkQuat Inverse(tkQuat q)
	{
		return tkQuat(-q.x, -q.y, -q.z, -q.w);
	}

	static tkQuat Normalized(const tkQuat& q)
	{
		float sq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
		assert(sq > 0.01f);
		float magnitude = sqrt(sq);
		float qx = q.x / magnitude;
		float qy = q.y / magnitude;
		float qz = q.z / magnitude;
		float qw = q.w / magnitude;
		return tkQuat(qx, qy, qz, qw);
	}

	/* r = ( cos(theta/2), n sin(theta/2) ), where theta angle (rad) and the axis 'n'. */
	static tkQuat QuatFromAxisAngle(const tkVec3 &axis, float angleInRadians)
	{
		float angle = angleInRadians / 2.0f;
		float sinAngle = sin(angle);
		tkVec3 n = tkVec3::Normalized(axis);
		float qw = cos(angle);
		float qx = n.x * sinAngle;
		float qy = n.y * sinAngle;
		float qz = n.z * sinAngle;
		return tkQuat(qx, qy, qz, qw);
	}

	static tkMat3 QuatToMatrix(const tkQuat& q)
	{
		tkMat3 M;

		float xSq = q.x*q.x;
		float ySq = q.y*q.y;
		float zSq = q.z*q.z;
		float wSq = q.w*q.w;

		M(0, 0) = 1 - 2 * ySq - 2 * zSq;
		M(1, 0) = 2 * q.x*q.y + 2 * q.w*q.z;
		M(2, 0) = 2 * q.x*q.z - 2 * q.w*q.y;

		M(0, 1) = 2 * q.x*q.y - 2 * q.w*q.z;
		M(1, 1) = 1 - 2 * xSq - 2 * zSq;
		M(2, 1) = 2 * q.y*q.z + 2 * q.w*q.x;

		M(0, 2) = 2 * q.x*q.y + 2 * q.w*q.z;
		M(1, 2) = 2 * q.y*q.z - 2 * q.w*q.x;
		M(2, 2) = 1 - 2 * xSq - 2 * ySq;

		return M;
	}

	/*static tkQuat MatrixToQuaternion(const tkMat3& M)
	{
		return tkQuat();
	}*/
};

#endif /* !__MATH_QUATERNION_H__ */