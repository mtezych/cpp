
#include <windows/Bitmap.h>

namespace windows
{
	Bitmap::Bitmap (const util::uvec2& size)
	:
		bitmapHandle { nullptr }
	{
		// CreateDIBSection
		// CreateCompatibleBitmap
		// CreateBitmap
		// CreateDIBitmap
	}

	Bitmap::~Bitmap ()
	{
	}

	Bitmap::Bitmap (Bitmap&& bitmap)
	:
		bitmapHandle { bitmap.bitmapHandle }
	{
		bitmap.bitmapHandle = nullptr;
	}

	Bitmap& Bitmap::operator = (Bitmap&& bitmap)
	{
		bitmapHandle = bitmap.bitmapHandle;

		bitmap.bitmapHandle = nullptr;

		return *this;
	}
}
