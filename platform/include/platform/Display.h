
#ifndef PLATFORM_DISPLAY
#define PLATFORM_DISPLAY

#if defined(PLATFORM_XLIB)

	#include <xlib/Display.h>

#elif defined(PLATFORM_XCB)

	#include <xcb/Connection.h>

#elif defined(PLATFORM_WAYLAND)

	#include <wayland/Display.h>

#endif

// Display Server Connection

namespace platform
{
#if defined(PLATFORM_XLIB)

	using Display =    xlib::Display;

#elif defined(PLATFORM_XCB)

	using Display =     xcb::Connection;

#elif defined(PLATFORM_WAYLAND)

	using Display = wayland::Display;

#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_WINDOWS)

	struct Display
	{
		Display () = default;
		~Display () = default;

		Display (Display&& display) = default;
		Display (const Display& display) = delete;

		Display& operator = (Display&& display) = default;
		Display& operator = (const Display& display) = delete;
	};

#endif
}

#endif
