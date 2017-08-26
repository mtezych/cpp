
#include <xcb/GraphicsContext.h>

#include <xcb/Connection.h>
#include <xcb/Window.h>

#include <cassert>

namespace xcb
{
	GraphicsContext::GraphicsContext (const Connection& connection, const Window& window)
	:
		connection         { &connection },
		xcbGraphicsContext { xcb_generate_id(connection.xcbConnection) }
	{
		assert(xcbGraphicsContext != XCB_NONE);

		const auto valueMask = uint32_t
		{
				XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES
		};
		uint32_t valueList[2] =
		{
			connection.xcbScreen->white_pixel,
			connection.xcbScreen->black_pixel,
		};

		xcb_create_gc
		(
			connection.xcbConnection,
			xcbGraphicsContext,
			xcb_drawable_t { window.xcbWindow },
			valueMask, valueList
		);
	}

	GraphicsContext::~GraphicsContext ()
	{
		if (xcbGraphicsContext != XCB_NONE)
		{
			xcb_free_gc(connection->xcbConnection, xcbGraphicsContext);
		}
	}

	GraphicsContext::GraphicsContext (GraphicsContext&& graphicsContext)
	:
		connection         { graphicsContext.connection },
		xcbGraphicsContext { graphicsContext.xcbGraphicsContext }
	{
		graphicsContext.connection          = nullptr;
		graphicsContext.xcbGraphicsContext  = XCB_NONE;
	}

	GraphicsContext& GraphicsContext::operator = (GraphicsContext&& graphicsContext)
	{
		if (xcbGraphicsContext != XCB_NONE)
		{
			xcb_free_gc(connection->xcbConnection, xcbGraphicsContext);
		}

		connection         = graphicsContext.connection;
		xcbGraphicsContext = graphicsContext.xcbGraphicsContext;

		graphicsContext.connection         = nullptr;
		graphicsContext.xcbGraphicsContext = XCB_NONE;

		return *this;
	}
}
