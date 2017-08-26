
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

#include <egl/Surface.h>

#include <egl/Display.h>

#include <platform/Window.h>
#include <platform/Pixmap.h>

#include <cassert>

namespace egl
{
	Surface::Surface
	(
		const Display& display, const Config& config,
		const std::vector<EGLint>& attribs
	):
		eglSurface { EGL_NO_SURFACE     },
		eglDisplay { display.eglDisplay }
	{
		assert(display.eglDisplay == config.eglDisplay);

		assert(attribs.size() > 0);
		assert(attribs.back() == EGL_NONE);
		assert(attribs.size() % 2);

		eglSurface = eglCreatePbufferSurface
		(
			display.eglDisplay,
			config.eglConfig,
			attribs.data()
		);
		assert(eglSurface != EGL_NO_SURFACE);
		assert(eglGetError() == EGL_SUCCESS);
	}

	Surface::Surface
	(
		const Display& display, const Config& config,
		const platform::Window& window,
		const std::vector<EGLint>& attribs
	):
		eglSurface { EGL_NO_SURFACE     },
		eglDisplay { display.eglDisplay }
	{
		assert(display.eglDisplay == config.eglDisplay);

		assert(attribs.size() > 0);
		assert(attribs.back() == EGL_NONE);
		assert(attribs.size() % 2);

		const auto& eglNativeWindow = EGLNativeWindowType
		{
#if   defined(PLATFORM_XLIB)

			window.nativeWindow.xWindow

#elif defined(PLATFORM_XCB)

			window.nativeWindow.xcbWindow

#elif defined(PLATFORM_ANDROID)

			window.nativeWindow.NativeHandle()

#elif defined(PLATFORM_WINDOWS)

			window.nativeWindow.windowHandle

#endif
		};

		eglSurface = eglCreateWindowSurface
		(
			display.eglDisplay,
			config.eglConfig,
			eglNativeWindow,
			attribs.data()
		);
		assert(eglSurface != EGL_NO_SURFACE);
		assert(eglGetError() == EGL_SUCCESS);
	}

	Surface::Surface
	(
		const Display& display, const Config& config,
		const platform::Pixmap& pixmap,
		const std::vector<EGLint>& attribs
	):
		eglSurface { EGL_NO_SURFACE     },
		eglDisplay { display.eglDisplay }
	{
		assert(display.eglDisplay == config.eglDisplay);

		assert(attribs.size() > 0);
		assert(attribs.back() == EGL_NONE);
		assert(attribs.size() % 2);

		const auto& eglNativePixmap = EGLNativePixmapType
		{
#if   defined(PLATFORM_XLIB)

			pixmap.nativePixmap.xPixmap

#elif defined(PLATFORM_XCB)

			pixmap.nativePixmap.xcbPixmap

#elif defined(PLATFORM_ANDROID)

			pixmap.nativePixmap.NativeHandle()

#elif defined(PLATFORM_WINDOWS)

			pixmap.nativePixmap.bitmapHandle

#endif
		};

		eglSurface = eglCreatePixmapSurface
		(
			eglDisplay,
			config.eglConfig,
			eglNativePixmap,
			attribs.data()
		);
		assert(eglSurface != EGL_NO_SURFACE);
		assert(eglGetError() == EGL_SUCCESS);
	}

	Surface::~Surface ()
	{
		if (eglSurface != EGL_NO_SURFACE)
		{
			const auto eglResult = EGLBoolean
			{
				eglDestroySurface(eglDisplay, eglSurface)
			};
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}
	}

	Surface::Surface (Surface&& surface)
	:
		eglSurface { surface.eglSurface },
		eglDisplay { surface.eglDisplay }
	{
		surface.eglSurface = EGL_NO_SURFACE;
		surface.eglDisplay = EGL_NO_DISPLAY;
	}

	Surface& Surface::operator = (Surface&& surface)
	{
		if (eglSurface != EGL_NO_SURFACE)
		{
			const auto eglResult = EGLBoolean
			{
				eglDestroySurface(eglDisplay, eglSurface)
			};
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}

		eglSurface = surface.eglSurface;
		eglDisplay = surface.eglDisplay;
		surface.eglSurface = EGL_NO_SURFACE;
		surface.eglDisplay = EGL_NO_DISPLAY;

		return *this;
	}

	void Surface::SwapBuffers ()
	{
		const auto eglResult = eglSwapBuffers(eglDisplay, eglSurface);
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);
	}
}
