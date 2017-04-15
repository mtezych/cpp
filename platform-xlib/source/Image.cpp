
#include <xlib/Image.h>

#include <xlib/Display.h>
#include <xlib/Window.h>
#include <xlib/Pixmap.h>

#include <X11/Xutil.h>

#include <cassert>
#include <algorithm>

namespace xlib
{
	Image::Image
	(
		const Display& display,
		const util::uvec2& size,
		const std::vector<uint8_t>& bytes
	):
		xImage { None }
	{
		assert(bytes.size() == (size.width * size.height * 4));

		auto data = static_cast<char*>(malloc(bytes.size()));

		std::copy(bytes.begin(), bytes.end(), data);

		xImage = XCreateImage
		(
			display.xDisplay, CopyFromParent,
			24, ZPixmap,
			0,
			data, size.width, size.height,
			32, 0
		);
		assert(xImage != None);
	}

	Image::~Image()
	{
		if (xImage != None)
		{
			XDestroyImage(xImage);
		}
	}

	Image::Image (Image&& image)
	:
		xImage { image.xImage }
	{
		image.xImage = None;
	}

	Image& Image::operator = (Image&& image)
	{
		if (xImage != None)
		{
			XDestroyImage(xImage);
		}

		xImage = image.xImage;
		image.xImage = None;

		return *this;
	}

	util::uvec2 Image::Size() const
	{
		return util::uvec2
		{
			static_cast<unsigned int>(xImage->width),
			static_cast<unsigned int>(xImage->height)
		};
	}

	Image::Image (const Window& window)
	:
		xImage { None }
	{
		auto attribs = XWindowAttributes { };

		while (attribs.map_state != IsViewable)
		{
			const auto status = Status
			{
				XGetWindowAttributes(window.xDisplay, window.xWindow, &attribs)
			};
			assert(status != False);
		}

		const auto windowSize = window.Size();

		xImage = XGetImage
		(
			window.xDisplay, window.xWindow,
			0, 0, // (x, y)
			windowSize.width, windowSize.height,
			AllPlanes, ZPixmap
		);
		assert(xImage != None);
	}

	Image::Image (const Pixmap& pixmap)
	:
		xImage { None }
	{
		const auto pixmapSize = pixmap.Size();

		xImage = XGetImage
		(
			pixmap.xDisplay, pixmap.xPixmap,
			0, 0, // (x, y)
			pixmapSize.width, pixmapSize.height,
			AllPlanes, ZPixmap
		);
		assert(xImage != None);
	}
}
