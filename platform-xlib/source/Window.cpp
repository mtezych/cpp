
#include <xlib/Window.h>

#include <xlib/Display.h>
#include <xlib/Image.h>
#include <xlib/Pixmap.h>

#include <X11/Xutil.h>

#include <cassert>
#include <limits>

namespace xlib
{
	Window::Window (const Display& display, const util::uvec2& size)
	:
		Drawable  { display },
		xWindow   { None },
		xColormap { None }
	{
		::Visual* xVisual = XDefaultVisualOfScreen(xScreen);
		assert(xVisual != None);

		xColormap = XCreateColormap
		(
			xDisplay, XRootWindowOfScreen(xScreen),
			xVisual, AllocNone
		);
		assert(xColormap != None);

		auto attribs = XSetWindowAttributes { };
		attribs.background_pixel = XBlackPixelOfScreen(xScreen);
		attribs.border_pixel     = XWhitePixelOfScreen(xScreen);
		attribs.colormap         = xColormap;
		attribs.event_mask       = NoEventMask;

		xWindow = XCreateWindow
		(
			xDisplay,
			XRootWindowOfScreen(xScreen),
			0, 0, size.width, size.height, 0,
			XDefaultDepthOfScreen(xScreen),
			InputOutput,
			xVisual,
			CWBorderPixel | CWBackPixel | CWColormap | CWEventMask, &attribs
		);
		assert(xWindow != None);

		xDrawable = xWindow;

		XSelectInput
		(
			xDisplay, xWindow,
			ExposureMask      | StructureNotifyMask |

			KeyPressMask      | EnterWindowMask     |
			KeyReleaseMask    | LeaveWindowMask     |

			ButtonPressMask   | PointerMotionMask   |
			ButtonReleaseMask |  ButtonMotionMask
		);

		XStoreName(xDisplay, xWindow, "Xlib Window Title");

		auto deleteWindowAtom = ::Atom
		{
			XInternAtom(xDisplay, "WM_DELETE_WINDOW", True)
		};
		assert(deleteWindowAtom != None);

		const auto status = Status
		{
			XSetWMProtocols(xDisplay, xWindow, &deleteWindowAtom, 1)
		};
		assert(status != False);

		XMapRaised(xDisplay, xWindow);
	}

	Window::~Window ()
	{
		if (xWindow != None)
		{
			XDestroyWindow(xDisplay, xWindow);
			XFreeColormap(xDisplay, xColormap);
		}
	}

	Window::Window (Window&& window)
	:
		Drawable  { std::move(window) },
		xWindow   { window.xWindow    },
		xColormap { window.xColormap  }
	{
		window.xWindow   = None;
		window.xColormap = None;
	}

	Window& Window::operator = (Window&& window)
	{
		if (xWindow != None)
		{
			XDestroyWindow(xDisplay, xWindow);
			XFreeColormap(xDisplay, xColormap);
		}

		Drawable::operator = (std::move(window));

		xWindow   = window.xWindow;
		xColormap = window.xColormap;
		window.xWindow   = None;
		window.xColormap = None;

		return *this;
	}

	bool Window::HandleEvent(const XEvent& event) const
	{
		switch (event.type)
		{
			case ButtonPress :
			{
				switch (event.xbutton.button)
				{
					case Button1 : // Left Mouse Button
					case Button2 : // Middle Mouse Button (Mouse Wheel)
					case Button3 : // Right Mouse Button
					default :
					{
					}
				}
				break;
			}
			case ButtonRelease :
			{
				break;
			}
			case MotionNotify :
			{
				if (event.xbutton.state & Button1Mask) // LMB
				{
				}
				else if (event.xbutton.state & Button2Mask) // MMB
				{
				}
				else if (event.xbutton.state & Button3Mask) // RMB
				{
				}
				break;
			}
			case EnterNotify :
			{
				if(event.xcrossing.state & ControlMask)
				{
				}
				break;
			}
			case LeaveNotify :
			{
				if(event.xcrossing.focus == True)
				{
				}
				break;
			}
			case KeyPress :
			case KeyRelease :
			{
				auto keyEvent = event.xkey;
				if (XLookupKeysym(&keyEvent, 0) == XK_Escape)
				{
					return false;
				}
				break;
			}
			case Expose :
			{
				if (event.xexpose.count == 0)
				{
					XClearWindow(xDisplay, xWindow);
				}
				break;
			}
			case ConfigureNotify :
			{
				const auto windowSize = util::ivec2
				{
					event.xconfigure.width,
					event.xconfigure.height
				};
				static_cast<void>(windowSize);
				break;
			}
			case ClientMessage :
			{
				const auto protocolsAtom = ::Atom
				{
					XInternAtom(xDisplay, "WM_PROTOCOLS", True)
				};
				const auto deleteWindowAtom = ::Atom
				{
					XInternAtom(xDisplay, "WM_DELETE_WINDOW", True)
				};
				if (event.xclient.message_type == protocolsAtom)
				{
					if (::Atom(event.xclient.data.l[0]) == deleteWindowAtom)
					{
						return false;
					}
				}
				break;
			}
			case MapNotify :
			default :
			{
				break;
			}
		}
		return true;
	}
}
