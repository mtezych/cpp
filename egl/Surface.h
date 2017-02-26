
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

#ifndef EGL_SURFACE
#define EGL_SURFACE

#include <EGL/egl.h>

#include <vector>

namespace egl
{
	struct Display;
	struct Config;

	struct Surface
	{
		EGLSurface eglSurface;
		EGLDisplay eglDisplay;

		Surface
		(
			const egl::Display& display, const Config& config,
			const std::vector<EGLint>& attribs
		);

		Surface
		(
			const egl::Display& display, const Config& config,
			const EGLNativeWindowType eglNativeWindow,
			const std::vector<EGLint>& attribs
		);

//		Surface
//		(
//			const egl::Display& display, const Config& config,
//			const EGLNativePixmapType eglNativePixmap,
//			const std::vector<EGLint>& attribs
//		);

		~Surface ();

		Surface (Surface&& surface);

		Surface& operator = (Surface&& surface);

		Surface (const Surface& surface) = delete;

		Surface& operator = (const Surface& surface) = delete;

		void SwapBuffers ();
	};
}

#endif
