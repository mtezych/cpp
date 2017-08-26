
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

		// @todo: Finnish creating XCB Pixmap.
		// xcb_create_pixmap();
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
