//
//  Entity.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "maths\Vector.h"
#include "maths\Quaternion.h"

class tkEntity
{
public:
	tkEntity(){}
	~tkEntity(){}

	virtual void Begin() {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void Destroy() {};

	inline void SetPosition(const tkVec3& position) { this->position = position; }
	inline void SetRotation(const tkQuat& rotation) { this->q = rotation; }
	inline void SetScale(const tkVec3& scale) { this->scale = scale; }
	
	tkVec3 position;	/* x(t) : position	*/
	tkQuat q;			/* q(t) : rotation	*/
	tkVec3 scale;		/* Scale */
};

#endif /*!__ENTITY_H__*/