//
//  Cloth.cpp
//  Cloth Simulation Engine
//
//  Created by Angelo Moro on 21/10/2009
//

#include "Cloth.h"

//----------------------------------------------------------------//
CCloth::CCloth(void)
	:Ks_stretch(4.0f), Ks_shear(0.1f), Ks_bend(1.0f), numT(32)
{
}
//----------------------------------------------------------------//
CCloth::~CCloth(void)
{
}
//----------------------------------------------------------------//