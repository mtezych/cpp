
#include <platform/Window.h>

namespace platform
{
	Window::Window (const Display& display, const util::uvec2& size)
	{
		static_cast<void>(display);
		static_cast<void>(size);
	}

	Window::~Window ()
	{
	}

	Window::Window (Window&& window)
	{
		static_cast<void>(window);
	}

	Window& Window::operator = (Window&& window)
	{
		static_cast<void>(window);

		return *this;
	}
}
