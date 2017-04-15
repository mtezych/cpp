
#include <xlib/Drawable.h>

#include <xlib/Display.h>
#include <xlib/Image.h>

#include <limits>

namespace xlib
{
	Drawable::Drawable (const Display& display)
	:
		xDrawable { None },
		xDisplay  { display.xDisplay },
		xScreen   { XDefaultScreenOfDisplay(xDisplay) }
	{
		assert(xDisplay  != None);
		assert(xScreen   != None);
	}

	Drawable::Drawable (Drawable&& drawable)
	:
		xDrawable { drawable.xDrawable },
		xDisplay  { drawable.xDisplay  },
		xScreen   { drawable.xScreen   }
	{
		drawable.xDrawable = None;
		drawable.xDisplay  = None;
		drawable.xScreen   = None;
	}

	Drawable& Drawable::operator = (Drawable&& drawable)
	{
		xDrawable = drawable.xDrawable;
		xDisplay  = drawable.xDisplay;
		xScreen   = drawable.xScreen;

		drawable.xDrawable = None;
		drawable.xDisplay  = None;
		drawable.xScreen   = None;

		return *this;
	}

	util::uvec2 Drawable::Size () const
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
				xDisplay, xDrawable,
				&ignoredXID, &ignoredSInt, &ignoredSInt,
				&width, &height,
				&ignoredUint, &ignoredUint
			)
		};
		assert(status != False);

		return util::uvec2 { width, height };
	}

	void Drawable::Clear (const util::vec4& color)
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

		const auto drawableSize = Size();
		XFillRectangle
		(
			xDisplay, xDrawable, XDefaultGCOfScreen(xScreen),
			0, 0, // (x, y)
			drawableSize.width,
			drawableSize.height
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

	void Drawable::Draw (const Drawable& drawable)
	{
		assert(drawable.xDisplay == xDisplay);
		assert(drawable.xScreen  == xScreen);

		const auto srcSize = drawable.Size();
		const auto dstSize =    this->Size();

		XCopyArea
		(
			xDisplay,
			drawable.xDrawable, // source      Drawable
			xDrawable,          // destination Drawable
			XDefaultGCOfScreen(xScreen),
			0, 0,               // source      (x, y)
			std::min(srcSize.width , dstSize.width ),
			std::min(srcSize.height, dstSize.height),
			0, 0                // destination (x, y)
		);
	}

	void Drawable::Draw (const Image& image)
	{
		const auto    imageSize = image.Size();
		const auto drawableSize = this->Size();

		XPutImage
		(
			xDisplay,
			xDrawable,
			XDefaultGCOfScreen(xScreen),
			image.xImage,
			0, 0, // source      (x, y)
			0, 0, // destination (x, y)
			std::min(drawableSize.width , imageSize.width),
			std::min(drawableSize.height, imageSize.height)
		);
	}
}