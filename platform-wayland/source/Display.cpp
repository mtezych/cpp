
#include <wayland/Display.h>

#include <cassert>

namespace wayland
{
	Display::Display (const char* name)
	:
		wlDisplay { wl_display_connect(name) }
	{
		assert(wlDisplay != nullptr);
	}

	Display::~Display ()
	{
		if (wlDisplay != nullptr)
		{
			wl_display_disconnect(wlDisplay);
		}
	}

	Display::Display (Display&& display)
	:
		wlDisplay { display.wlDisplay }
	{
		display.wlDisplay = nullptr;
	}

	Display& Display::operator = (Display&& display)
	{
		if (wlDisplay != nullptr)
		{
			wl_display_disconnect(wlDisplay);
		}

		wlDisplay = display.wlDisplay;
		display.wlDisplay = nullptr;

		return *this;
	}
}
