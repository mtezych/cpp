
#ifndef PLATFORM_WAYLAND_DISPLAY
#define PLATFORM_WAYLAND_DISPLAY

#include <wayland-client.h>

// Wayland:
// ~ Architecture       -> https://wayland.freedesktop.org/architecture.html
// ~ Client Programming -> https://jan.newmarch.name/Wayland/ProgrammingClient

namespace wayland
{
	struct Display
	{
		wl_display* wlDisplay;

		Display (const char* name = nullptr);
		~Display ();

		Display (Display&& display);
		Display& operator = (Display&& display);

		Display (const Display& display) = delete;
		Display& operator = (const Display& display) = delete;
	};
}

#endif
