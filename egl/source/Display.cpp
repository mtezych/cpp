
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

#include <egl/Display.h>

#include <cassert>

namespace egl
{
	Display::Display ()
	:
		eglDisplay { eglGetDisplay(EGL_DEFAULT_DISPLAY) }
	{
		assert(eglDisplay != EGL_NO_DISPLAY);
		assert(eglGetError() == EGL_SUCCESS);

		auto eglMajorVersion = EGLint { };
		auto eglMinorVersion = EGLint { };
		const auto eglResult = EGLBoolean
		{
			eglInitialize(eglDisplay, &eglMajorVersion, &eglMinorVersion)
		};
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);
	}

	Display::Display (const platform::Display& display)
	:
		eglDisplay { EGL_NO_DISPLAY }
	{
		const auto eglNativeDisplay = EGLNativeDisplayType
		{
#if   defined(PLATFORM_XLIB)

			display.xDisplay

#elif defined(PLATFORM_XCB)

			display.xcbConnection

#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_WINDOWS)

			EGL_DEFAULT_DISPLAY

#endif
		};

		eglDisplay = eglGetDisplay(eglNativeDisplay);
		assert(eglDisplay != EGL_NO_DISPLAY);
		assert(eglGetError() == EGL_SUCCESS);

		auto eglMajorVersion = EGLint { };
		auto eglMinorVersion = EGLint { };
		const auto eglResult = EGLBoolean
		{
			eglInitialize(eglDisplay, &eglMajorVersion, &eglMinorVersion)
		};
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);
	}

	Display::~Display ()
	{
		if (eglDisplay != EGL_NO_DISPLAY)
		{
			const auto eglResult = EGLBoolean { eglTerminate(eglDisplay) };
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}
	}

	Display::Display (Display&& display)
	:
		eglDisplay { display.eglDisplay }
	{
		display.eglDisplay = EGL_NO_DISPLAY;
	}

	Display& Display::operator = (Display&& display)
	{
		if (eglDisplay != EGL_NO_DISPLAY)
		{
			const auto eglResult = EGLBoolean { eglTerminate(eglDisplay) };
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}

		eglDisplay = display.eglDisplay;
		display.eglDisplay = EGL_NO_DISPLAY;

		return *this;
	}

	std::vector<Config> Display::Configs () const
	{
		auto configsCount = EGLint { };
		auto eglResult = EGLBoolean
		{
			eglGetConfigs(eglDisplay, nullptr, 0, &configsCount)
		};
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);

		auto eglConfigs = std::vector<EGLConfig>
		(
			size_t ( configsCount ), EGLConfig { }
		);
		eglResult = eglGetConfigs
		(
			eglDisplay,
			eglConfigs.data(), EGLint ( eglConfigs.size() ),
			&configsCount
		);
		assert(eglConfigs.size() == size_t ( configsCount ));
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);

		auto configs = std::vector<Config> { };

		for (const auto eglConfig : eglConfigs)
		{
			configs.emplace_back(*this, eglConfig);
		}

		return configs;
	}

	std::vector<Config>
	Display::Configs (const std::vector<EGLint>& requiredAttribs) const
	{
		assert(requiredAttribs.size() > 0);
		assert(requiredAttribs.back() == EGL_NONE);
		assert(requiredAttribs.size() % 2);

		auto configsCount = EGLint { };
		auto eglResult = eglChooseConfig
		(
			eglDisplay,
			requiredAttribs.data(),
			nullptr, 0,
			&configsCount
		);
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);

		auto eglConfigs = std::vector<EGLConfig>
		(
			size_t ( configsCount ), EGLConfig { }
		);
		eglResult = eglChooseConfig
		(
			eglDisplay,
			requiredAttribs.data(),
			eglConfigs.data(), EGLint ( eglConfigs.size() ),
			&configsCount
		);
		assert(eglConfigs.size() == size_t ( configsCount ));
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);

		auto configs = std::vector<Config> { };

		for (const auto eglConfig : eglConfigs)
		{
			configs.emplace_back(*this, eglConfig);
		}

		return configs;
	}

	void Display::BindAPI (const EGLenum eglAPI)
	{
		assert(eglAPI == EGL_OPENGL_API || eglAPI == EGL_OPENGL_ES_API);

		const auto eglResult = EGLBoolean { eglBindAPI(eglAPI) };
		assert(eglResult == EGL_TRUE);
		assert(eglGetError() == EGL_SUCCESS);
	}
}
