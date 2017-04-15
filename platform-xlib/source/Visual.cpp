
#include <xlib/Visual.h>

#include <xlib/Display.h>

#include <X11/Xutil.h>

#include <cassert>

namespace xlib
{
	Visual::Visual(const Display& display)
	:
		xVisual  { None },
		xDisplay { display.xDisplay },
		xScreen  { XDefaultScreenOfDisplay(xDisplay) }
	{
		auto xVisualInfo = XVisualInfo { };
		const auto status = XMatchVisualInfo
		(
			xDisplay, XScreenNumberOfScreen(xScreen),
			32, TrueColor,
			&xVisualInfo
		);
		assert(status != False);

		xVisual = xVisualInfo.visual;
	}
}
