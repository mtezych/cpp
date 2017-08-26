
#ifndef PLATFORM_XCB_GRAPHICS_CONTEXT
#define PLATFORM_XCB_GRAPHICS_CONTEXT

#include <xcb/xcb.h>

namespace xcb
{
	struct Connection;
	struct Window;

	struct GraphicsContext
	{
		const Connection* connection;
		xcb_gcontext_t    xcbGraphicsContext;

		// @todo: GraphicsContext should be taking Drawable, not Window.
		GraphicsContext (const Connection& connection, const Window& window);

		~GraphicsContext ();

		GraphicsContext (GraphicsContext&& graphicsContext);
		GraphicsContext (const GraphicsContext& graphicsContext) = delete;

		GraphicsContext& operator = (GraphicsContext&& graphicsContext);
		GraphicsContext& operator = (const GraphicsContext& graphicsContext) = delete;
	};
}

#endif
