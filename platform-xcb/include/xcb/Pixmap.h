
#ifndef PLATFORM_XCB_PIXMAP
#define PLATFORM_XCB_PIXMAP

#include <xcb/Connection.h>

#include <util/vec.h>

namespace xcb
{
	struct Pixmap
	{
		const Connection* connection;
		xcb_pixmap_t      xcbPixmap;

		Pixmap (const Connection& connection, const util::uvec2& size);
		~Pixmap ();

		Pixmap (Pixmap&& pixmap);
		Pixmap (const Pixmap& pixmap) = delete;

		Pixmap& operator = (Pixmap&& pixmap);
		Pixmap& operator = (const Pixmap& pixmap) = delete;
	};
}

#endif
