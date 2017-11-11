
#ifndef PLATFORM_WINDOWS_BITMAP
#define PLATFORM_WINDOWS_BITMAP

#define UNICODE
#include <Windows.h>

#include <util/vec.h>

#include <cassert>

namespace windows
{
	struct Bitmap
	{
		HBITMAP bitmapHandle;

		Bitmap (const util::uvec2& size);
		~Bitmap ();

		Bitmap (Bitmap&& bitmap);
		Bitmap (const Bitmap& bitmap) = delete;

		Bitmap& operator = (Bitmap&& bitmap);
		Bitmap& operator = (const Bitmap& bitmap) = delete;
	};
}

#endif
