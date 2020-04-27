//
//  Color.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __CORE_COLOR_H__
#define __CORE_COLOR_H__

namespace Core 
{
	/* Representation of a color in RGBA channels.
	Use values between [0, 1] */
	struct Color
	{
		GLclampf R; /* Red channel value between [0, 1] */
		GLclampf G; /* Green channel value between [0, 1] */
		GLclampf B; /* Blue channel value between [0, 1] */
		GLclampf A; /* Alpha channel value between [0, 1] */

		Color(GLclampf r, GLclampf g, GLclampf b, GLclampf alpha)
			:R(r), G(g), B(b), A(alpha) {}

		Color(GLclampf r, GLclampf g, GLclampf b)
			:Color(r, g, b, 1) {}

		Color() : Color(1, 1, 1, 1) {}

		static Color White() { return Color(1, 1, 1); }
		static Color Red() { return Color(1, 0, 0); }
		static Color Green() { return Color(0, 1, 0); }
		static Color Blue() { return Color(0, 0, 1); }
		static Color Grey() { return Color(0.5f, 0.5f, 0.5f); }
		static Color LightGrey() { return Color(0.85f, 0.85f, 0.85f); }
		static Color DarkGrey() { return Color(0.25f, 0.25f, 0.25f); }
	};
}

#endif /*!__CORE_COLOR_H__*/
