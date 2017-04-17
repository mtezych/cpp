
#ifndef PLATFORM_WINDOW
#define PLATFORM_WINDOW

#include <platform/Display.h>

#include <util/vec.h>

namespace platform
{
	struct Window
	{
		Window (const Display& display, const util::uvec2& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		template <typename Callable>
		void ReceiveMessages(const Callable& render) const
		{
			static_cast<void>(render);
		}
	};
}

#endif
