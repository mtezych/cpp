
#include <xlib/Display.h>

#include <cassert>

namespace xlib
{
	Display::Display (const char* name)
	:
		xDisplay { XOpenDisplay(name) }
	{
		assert(xDisplay != None);

#ifndef NDEBUG
		XSynchronize(xDisplay, True);
#endif
	}

	Display::~Display ()
	{
		if (xDisplay != None)
		{
			XCloseDisplay(xDisplay);
		}
	}

	Display::Display (Display&& display)
	:
		xDisplay { display.xDisplay }
	{
		display.xDisplay = None;
	}

	Display& Display::operator = (Display&& display)
	{
		if (xDisplay != None)
		{
			XCloseDisplay(xDisplay);
		}

		xDisplay = display.xDisplay;
		display.xDisplay = None;

		return *this;
	}
}
