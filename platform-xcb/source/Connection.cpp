
#include <xcb/Connection.h>

#include <cassert>

namespace xcb
{
	Connection::Connection (const char* name)
	:
		xcbConnection { nullptr },
		xcbScreen     { nullptr }
	{
		auto preferredScreenNumber = 0;

		xcbConnection = xcb_connect(name, &preferredScreenNumber);
		assert(xcbConnection != nullptr);
		assert(xcb_connection_has_error(xcbConnection) == 0);

		const auto xcbSetup = xcb_get_setup(xcbConnection);
		assert(xcbSetup != nullptr);

		auto xcbScreenIt = xcb_setup_roots_iterator(xcbSetup);

		for (auto i = 0; i < preferredScreenNumber; ++i)
		{
			xcb_screen_next(&xcbScreenIt);
		}

		xcbScreen = xcbScreenIt.data;
		assert(xcbScreen != nullptr);
	}

	Connection::~Connection ()
	{
		if (xcbConnection != nullptr)
		{
			xcb_disconnect(xcbConnection);
		}
	}

	Connection::Connection (Connection&& connection)
	:
		xcbConnection { connection.xcbConnection },
		xcbScreen     { connection.xcbScreen     }
	{
		connection.xcbConnection = nullptr;
		connection.xcbScreen     = nullptr;
	}

	Connection& Connection::operator = (Connection&& connection)
	{
		if (xcbConnection != nullptr)
		{
			xcb_disconnect(xcbConnection);
		}

		xcbConnection = connection.xcbConnection;
		xcbScreen     = connection.xcbScreen;
		connection.xcbConnection = nullptr;
		connection.xcbScreen     = nullptr;

		return *this;
	}
}
