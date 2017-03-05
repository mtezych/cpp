
#ifndef XLIB_PIXMAP
#define XLIB_PIXMAP

#include <X11/Xlib.h>

#include "Drawable.h"

#include <vec.h>

#include <cstdint>

namespace xlib
{
	struct Display;
	struct Window;
	struct Image;

	struct Pixmap : Drawable
	{
		::Pixmap   xPixmap;

		Pixmap(const xlib::Display& display, const util::uvec2& size);
		~Pixmap();

		Pixmap (Pixmap&& pixmap);
		Pixmap (const Pixmap& pixmap) = delete;

		Pixmap& operator = (Pixmap&& pixmap);
		Pixmap& operator = (const Pixmap& pixmap) = delete;
	};
}

#endif
