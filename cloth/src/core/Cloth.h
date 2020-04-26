//
//  Cloth.h
//  Cloth Simulation Engine
//
//  Created by Angelo Moro on 21/10/2009
//

#pragma once
#include "Triangle.h"

class CCloth
{
public:
	CCloth(void);
	~CCloth(void);

	CTriangle *getTri() { return T; }
	float getStretchEk() { return Ks_stretch; }
	float getShearEk() { return Ks_shear; }
	float getBendEk() { return Ks_bend; }
	int getTriangleCount() { return numT; }

private:
	CTriangle T[32];	// Triangle array
	int numT;			// Number of triangles
	float Ks_stretch;	// Stretch energy constant
	float Ks_shear;		// Shear energy constant
	float Ks_bend;		// Bend energy constant
};