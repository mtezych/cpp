
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, mtezych
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef __gnu_linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

// @todo: Enable transparency by creating TrueColor Colormap.
// https://gist.github.com/je-so/903479/834dfd78705b16ec5f7bbd10925980ace4049e17

/*
 * Ch. Tronche Tutorial:    https://tronche.com/gui/x/xlib
 * Xlib Programming Manual: https://www.niksula.hut.fi/~jkirma/books/xlib.pdf
 * Xorg reference:          https://www.x.org/docs/X11/xlib.pdf
 *
 * X server side objects:
 *
 *  - Display   ->  connection to X server running on
 *                  particular machine with keyboard, mouse and screens
 *  - Screen    ->  physical monitor
 *  - Visual    ->  way in which screen interprets colors
 *  - GC        ->  graphics context (drawing settings)
 *  - Window    ->   on-screen rendering area
 *  - Pixmap    ->  off-screen rendering area
 *  - Drawable  ->  Window or Pixmap (on-screen or off-screen rendering area)
 *  - Bitmap    ->  monochrome Pixmap (its depth is equal 1)
 *
 * X client side objects:
 *
 *  - XImage    ->  bitmap image
 */

#include <cassert>
#include <type_traits>
#include <limits>
#include <cstdint>
#include <cstdlib>
#include <vector>

namespace utility
{
	template <typename ArithmeticType>
	struct Size
	{
		static_assert
		(
			std::is_arithmetic<ArithmeticType>::value,
			"Arithmetic type is required."
		);
		ArithmeticType width;
		ArithmeticType height;
	};
}

namespace platform
{
	struct Display
	{
		::Display* xDisplay;

		Display (const char* name = nullptr)
		:
			xDisplay { XOpenDisplay(name) }
		{
			assert(xDisplay != None);

#ifndef NDEBUG
			XSynchronize(xDisplay, True);
#endif
		}

		~Display ()
		{
			if (xDisplay != None)
			{
				XCloseDisplay(xDisplay);
			}
		}

		Display (Display&& display)
		:
			xDisplay { display.xDisplay }
		{
			display.xDisplay = None;
		}

		Display& operator = (Display&& display)
		{
			if (xDisplay != None)
			{
				XCloseDisplay(xDisplay);
			}

			xDisplay = display.xDisplay;
			display.xDisplay = None;

			return *this;
		}

		Display (const Display& display) = delete;

		Display& operator = (const Display& display) = delete;
	};

	struct Window;
	struct Pixmap;

	struct Image
	{
		::XImage* xImage;

		// @todo: Create utility struct representing Image in CPU memory.
		Image
		(
			const Display& display,
			uint32_t width, uint32_t height, const std::vector<uint8_t>& bytes
		):
			xImage { None }
		{
			assert(bytes.size() == (width * height * 4));

			auto data = static_cast<char*>(malloc(bytes.size()));

			std::copy(bytes.begin(), bytes.end(), data);

			xImage = XCreateImage
			(
				display.xDisplay, CopyFromParent,
				24, ZPixmap,
				0,
				data, width, height,
				32, 0
			);
			assert(xImage != None);
		}

		~Image()
		{
			if (xImage != None)
			{
				XDestroyImage(xImage);
			}
		}

		Image (Image&& image)
		:
			xImage { image.xImage }
		{
			image.xImage = None;
		}

		Image& operator = (Image&& image)
		{
			if (xImage != None)
			{
				XDestroyImage(xImage);
			}

			xImage = image.xImage;
			image.xImage = None;

			return *this;
		}

		Image (const Image& image) = delete;

		Image& operator = (const Image& image) = delete;

		explicit Image (const Window& window);

		explicit Image (const Pixmap& pixmap);

		utility::Size<unsigned int> Size() const
		{
			return utility::Size<unsigned int>
			{
				static_cast<unsigned int>(xImage->width),
				static_cast<unsigned int>(xImage->height)
			};
		}
	};

	struct Pixmap
	{
		::Pixmap   xPixmap;
		::Display* xDisplay;
		::Screen*  xScreen;

