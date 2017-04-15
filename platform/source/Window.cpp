
#include <platform/Window.h>

namespace platform
{
	Window::Window (const Display& display, const util::uvec2& size)
	{
	}

	Window::~Window ()
	{
	}

	Window::Window (Window&& window)
	{
	}

	Window& Window::operator = (Window&& window)
	{
		return *this;
	}
}
