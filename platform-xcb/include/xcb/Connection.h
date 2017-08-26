
#ifndef PLATFORM_XCB_CONNECTION
#define PLATFORM_XCB_CONNECTION

//
// XCB Tutorial:           https://xcb.freedesktop.org/tutorial
// XCB API:                https://xcb.freedesktop.org/PublicApi
// XCB Manual:             https://xcb.freedesktop.org/manual
// From Xlib to XCB Guide: https://xcb.freedesktop.org/xlibtoxcbtranslationguide
//

#include <xcb/xcb.h>

namespace xcb
{
	struct Connection
	{
		xcb_connection_t* xcbConnection;
		xcb_screen_t*     xcbScreen;

		explicit Connection (const char* name = nullptr);
		~Connection ();

		Connection (Connection&& connection);
		Connection (const Connection& connection) = delete;

		Connection& operator = (Connection&& connection);
		Connection& operator = (const Connection& connection) = delete;
	};
}

#endif
