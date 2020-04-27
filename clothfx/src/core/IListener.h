//
//  IListener.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __ILISTENER_H__
#define __ILISTENER_H__


namespace Core
{
	class IListener
	{
	public:
		virtual ~IListener() = 0;

		// Pure abstract virtual functions (notifiers), i.e. method() = 0
		inline virtual void Init() = 0;
		inline virtual void Update() = 0;
		inline virtual void Render() = 0;
		inline virtual void KeyPressed(unsigned char key, int x, int y) = 0;
	};

	inline IListener::~IListener() {}
}


#endif /*!__ILISTENER_H__*/