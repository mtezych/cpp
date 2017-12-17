
#ifndef PLATFORM_WINDOWS_BITMAP
#define PLATFORM_WINDOWS_BITMAP

#include <Windows.h>

#include <util/vec.h>

namespace windows
{
	struct Bitmap
	{
		HBITMAP bitmapHandle;
		HDC     deviceContextHandle;

		Bitmap (const util::uvec2& size);
		~Bitmap ();

		Bitmap (Bitmap&& bitmap);
		Bitmap (const Bitmap& bitmap) = delete;

		Bitmap& operator = (Bitmap&& bitmap);
		Bitmap& operator = (const Bitmap& bitmap) = delete;

		util::uvec2 Size() const;
	};
}

#endif
