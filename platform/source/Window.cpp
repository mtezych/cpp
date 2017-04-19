
#include <platform/Window.h>

namespace platform
{
	Window::Window (const Display& display, const util::uvec2& size)
	:
#if defined(PLATFORM_XLIB)

		nativeWindow { display, size }

#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_WINDOWS)

		nativeWindow { size }

#endif
	{
		static_cast<void>(display);
	}
}
