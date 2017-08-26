
#include <xcb/Pixmap.h>

#include <xcb/Connection.h>

namespace xcb
{
	Pixmap::Pixmap (const Connection& connection, const util::uvec2& size)
	:
		connection { &connection },
		xcbPixmap  { xcb_generate_id(connection.xcbConnection) }
	{
		assert(xcbPixmap != XCB_NONE);

		xcb_create_pixmap
		(
			connection.xcbConnection,
			connection.xcbScreen->root_depth,
			xcbPixmap,
			xcb_drawable_t { connection.xcbScreen->root },
			static_cast<uint16_t>(size.width),
			static_cast<uint16_t>(size.height)
		);
	}

	Pixmap::~Pixmap ()
	{
		if (xcbPixmap != XCB_NONE)
		{
			xcb_free_pixmap(connection->xcbConnection, xcbPixmap);
		}
	}

	Pixmap::Pixmap (Pixmap&& pixmap)
	:
		connection { pixmap.connection },
		xcbPixmap  { pixmap.xcbPixmap  }
	{
		pixmap.connection = nullptr;
		pixmap.xcbPixmap  = XCB_NONE;
	}

	Pixmap& Pixmap::operator = (Pixmap&& pixmap)
	{
		if (xcbPixmap != XCB_NONE)
		{
			xcb_free_pixmap(connection->xcbConnection, xcbPixmap);
		}

		connection = pixmap.connection;
		xcbPixmap  = pixmap.xcbPixmap;

		pixmap.connection = nullptr;
		pixmap.xcbPixmap  = XCB_NONE;

		return *this;
	}
}
