
#include "Pixmap.h"

#include "Display.h"
#include "Image.h"
#include "Window.h"

#include <cassert>
#include <algorithm>

namespace xlib
{
	Pixmap::Pixmap (const Display& display, const util::uvec2& size)
	:
		xPixmap  { None },
		xDisplay { display.xDisplay },
		xScreen  { XDefaultScreenOfDisplay(xDisplay) }
	{
		assert(xDisplay != None);

		xPixmap = XCreatePixmap
		(
			xDisplay,
			XRootWindowOfScreen(xScreen),
			size.width, size.height,
			static_cast<unsigned int>(XDefaultDepthOfScreen(xScreen))
		);
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
		xPixmap  { pixmap.xPixmap  },
		xDisplay { pixmap.xDisplay },
		xScreen  { pixmap.xScreen  }
	{
		pixmap.xPixmap  = None;
		pixmap.xDisplay = None;
		pixmap.xScreen  = None;
	}

	Pixmap& Pixmap::operator = (Pixmap&& pixmap)
	{
		if (xPixmap != None)
		{
			XFreePixmap(xDisplay, xPixmap);
		}

		xPixmap  = pixmap.xPixmap;
		xDisplay = pixmap.xDisplay;
		xScreen  = pixmap.xScreen;
		pixmap.xPixmap  = None;
		pixmap.xDisplay = None;
		pixmap.xScreen  = None;

		return *this;
	}

	util::uvec2 Pixmap::Size() const
	{
		auto ignoredXID  = ::XID { None };
		auto ignoredSInt = 0;
		auto ignoredUint = 0u;

		auto width  = 0u;
		auto height = 0u;

		const auto status = Status
		{
			XGetGeometry
			(
				xDisplay, xPixmap,
				&ignoredXID, &ignoredSInt, &ignoredSInt,
				&width, &height,
				&ignoredUint, &ignoredUint
			)
		};
		assert(status != False);

		return util::uvec2 { width, height };
	}

	void Pixmap::Clear (const util::vec4& color)
	{
		static_cast<void>(color.alpha);

		constexpr auto scale = std::numeric_limits<unsigned short>::max();

		auto xColor = XColor { };
		xColor.red   = static_cast<unsigned short>(scale * color.red);
		xColor.green = static_cast<unsigned short>(scale * color.green);
		xColor.blue  = static_cast<unsigned short>(scale * color.blue);
		xColor.flags = DoRed | DoGreen | DoBlue;

		auto status = Status
		{
			XAllocColor(xDisplay, XDefaultColormapOfScreen(xScreen), &xColor)
		};
		assert(status != False);

		auto values = XGCValues { };
		status = XGetGCValues
		(
			xDisplay,
			XDefaultGCOfScreen(xScreen),
			GCForeground, &values
		);
		assert(status != False);

		XSetForeground
		(
			xDisplay, XDefaultGCOfScreen(xScreen), xColor.pixel
		);

		const auto pixmapSize = Size();

		XFillRectangle
		(
			xDisplay, xPixmap, XDefaultGCOfScreen(xScreen),
			0, 0, // (x, y)
			pixmapSize.width,
			pixmapSize.height
		);

		XSetForeground
		(
			xDisplay, XDefaultGCOfScreen(xScreen), values.foreground
		);

		XFreeColors
		(
			xDisplay, XDefaultColormapOfScreen(xScreen),
			&xColor.pixel, 1,
			0
		);
	}

	void Pixmap::Draw(const Window& window)
	{
		assert(window.xDisplay == xDisplay);
		assert(window.xScreen  == xScreen);

		const auto windowSize = window.Size();
		const auto pixmapSize = this->Size();

		XCopyArea
		(
			xDisplay,
			window.xWindow, xPixmap,
			XDefaultGCOfScreen(xScreen),
			0, 0, // source      (x, y)
			std::min(pixmapSize.width , windowSize.width ),
			std::min(pixmapSize.height, windowSize.height),
			0, 0  // destination (x, y)
		);
	}

	void Pixmap::Draw(const Image& image)
	{
		const auto  imageSize = image.Size();
		const auto pixmapSize = this->Size();

		XPutImage
		(
			xDisplay,
			xPixmap,
			XDefaultGCOfScreen(xScreen),
			image.xImage,
			0, 0, // source      (x, y)
			0, 0, // destination (x, y)
			std::min(pixmapSize.width , imageSize.width),
			std::min(pixmapSize.height, imageSize.height)
		);
	}
}
