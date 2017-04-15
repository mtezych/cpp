
#ifndef PLATFORM_WINDOW
#define PLATFORM_WINDOW

#include <util/vec.h>

namespace platform
{
	struct Display;

	struct Window
	{
		Window (const Display& display, const util::uvec2& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		template <typename Callable>
		void RecieveMessages(const Callable& render) const
		{
		}
	};
}

#endif
