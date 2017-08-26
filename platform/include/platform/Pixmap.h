
#ifndef PLATFORM_PIXMAP
#define PLATFORM_PIXMAP

#include <platform/Display.h>

#if   defined(PLATFORM_XLIB)

	#include <xlib/Pixmap.h>

#elif defined(PLATFORM_XCB)

	#include <xcb/Pixmap.h>

#elif defined(PLATFORM_ANDROID)

	#include <android/Pixmap.h>

#elif defined(PLATFORM_WINDOWS)

	#include <windows/Bitmap.h>

#endif

#include <util/vec.h>

namespace platform
{
	struct Pixmap
	{

#if   defined(PLATFORM_XLIB)

		xlib::Pixmap    nativePixmap;

#elif defined(PLATFORM_XCB)

		xcb::Pixmap     nativePixmap;

#elif defined(PLATFORM_ANDROID)

		android::Pixmap nativePixmap;

#elif defined(PLATFORM_WINDOWS)

		windows::Bitmap nativePixmap;

#endif

		Pixmap (const Display& display, const util::uvec2& size);
		~Pixmap () = default;

		Pixmap (Pixmap&& pixmap)      = default;
		Pixmap (const Pixmap& pixmap) = delete;

		Pixmap& operator = (Pixmap&& pixmap)      = default;
		Pixmap& operator = (const Pixmap& pixmap) = delete;
	};
}

#endif
