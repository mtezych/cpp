
#ifndef PLATFORM_WINDOW
#define PLATFORM_WINDOW

#include <platform/Display.h>

#if defined(PLATFORM_XLIB)

	#include <xlib/Window.h>

#elif defined(PLATFORM_XCB)

	#include <xcb/Window.h>

#elif defined(PLATFORM_ANDROID)

	#include <android/Window.h>

#elif defined(PLATFORM_WINDOWS)

	#include <windows/Window.h>

#endif

#include <util/vec.h>

namespace platform
{
	struct Window
	{

#if defined(PLATFORM_XLIB)

		xlib::Window    nativeWindow;

#elif defined(PLATFORM_XCB)

		xcb::Window     nativeWindow;

#elif defined(PLATFORM_ANDROID)

		android::Window nativeWindow;

#elif defined(PLATFORM_WINDOWS)

		windows::Window nativeWindow;

#endif

		Window (const Display& display, const util::uvec2& size);
		~Window () = default;

		Window (Window&& window)      = default;
		Window (const Window& window) = delete;

		Window& operator = (Window&& window)      = default;
		Window& operator = (const Window& window) = delete;

		template <typename Callable>
		void ReceiveMessages (const Callable& render) const
		{
			nativeWindow.ReceiveMessages(render);
		}
	};
}

#endif
