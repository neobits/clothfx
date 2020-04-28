//
//  Vector.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include <math.h>
#include <iostream>

/* 2-dimensional vector (x,y) */
class tkVec2
{
public:
	float x, y;

	tkVec2(void) { this->x = this->y = 0.f; }
	tkVec2(float v) { this->x = v;  this->y = v; }
	tkVec2(float x, float y) { this->x = x;  this->y = y; }

	inline tkVec2& operator+(const tkVec2 &rhs)
	{
		float x = this->x + rhs.x;
		float y = this->y + rhs.y;
		return *(new tkVec2(x, y));
	}

	inline tkVec2& operator+=(const tkVec2 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	inline tkVec2& operator-(const tkVec2 &rhs)
	{
		float x = this->x - rhs.x;
		float y = this->y - rhs.y;
		return *(new tkVec2(x, y));
	}

	inline tkVec2& operator-=(const tkVec2 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	inline bool operator==(const tkVec2 &rhs) const
	{
		return(this->x == rhs.x && this->y == rhs.y);
	}

	inline bool operator!=(const tkVec2 &rhs) const
	{
		return(this->x != rhs.x || this->y != rhs.y);
	}

	/* Vector2(0, 0) */
	static tkVec2 ZeroVector() { return tkVec2(0); }
	/* Vector2(1, 1) */
	static tkVec2 UnityVector() { return tkVec2(1); }
};

/* 3-dimensional vector (x,y,z) */
class tkVec3
{
public:
	float x, y, z;

	tkVec3(void) { this->x = this->y = this->z = 0.f; }
	tkVec3(float k) { this->x = this->y = this->z = k; }
	tkVec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

	virtual ~tkVec3(void) {}

	inline tkVec3& operator+(const tkVec3 &rhs)
	{
		float x = this->x + rhs.x;
		float y = this->y + rhs.y;
		float z = this->z + rhs.z;
		return *(new tkVec3(x, y, z));
	}

	inline tkVec3& operator+=(const tkVec3 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	inline tkVec3& operator-(const tkVec3 &rhs)
	{
		float x = this->x - rhs.x;
		float y = this->y - rhs.y;
		float z = this->z - rhs.z;
		return *(new tkVec3(x, y, z));
	}

	inline tkVec3& operator-=(const tkVec3 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	inline tkVec3& operator*(const tkVec3 &rhs)
	{
		float x = this->x * rhs.x;
		float y = this->y * rhs.y;
		float z = this->z * rhs.z;
		return *(new tkVec3(x, y, z));
	}

	inline tkVec3& operator*=(const tkVec3 &rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;
		return *this;
	}

	inline tkVec3& operator*(const float k)
	{
		float x = k*this->x;
		float y = k*this->y;
		float z = k*this->z;
		return *(new tkVec3(x, y, z));
	}

	inline tkVec3& operator*=(const float k)
	{
		this->x *= k;
		this->y *= k;
		this->z *= k;
		return *this;
	}

	inline tkVec3& operator/(const float k)
	{
		float x = this->x / k;
		float y = this->y / k;
		float z = this->z / k;
		return *(new tkVec3(x, y, z));
	}

	inline bool operator==(const tkVec3 &rhs) const
	{
		return(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}

	inline bool operator!=(const tkVec3 &rhs) const
	{
		return(this->x != rhs.x || this->y != rhs.y || this->z != rhs.z);
	}

	/* Length of this vector. */
	float Magnitude(void)
	{
		return (sqrt(x*x + y * y + z * z));
	}

	void Normalize(void)
	{
		float magnitude = Magnitude();
		float epsilon = 0.0001f;
		if (magnitude <= epsilon) { magnitude = 1; }
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;

		if (fabs(x) < epsilon) x = 0.0f;
		if (fabs(y) < epsilon) y = 0.0f;
		if (fabs(z) < epsilon) z = 0.0f;
	}

	void Invert(void)
	{
		x = -x; y = -y; z = -z;
	}

	// Static functions

	static tkVec3& Cross(const tkVec3& A, const tkVec3& B)
	{
		float x = A.y*B.z - B.y*A.z;
		float y = -(A.x*B.z - B.x*A.z);
		float z = A.x*B.y - B.x*A.y;
		return *(new tkVec3(x, y, z));
	}

	static float Dot(const tkVec3& A, const tkVec3& B)
	{
		return(A.x*B.x + A.y*B.y + A.z*B.z);
	}

	static tkVec3 Normalized(const tkVec3& V)
	{
		float magnitude = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
		float epsilon = 0.0001f;
		if (magnitude <= epsilon) { magnitude = 1; }
		float x = V.x / magnitude;
		float y = V.y / magnitude;
		float z = V.z / magnitude;

		if (fabs(x) < epsilon) x = 0.0f;
		if (fabs(y) < epsilon) y = 0.0f;
		if (fabs(z) < epsilon) z = 0.0f;

		return tkVec3(x, y, z);
	}

	/* Vector3(0, 0, 0) */
	static tkVec3 ZeroVector() { return tkVec3(0); }
	/* Vector3(1, 1, 1) */
	static tkVec3 UnityVector() { return tkVec3(1); }
	/* Vector3(-1, 0, 0) */
	static tkVec3 LeftVector() { return tkVec3(-1, 0, 0); }
	/* Vector3(1, 0, 0) */
	static tkVec3 RightVector() { return tkVec3(1, 0, 0); }
	/* Vector3(0, 1, 0) */
	static tkVec3 UpVector() { return tkVec3(0, 1, 0); }
	/* Vector3(0, -1, 0) */
	static tkVec3 DownVector() { return tkVec3(0, -1, 0); }
	/* Vector3(0, 0, 1) */
	static tkVec3 ForwardVector() { return tkVec3(0, 0, 1); }
	/* Vector3(0, 0, -1) */
	static tkVec3 BackwardVector() { return tkVec3(0, 0, -1); }
};

#endif /* !__MATH_QUATERNION_H__ */