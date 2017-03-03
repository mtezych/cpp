
#ifndef XLIB_DISPLAY
#define XLIB_DISPLAY

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

#include <X11/Xlib.h>

namespace xlib
{
	struct Display
	{
		::Display* xDisplay;

		Display (const char* name = nullptr);
		~Display ();

		Display (Display&& display);
		Display (const Display& display) = delete;

		Display& operator = (Display&& display);
		Display& operator = (const Display& display) = delete;
	};
}

#endif
