
#ifndef XLIB_WINDOW
#define XLIB_WINDOW

#include <X11/Xlib.h>

#include "Utils.h"

#include <cstdint>

namespace xlib
{
	struct Display;
	struct Image;

	struct Window
	{
		::Window   xWindow;
		::Display* xDisplay;
		::Screen*  xScreen;
		::Colormap xColormap;

		Window (const xlib::Display& display, const utility::Size<unsigned int>& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		utility::Size<unsigned int> Size() const;

		bool HandleEvent(const XEvent& event) const;

		template <typename Callable>
		void RecieveMessages(const Callable& render) const
		{
			while (true)
			{
				if (XPending(xDisplay))
				{
					auto xEvent = XEvent { };
					XNextEvent(xDisplay, &xEvent);

					if (!HandleEvent(xEvent))
					{
						break;
					}
				}
				else
				{
					render();
				}
			}
		}

		void Clear
		(
			const uint8_t R, const uint8_t G, const uint8_t B, const uint8_t A
		);

		void Draw(const Image& image);
	};
}

#endif
