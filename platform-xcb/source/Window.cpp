
#include <xcb/Window.h>

#include <xcb/Connection.h>

#include <cstdlib>
#include <cassert>

namespace xcb
{
	Window::Event::Event (xcb_generic_event_t* const xcbEvent)
	:
		xcbEvent { xcbEvent }
	{
	}

	Window::Event::~Event ()
	{
		if (xcbEvent != nullptr)
		{
			std::free(xcbEvent);
		}
	}

	Window::Event::Event (Event&& event)
	:
		xcbEvent { event.xcbEvent }
	{
		event.xcbEvent = nullptr;
	}

	Window::Event& Window::Event::operator = (Event&& event)
	{
		if (xcbEvent != nullptr)
		{
			std::free(xcbEvent);
		}

		xcbEvent = event.xcbEvent;

		event.xcbEvent = nullptr;

		return *this;
	}

	Window::Event::operator bool () const
	{
		return xcbEvent != nullptr;
	}


	Window::Atom::Atom (const Connection& connection, const std::string& atomName)
	:
		xcbAtomReply { nullptr }
	{
		const auto createAtomOnlyIfExists = uint8_t { false };

		const auto xcbAtomCookie = xcb_intern_atom
		(
			connection.xcbConnection,
			createAtomOnlyIfExists,
			static_cast<uint16_t>(atomName.length()), atomName.data()
		);

		xcbAtomReply = xcb_intern_atom_reply
		(
			connection.xcbConnection,
			xcbAtomCookie,
			nullptr
		);
		assert(xcbAtomReply != nullptr);
	}

	Window::Atom::~Atom ()
	{
		if (xcbAtomReply != nullptr)
		{
			std::free(xcbAtomReply);
		}
	}

	Window::Atom::Atom (Atom&& atom)
	:
		xcbAtomReply { atom.xcbAtomReply }
	{
		atom.xcbAtomReply = nullptr;
	}

	Window::Atom& Window::Atom::operator = (Atom&& atom)
	{
		if (xcbAtomReply != nullptr)
		{
			std::free(xcbAtomReply);
		}

		xcbAtomReply = atom.xcbAtomReply;

		atom.xcbAtomReply = nullptr;

		return *this;
	}


	Window::Window (const Connection& connection, const util::uvec2& size)
	:
		connection { &connection },
		xcbWindow  { xcb_generate_id(connection.xcbConnection) }
	{
		assert(xcbWindow != XCB_NONE);

		const auto valueMask = uint32_t
		{
			XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK
		};
		uint32_t valueList[] =
		{
			connection.xcbScreen->white_pixel,
			connection.xcbScreen->black_pixel,

			XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_STRUCTURE_NOTIFY |

			XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_ENTER_WINDOW     |
			XCB_EVENT_MASK_KEY_RELEASE    | XCB_EVENT_MASK_LEAVE_WINDOW     |

			XCB_EVENT_MASK_BUTTON_PRESS   | XCB_EVENT_MASK_POINTER_MOTION   |
			XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION
		};

		xcb_create_window
		(
			connection.xcbConnection,           // connection
			XCB_COPY_FROM_PARENT,               // depth
			xcbWindow,                          // window
			connection.xcbScreen->root,         // root window
			0, 0,                               // x, y
			static_cast<uint16_t>(size.width),  // width
			static_cast<uint16_t>(size.height), // height
			0,                                  // border
			XCB_WINDOW_CLASS_INPUT_OUTPUT,      // class
			connection.xcbScreen->root_visual,  // visual
			valueMask, valueList                // values
		);

		const auto title = std::string { "XCB Window Title" };
		xcb_change_property
		(
			connection.xcbConnection,
			XCB_PROP_MODE_REPLACE,
			xcbWindow,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8, // 8-bit array, because title is encoded in UTF-8
			static_cast<uint32_t>(title.length()), title.data()
		);

		const auto wmProtocols = Atom { connection, "WM_PROTOCOLS" };
		const auto wmDelete    = Atom { connection, "WM_DELETE_WINDOW" };
		xcb_change_property
		(
			connection.xcbConnection,
			XCB_PROP_MODE_REPLACE,
			xcbWindow,
			wmProtocols.xcbAtomReply->atom,
			XCB_ATOM_ATOM,
			32, // 32-bit array, because xcb_atom_t is an alias for uint32_t
			1, &wmDelete.xcbAtomReply->atom
		);

		xcb_map_window(connection.xcbConnection, xcbWindow);

		xcb_flush(connection.xcbConnection);
	}

	Window::~Window ()
	{
		if (xcbWindow != XCB_NONE)
		{
			xcb_destroy_window(connection->xcbConnection, xcbWindow);
		}
	}

	Window::Window (Window&& window)
	:
		connection { window.connection },
		xcbWindow  { window.xcbWindow  }
	{
		window.connection = nullptr;
		window.xcbWindow  = XCB_NONE;
	}

	Window& Window::operator = (Window&& window)
	{
		if (xcbWindow != XCB_NONE)
		{
			xcb_destroy_window(connection->xcbConnection, xcbWindow);
		}

		connection = window.connection;
		xcbWindow  = window.xcbWindow;

		window.connection = nullptr;
		window.xcbWindow  = XCB_NONE;

		return *this;
	}

	bool Window::HandleEvent (const Event& event) const
	{
		switch (event.xcbEvent->response_type & 0x7F)
		{
			case XCB_CONFIGURE_NOTIFY :
			{
				const auto configureNotifyEvent = event.CastTo<xcb_configure_notify_event_t>();
				const auto windowSize = util::ivec2
				{
					configureNotifyEvent.width,
					configureNotifyEvent.height,
				};
				static_cast<void>(windowSize);
				break;
			}
			case XCB_CLIENT_MESSAGE :
			{
				const auto wmProtocols = Atom { *connection, "WM_PROTOCOLS" };
				const auto wmDelete    = Atom { *connection, "WM_DELETE_WINDOW" };

				const auto clientMessageEvent = event.CastTo<xcb_client_message_event_t>();

				if (clientMessageEvent.type == wmProtocols.xcbAtomReply->atom)
				{
					if (clientMessageEvent.data.data32[0] == wmDelete.xcbAtomReply->atom)
					{
						return false;
					}
				}
				break;
			}
			case XCB_KEY_PRESS :
			default:
			{
				break;
			}
		}

		return true;
	}
}
