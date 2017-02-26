
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

#include "Context.h"

#include "Display.h"
#include "Surface.h"

#include <cassert>

namespace egl
{
	Context::Context (const Display& display)
	:
		eglContext { EGL_NO_CONTEXT     },
		eglDisplay { display.eglDisplay }
	{
	}

	Context::Context
	(
		const Display& display, const Config& config,
		const std::vector<EGLint>& attribs
	):
		Context(display, config, Context { display }, attribs)
	{
	}

	Context::Context
	(
		const Display& display, const Config& config,
		const Context& sharedContext,
		const std::vector<EGLint>& attribs
	):
		eglContext { EGL_NO_CONTEXT     },
		eglDisplay { display.eglDisplay }
	{
		assert(display.eglDisplay ==        config.eglDisplay);
		assert(display.eglDisplay == sharedContext.eglDisplay);

		assert(attribs.size() > 0);
		assert(attribs.back() == EGL_NONE);
		assert(attribs.size() % 2);

		eglContext = eglCreateContext
		(
			display.eglDisplay,
			config.eglConfig,
			sharedContext.eglContext,
			attribs.data()
		);
		assert(eglContext != EGL_NO_CONTEXT);
		assert(eglGetError() == EGL_SUCCESS);
	}

	Context::~Context ()
	{
		if (eglContext != EGL_NO_CONTEXT)
		{
			if (eglContext == eglGetCurrentContext())
			{
				const auto eglResult = eglMakeCurrent
				(
					eglDisplay,
					EGL_NO_SURFACE, EGL_NO_SURFACE,
					EGL_NO_CONTEXT
				);
				assert(eglResult == EGL_TRUE);
				assert(eglGetError() == EGL_SUCCESS);
			}

			const auto eglResult = EGLBoolean
			{
				eglDestroyContext(eglDisplay, eglContext)
			};
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}
	}

	Context::Context (Context&& context)
	:
		eglContext { context.eglContext },
		eglDisplay { context.eglDisplay }
	{
		context.eglContext = EGL_NO_CONTEXT;
		context.eglDisplay = EGL_NO_DISPLAY;
	}

	Context& Context::operator = (Context&& context)
	{
		if (eglContext != EGL_NO_CONTEXT)
		{
			if (eglContext == eglGetCurrentContext())
			{
				const auto eglResult = eglMakeCurrent
				(
					eglDisplay,
					EGL_NO_SURFACE, EGL_NO_SURFACE,
					EGL_NO_CONTEXT
				);
				assert(eglResult == EGL_TRUE);
				assert(eglGetError() == EGL_SUCCESS);
			}

			const auto eglResult = EGLBoolean
			{
				eglDestroyContext(eglDisplay, eglContext)
			};
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}

		eglContext = context.eglContext;
		eglDisplay = context.eglDisplay;
		context.eglContext = EGL_NO_CONTEXT;
		context.eglDisplay = EGL_NO_DISPLAY;

		return *this;
	}

	void
	Context::MakeCurrent (const Surface& drawSurface,const Surface& readSurface)
	{
		assert(eglDisplay == drawSurface.eglDisplay);
		assert(eglDisplay == readSurface.eglDisplay);

		const auto eglResult = eglMakeCurrent
		(
			eglDisplay,
			drawSurface.eglSurface,
			readSurface.eglSurface,
			eglContext
		);
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);
	}

	void Context::MakeCurrent (const Surface& surface)
	{
		MakeCurrent(surface, surface);
	}
}
