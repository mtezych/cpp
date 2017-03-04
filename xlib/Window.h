
#ifndef XLIB_WINDOW
#define XLIB_WINDOW

#include <X11/Xlib.h>

#include <vec.h>

#include <cstdint>

namespace xlib
{
	struct Display;
	struct Pixmap;
	struct Image;

	struct Window
	{
		::Window   xWindow;
		::Display* xDisplay;
		::Screen*  xScreen;
		::Colormap xColormap;

		Window (const xlib::Display& display, const util::uvec2& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		util::uvec2 Size() const;

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

		void Clear (const util::vec4& color);

		void Draw (const Pixmap& pixmap);
		void Draw (const Image& image);
	};
}

#endif
