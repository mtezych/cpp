
#ifndef XLIB_DRAWABLE
#define XLIB_DRAWABLE

#include <X11/Xlib.h>

#include <util/vec.h>

#include <cstdint>

namespace xlib
{
	struct Display;
	struct Image;

	struct Drawable
	{
		::Drawable xDrawable;
		::Display* xDisplay;
		::Screen*  xScreen;

	protected:
		Drawable (const xlib::Display& display);
		~Drawable () = default;

	public:
		Drawable (Drawable&& drawable);
		Drawable (const Drawable& drawable) = delete;

		Drawable& operator = (Drawable&& drawable);
		Drawable& operator = (const Drawable& drawable) = delete;

		util::uvec2 Size () const;

		void Clear (const util::vec4& color);

		void Draw (const Drawable& drawable);
		void Draw (const Image& image);
	};
}

#endif
