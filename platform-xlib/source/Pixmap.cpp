
#include <xlib/Pixmap.h>

#include <xlib/Display.h>
#include <xlib/Image.h>
#include <xlib/Window.h>

#include <cassert>
#include <algorithm>

namespace xlib
{
	Pixmap::Pixmap (const Display& display, const util::uvec2& size)
	:
		Drawable { display },
		xPixmap  { None }
	{
		xPixmap = XCreatePixmap
		(
			xDisplay,
			XRootWindowOfScreen(xScreen),
			size.width, size.height,
			static_cast<unsigned int>(XDefaultDepthOfScreen(xScreen))
		);
		assert(xPixmap != None);

		xDrawable = xPixmap;
	}

	Pixmap::~Pixmap ()
	{
		if (xPixmap != None)
		{
			XFreePixmap(xDisplay, xPixmap);
		}
	}

	Pixmap::Pixmap (Pixmap&& pixmap)
	:
		Drawable { std::move(pixmap) },
		xPixmap  { pixmap.xPixmap    }
	{
		pixmap.xPixmap = None;
	}

	Pixmap& Pixmap::operator = (Pixmap&& pixmap)
	{
		if (xPixmap != None)
		{
			XFreePixmap(xDisplay, xPixmap);
		}

		Drawable::operator = (std::move(pixmap));

		xPixmap  = pixmap.xPixmap;
		pixmap.xPixmap  = None;

		return *this;
	}
}