		Pixmap(const Display& display, uint32_t width, uint32_t height)
		:
			xPixmap  { None },
			xDisplay { display.xDisplay },
			xScreen  { XDefaultScreenOfDisplay(xDisplay) }
		{
			assert(xDisplay != None);

			xPixmap = XCreatePixmap
			(
				xDisplay,
				XRootWindowOfScreen(xScreen),
				width, height,
				static_cast<unsigned int>(XDefaultDepthOfScreen(xScreen))
			);
		}

		~Pixmap()
		{
			if (xPixmap != None)
			{
				XFreePixmap(xDisplay, xPixmap);
			}
		}

		Pixmap (Pixmap&& pixmap)
		:
			xPixmap  { pixmap.xPixmap  },
			xDisplay { pixmap.xDisplay },
			xScreen  { pixmap.xScreen  }
		{
			pixmap.xPixmap  = None;
			pixmap.xDisplay = None;
			pixmap.xScreen  = None;
		}

		Pixmap& operator = (Pixmap&& pixmap)
		{
			if (xPixmap != None)
			{
				XFreePixmap(xDisplay, xPixmap);
			}

			xPixmap  = pixmap.xPixmap;
			xDisplay = pixmap.xDisplay;
			xScreen  = pixmap.xScreen;
			pixmap.xPixmap  = None;
			pixmap.xDisplay = None;
			pixmap.xScreen  = None;

			return *this;
		}

		Pixmap (const Pixmap& pixmap) = delete;

		Pixmap& operator = (const Pixmap& pixmap) = delete;

		::Pixmap NativeHandle()
		{
			return xPixmap;
		}

		utility::Size<unsigned int> Size() const
		{
			auto ignoredXID  = ::XID { None };
			auto ignoredSInt = 0;
			auto ignoredUint = 0u;

			auto width  = 0u;
			auto height = 0u;

			const auto status = Status
			{
				XGetGeometry
				(
					xDisplay, xPixmap,
					&ignoredXID, &ignoredSInt, &ignoredSInt,
					&width, &height,
					&ignoredUint, &ignoredUint
				)
			};
			assert(status != False);

			return utility::Size<unsigned int> { width, height };
		}

		void Draw(const Window& window);

		void Draw(const Image& image)
		{
			const auto  imageSize = image.Size();
			const auto pixmapSize = this->Size();

			XPutImage
			(
				xDisplay,
				xPixmap,
				XDefaultGCOfScreen(xScreen),
				image.xImage,
				0, 0, // source      (x, y)
				0, 0, // destination (x, y)
				std::min(pixmapSize.width , imageSize.width),
				std::min(pixmapSize.height, imageSize.height)
			);
		}
	};

	struct Window
	{
		::Window   xWindow;
		::Display* xDisplay;
		::Screen*  xScreen;

