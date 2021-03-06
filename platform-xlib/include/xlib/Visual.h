
#ifndef PLATFORM_XLIB_VISUAL
#define PLATFORM_XLIB_VISUAL

#include <X11/Xlib.h>

namespace xlib
{
	struct Display;

	struct Visual
	{
		::Visual*  xVisual;
		::Display* xDisplay;
		::Screen*  xScreen;

		Visual(const xlib::Display& display);

		~Visual() = default;
	};
}

#endif
