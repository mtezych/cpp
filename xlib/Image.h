
#ifndef XLIB_IMAGE
#define XLIB_IMAGE

#include <X11/Xlib.h>

#include <vec.h>

#include <cstdint>
#include <vector>

namespace xlib
{
	struct Display;
	struct Window;
	struct Pixmap;

	struct Image
	{
		::XImage* xImage;

		// @todo: Create utility struct representing Image in CPU memory.
		Image
		(
			const xlib::Display& display,
			const util::uvec2& size,
			const std::vector<uint8_t>& bytes
		);
		~Image();

		Image (Image&& image);
		Image (const Image& image) = delete;

		Image& operator = (Image&& image);
		Image& operator = (const Image& image) = delete;

		util::uvec2 Size() const;

		explicit Image (const xlib::Window& window);
		explicit Image (const xlib::Pixmap& pixmap);
	};
}

#endif
