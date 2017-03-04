
#ifndef XLIB_PIXMAP
#define XLIB_PIXMAP

#include <X11/Xlib.h>

#include <vec.h>

#include <cstdint>

namespace xlib
{
	struct Display;
	struct Window;
	struct Image;

	struct Pixmap
	{
		::Pixmap   xPixmap;
		::Display* xDisplay;
		::Screen*  xScreen;

		Pixmap(const xlib::Display& display, const util::uvec2& size);
		~Pixmap();

		Pixmap (Pixmap&& pixmap);
		Pixmap (const Pixmap& pixmap) = delete;

		Pixmap& operator = (Pixmap&& pixmap);
		Pixmap& operator = (const Pixmap& pixmap) = delete;

		util::uvec2 Size() const;

		void Clear (const util::vec4& color);

		void Draw (const xlib::Window& window);
		void Draw (const xlib::Image& image);
	};
}

#endif
