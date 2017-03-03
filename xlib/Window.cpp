
#include "Window.h"

#include "Display.h"
#include "Image.h"

#include <X11/Xutil.h>

#include <cassert>
#include <limits>

namespace xlib
{
	Window::Window (const Display& display, const utility::Size<unsigned int>& size)
	:
		xWindow   { None },
		xDisplay  { display.xDisplay },
		xScreen   { XDefaultScreenOfDisplay(xDisplay) },
		xColormap { None }
	{
		assert(xDisplay != None);
		assert(xScreen  != None);

		::Visual* xVisual = XDefaultVisualOfScreen(xScreen);
		assert(xVisual != None);

		xColormap = XCreateColormap
		(
			xDisplay, XRootWindowOfScreen(xScreen),
			xVisual, AllocNone
		);
		assert(xColormap != None);

		auto attribs = XSetWindowAttributes { };
		attribs.border_pixel     = XWhitePixelOfScreen(xScreen);
		attribs.background_pixel = XBlackPixelOfScreen(xScreen);
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

	Window::~Window()
	{
		if (xWindow != None)
		{
			XDestroyWindow(xDisplay, xWindow);
			XFreeColormap(xDisplay, xColormap);
		}
	}

	Window::Window (Window&& window)
	:
		xWindow   { window.xWindow   },
		xDisplay  { window.xDisplay  },
		xScreen   { window.xScreen   },
		xColormap { window.xColormap }
	{
		window.xWindow   = None;
		window.xDisplay  = None;
		window.xScreen   = None;
		window.xColormap = None;
	}

	Window& Window::operator = (Window&& window)
	{
		if (xWindow != None)
		{
			XDestroyWindow(xDisplay, xWindow);
			XFreeColormap(xDisplay, xColormap);
		}

		xWindow   = window.xWindow;
		xDisplay  = window.xDisplay;
		xScreen   = window.xScreen;
		xColormap = window.xColormap;
		window.xWindow   = None;
		window.xDisplay  = None;
		window.xScreen   = None;
		window.xColormap = None;

		return *this;
	}

	utility::Size<unsigned int> Window::Size() const
	{
		auto attribs = XWindowAttributes { };

		auto status = Status
		{
			XGetWindowAttributes(xDisplay, xWindow, &attribs)
		};
		assert(status != False);

		return utility::Size<unsigned int>
		{
			static_cast<unsigned int>(attribs.width ),
			static_cast<unsigned int>(attribs.height),
		};
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
				const auto windowSize = utility::Size<int>
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

	void Window::Clear
	(
		const uint8_t R, const uint8_t G, const uint8_t B, const uint8_t A
	)
	{
		static_cast<void>(A);

		constexpr auto scale =	std::numeric_limits<unsigned short>::max() /
								std::numeric_limits<uint8_t>::max();

		auto xColor = XColor { };
		xColor.red   = scale * R;
		xColor.green = scale * G;
		xColor.blue  = scale * B;
		xColor.flags = DoRed | DoGreen | DoBlue;

		auto status = Status
		{
			XAllocColor(xDisplay,XDefaultColormapOfScreen(xScreen), &xColor)
		};
		assert(status != False);

		auto values = XGCValues { };
		status = XGetGCValues
		(
			xDisplay,
			XDefaultGCOfScreen(xScreen),
			GCForeground, &values
		);
		assert(status != False);

		XSetForeground
		(
			xDisplay, XDefaultGCOfScreen(xScreen), xColor.pixel
		);

		const auto windowSize = Size();

		XFillRectangle
		(
			xDisplay, xWindow, XDefaultGCOfScreen(xScreen),
			0, 0, // (x, y)
			windowSize.width,
			windowSize.height
		);

		XSetForeground
		(
			xDisplay, XDefaultGCOfScreen(xScreen), values.foreground
		);

		XFreeColors
		(
			xDisplay, XDefaultColormapOfScreen(xScreen),
			&xColor.pixel, 1,
			0
		);
	}

	void Window::Draw(const Image& image)
	{
		const auto  imageSize = image.Size();
		const auto windowSize = this->Size();

		XPutImage
		(
			xDisplay,
			xWindow,
			XDefaultGCOfScreen(xScreen),
			image.xImage,
			0, 0, // source      (x, y)
			0, 0, // destination (x, y)
			std::min(imageSize.width , windowSize.width ),
			std::min(imageSize.height, windowSize.height)
		);
	}
}
