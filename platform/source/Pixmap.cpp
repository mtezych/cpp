
#include <platform/Pixmap.h>

namespace platform
{
	Pixmap::Pixmap (const Display& display, const util::uvec2& size)
	:
#if   defined(PLATFORM_XLIB)    || defined(PLATFORM_XCB)

		nativePixmap { display, size }

#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_WINDOWS)

		nativePixmap { size }

#endif
	{
		static_cast<void>(display);
	}
}
