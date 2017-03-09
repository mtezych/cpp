
#ifndef XLIB_WINDOW
#define XLIB_WINDOW

#include <X11/Xlib.h>

#include "Drawable.h"

#include <vec.h>

#include <cstdint>

namespace xlib
{
	struct Display;
	struct Pixmap;
	struct Image;

	struct Window : Drawable
	{
		::Window   xWindow;
		::Colormap xColormap;

		Window (const xlib::Display& display, const util::uvec2& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

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
	};
}

#endif