		Window (const Display& display, uint32_t width, uint32_t height)
		:
			xWindow  { None },
			xDisplay { display.xDisplay },
			xScreen  { XDefaultScreenOfDisplay(xDisplay) }
		{
			assert(xDisplay != None);
			assert(xScreen  != None);

			::Visual* xVisual = XDefaultVisualOfScreen(xScreen);
			assert(xVisual != None);

			auto attribs = XSetWindowAttributes { };
			attribs.border_pixel     = XWhitePixelOfScreen(xScreen);
			attribs.background_pixel = XBlackPixelOfScreen(xScreen);
			attribs.event_mask       = NoEventMask;

			xWindow = XCreateWindow
			(
				xDisplay,
				XRootWindowOfScreen(xScreen),
				0, 0, width, height, 0,
				XDefaultDepthOfScreen(xScreen),
				InputOutput,
				xVisual,
				CWBorderPixel | CWBackPixel | CWEventMask, &attribs
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

			::Atom deleteAtom = XInternAtom(xDisplay, "WM_DELETE_WINDOW", True);
			assert(deleteAtom != None);

			const auto status = Status
			{
				XSetWMProtocols(xDisplay, xWindow, &deleteAtom, 1)
			};
			assert(status != False);

			XMapRaised(xDisplay, xWindow);
		}

		~Window()
		{
			if (xWindow != None)
			{
				XDestroyWindow(xDisplay, xWindow);
			}
		}

		Window (Window&& window)
		:
			xWindow  { window.xWindow  },
			xDisplay { window.xDisplay },
			xScreen  { window.xScreen  }
		{
			window.xWindow  = None;
			window.xDisplay = None;
			window.xScreen  = None;
		}

		Window& operator = (Window&& window)
		{
			if (xWindow != None)
			{
				XDestroyWindow(xDisplay, xWindow);
			}

			xWindow  = window.xWindow;
			xDisplay = window.xDisplay;
			xScreen  = window.xScreen;
			window.xWindow  = None;
			window.xDisplay = None;
			window.xScreen  = None;

			return *this;
		}

		Window (const Window& window) = delete;

		Window& operator = (const Window& window) = delete;

		::Window NativeHandle()
		{
			return xWindow;
		}

		utility::Size<unsigned int> Size() const
		{
			auto attribs = XWindowAttributes { };

			auto status = Status
			{
				XGetWindowAttributes(xDisplay, xWindow, &attribs)
			};
			assert(status != False);

			return utility::Size<unsigned int>
			{
				static_cast<unsigned int>(attribs.width),
				static_cast<unsigned int>(attribs.height),
			};
		}

		bool HandleEvent(const XEvent& event) const
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
					const auto atomName = XGetAtomName
					(
						xDisplay, event.xclient.message_type
					);
					static_cast<void>(atomName);
					return false;
				}
				case MapNotify :
				default :
				{
					break;
				}
			}
			return true;
		}

		template <typename Callable>
		int RecieveMessages(const Callable& render) const
		{
			while (true)
			{
				if (XPending(xDisplay))
				{
					auto xEvent = XEvent { };
					XNextEvent(xDisplay, &xEvent);

					if (!HandleEvent(xEvent))
					{
						break;
					}
				}
				else
				{
					render();
				}
			}

			return 0;
		}

		void
		Clear(const uint8_t R, const uint8_t G, const uint8_t B,const uint8_t A)
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

		void Draw(const Image& image)
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
	};

	void Pixmap::Draw(const Window& window)
	{
		assert(window.xDisplay == xDisplay);
		assert(window.xScreen  == xScreen);

		const auto windowSize = window.Size();
		const auto pixmapSize = this->Size();

		XCopyArea
		(
			xDisplay,
			window.xWindow, xPixmap,
			XDefaultGCOfScreen(xScreen),
			0, 0, // source      (x, y)
			std::min(pixmapSize.width , windowSize.width ),
			std::min(pixmapSize.height, windowSize.height),
			0, 0  // destination (x, y)
		);
	}

	Image::Image (const Window& window)
	:
		xImage { None }
	{
		auto attribs = XWindowAttributes { };

		while (attribs.map_state != IsViewable)
		{
			const auto status = Status
			{
				XGetWindowAttributes(window.xDisplay, window.xWindow, &attribs)
			};
			assert(status != False);
		}

		const auto windowSize = window.Size();

		xImage = XGetImage
		(
			window.xDisplay, window.xWindow,
			0, 0, // (x, y)
			windowSize.width, windowSize.height,
			AllPlanes, ZPixmap
		);
		assert(xImage != None);
	}

	Image::Image (const Pixmap& pixmap)
	:
		xImage { None }
	{
		const auto pixmapSize = pixmap.Size();

		xImage = XGetImage
		(
			pixmap.xDisplay, pixmap.xPixmap,
			0, 0, // (x, y)
			pixmapSize.width, pixmapSize.height,
			AllPlanes, ZPixmap
		);
		assert(xImage != None);
	}
}

#include <chrono>
#include <thread>

int main()
{
	const auto status = Status { XInitThreads() };
	assert(status != False);

	const auto display = platform::Display { };

	const auto data = std::vector<uint8_t> // BGRA
	{
		0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00,
	};
	auto image = platform::Image { display, 3, 3, data };

	auto pixmap = platform::Pixmap { display, 512, 512 };
	auto savedPixmapImage = platform::Image { pixmap };
	pixmap.Draw(image);

	auto window = platform::Window { display, 512, 512 };
	const auto render = [&window, &image]()
	{
		window.Clear(0x00, 0x77, 0x00, 0x00);
		window.Draw(image);

		const auto savedWindowImage = platform::Image { window };

		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.RecieveMessages(render);

	pixmap.Draw(window);

	return 0;
}

#else

int main()
{
	return 0;
}

#endif
