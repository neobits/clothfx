//
//  Matrix.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

#include <math.h>
#include <iostream>


/*
	The array of a squared matrix of 3 is defined as:
	data_[x1, x2, x3, y1, y2, y3, z1, z2, z3]
*/
class tkMat3
{
public:
	tkMat3(void) :_rows(3), _cols(3)
	{
		_data = new float[_rows * _cols];
		for (unsigned i = 0; i < _rows * _cols; i++) { _data[i] = 0; }
	}

	tkMat3(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3) :_rows(3), _cols(3)
	{
		_data = new float[_rows * _cols];
		*_data++ = x1;	*_data++ = x2;	*_data++ = x3;
		*_data++ = y1;	*_data++ = y2;	*_data++ = y3;
		*_data++ = z1;	*_data++ = z2;	*_data++ = z3;
	}

	tkMat3(tkVec3 X, tkVec3 Y, tkVec3 Z) :_rows(3), _cols(3)
	{
		_data = new float[_rows * _cols];

		*_data++ = X.x; *_data++ = Y.x; *_data++ = Z.x;
		*_data++ = X.y; *_data++ = Y.y; *_data++ = Z.y;
		*_data++ = X.z; *_data++ = Y.z; *_data++ = Z.z;
	}

	virtual ~tkMat3(void)
	{
		//delete[] data_;
	}

	inline float& tkMat3::operator() (unsigned row, unsigned col)
	{
		if (row >= _rows || col >= _cols)
		{
			throw "Matrix subscript out of bounds";
			if (row >= _rows) row = _rows - 1;
			if (col >= _cols) col = _cols - 1;
		}
		return _data[_cols*row + col];
	}

	inline float tkMat3::operator() (unsigned row, unsigned col)const
	{
		if (row >= _rows || col >= _cols)
		{
			throw "Matrix subscript out of bounds";
			if (row >= _rows) row = _rows - 1;
			if (col >= _cols) col = _cols - 1;
		}
		return _data[_cols*row + col];
	}

	inline tkVec3& operator*(const tkVec3& rhs)
	{
		float v[3];
		for (int i = 0; i < 3; i++) {
			v[i] = _data[0 + (_cols*i)] * rhs.x +
				_data[1 + (_cols*i)] * rhs.y +
				_data[2 + (_cols*i)] * rhs.z;
		}
		return *(new tkVec3(v[0], v[1], v[2]));
	}

	inline tkMat3 operator*(const tkMat3 &rhs)
	{
		tkMat3 M;

		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 3; i++) {
				M(i, j) = this->_data[_cols*j + i] * rhs(i, j);
			}
		}
		return M;
	}

	inline tkMat3 operator*(const float& k)
	{
		tkMat3 M;

		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 3; i++) {
				M(i, j) = this->_data[_cols*j + i] * k;
			}
		}
		return M;
	}

	// Static functions

	static tkMat3 Transpose(const tkMat3& matrix)
	{
		tkMat3 M;

		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 3; i++) {
				M(j, i) = matrix(i, j);
			}
		}
		return M;
	}

	static tkMat3 Star(tkVec3 v)
	{
		tkMat3 M;

		M(0, 0) = 0;		M(0, 1) = -v.z;		M(0, 2) = v.y;
		M(1, 0) = v.z;		M(1, 1) = 0;		M(1, 2) = -v.x;
		M(2, 0) = -v.y;		M(2, 1) = v.x;		M(2, 2) = 0;

		return M;
	}

private:
	unsigned _rows, _cols;
	float* _data;
};

#endif /*!__MATH_MATRIX_H__*/