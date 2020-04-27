//
//  WindowInfo.h
//  RiBo Engine
//
//  Created by Angelo Moro on 10/12/2019
//

#pragma once

#ifndef __CORE_WINDOW_INFO_H__
#define __CORE_WINDOW_INFO_H__

namespace Core 
{
	class tkWindowInfo
	{
	private:
	
		const char* title;
		int width;
		int height;

	public:
		tkWindowInfo(const char* title)
		{
			this->title = title;
			width = 800;
			height = 600;
		}

		tkWindowInfo() {}
		~tkWindowInfo() {}

		inline int GetHeight() { return height; }
		inline void SetHeight(int h) { height = h; }
		inline int GetWidth() { return width; }
		inline void SetWidth(int w) { width = w; }
		inline const char* GetTitle() { return title; }
		inline void SetTitle(char* t) { title = t; }

		/* Returns the aspect ration. */
		float GetAspect()
		{
			return float(width / height);
		}

		void SetDimensions(int w, int h)
		{
			width = w;
			height = h;
		}
	};
}

#endif /*!__CORE_WINDOW_INFO_H__*/