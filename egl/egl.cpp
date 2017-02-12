
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

#include <EGL/egl.h>

#include <cassert>
#include <vector>

namespace egl
{
	struct Display;

	struct Config
	{
		EGLConfig  eglConfig;
		EGLDisplay eglDisplay;

		Config (const Display& display, const EGLConfig eglConfig);

		~Config () = default;

		Config (const Config&) = default;
		Config (Config&&)      = default;

		Config& operator = (const Config&) = default;
		Config& operator = (Config&&)      = default;

		EGLint Attribute (const EGLint attribute) const
		{
			auto value = EGLint { };

			const auto eglResult = EGLBoolean
			{
				eglGetConfigAttrib(eglDisplay, eglConfig, attribute, &value)
			};
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);

			return value;
		}
	};

	struct Display
	{
		EGLDisplay eglDisplay;

		Display (EGLNativeDisplayType eglNativeDisplay = EGL_DEFAULT_DISPLAY)
		:
			eglDisplay { eglGetDisplay(eglNativeDisplay) }
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

		~Display ()
		{
			if (eglDisplay != EGL_NO_DISPLAY)
			{
				const auto eglResult = EGLBoolean { eglTerminate(eglDisplay) };
				assert(eglResult == EGL_TRUE);
				assert(eglGetError() == EGL_SUCCESS);
			}
		}

		Display (Display&& display)
		:
			eglDisplay { display.eglDisplay }
		{
			display.eglDisplay = EGL_NO_DISPLAY;
		}

		Display& operator = (Display&& display)
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

		Display (const Display& display) = delete;

		Display& operator = (const Display& display) = delete;

		std::vector<Config> Configs () const
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
		Configs (const std::vector<EGLint>& requiredAttribs) const
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
	};

	Config::Config(const Display& display, const EGLConfig eglConfig)
	:
		eglConfig  {         eglConfig  },
		eglDisplay { display.eglDisplay }
	{
	}

//	enum SurfaceType
//	{
//		PBUFFER,
//		WINDOW,
//		PIXMAP,
//	};

	struct Surface
	{
		EGLSurface eglSurface;
		EGLDisplay eglDisplay;

		Surface
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

		Surface
		(
			const Display& display, const Config& config,
			const EGLNativeWindowType eglNativeWindow,
			const std::vector<EGLint>& attribs
		):
			eglSurface { EGL_NO_SURFACE     },
			eglDisplay { display.eglDisplay }
		{
			assert(display.eglDisplay == config.eglDisplay);

			assert(attribs.size() > 0);
			assert(attribs.back() == EGL_NONE);
			assert(attribs.size() % 2);

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

//		Surface
//		(
//			const Display& display, const Config& config,
//			const EGLNativePixmapType eglNativePixmap,
//			const std::vector<EGLint>& attribs
//		):
//			eglSurface { EGL_NO_SURFACE     },
//			eglDisplay { display.eglDisplay }
//		{
//			assert(display.eglDisplay == config.eglDisplay);
//
//			assert(attribs.size() > 0);
//			assert(attribs.back() == EGL_NONE);
//			assert(attribs.size() % 2);
//
//			eglCreatePixmapSurface
//			(
//				eglDisplay,
//				config.eglDisplay,
//				eglNativePixmap,
//				attribs.data()
//			);
//			assert(eglSurface != EGL_NO_SURFACE);
//			assert(eglGetError() == EGL_SUCCESS);
//		}

		~Surface ()
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

		Surface (Surface&& surface)
		:
			eglSurface { surface.eglSurface },
			eglDisplay { surface.eglDisplay }
		{
			surface.eglSurface = EGL_NO_SURFACE;
			surface.eglDisplay = EGL_NO_DISPLAY;
		}

		Surface& operator = (Surface&& surface)
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

		Surface (const Surface& surface) = delete;

		Surface& operator = (const Surface& surface) = delete;

		void SwapBuffers ()
		{
			const auto eglResult = eglSwapBuffers(eglDisplay, eglSurface);
			assert(eglResult == EGL_TRUE);
			assert(eglGetError() == EGL_SUCCESS);
		}
	};

	struct Context
	{
		EGLContext eglContext;
		EGLDisplay eglDisplay;

		Context (const Display& display)
		:
			eglContext { EGL_NO_CONTEXT     },
			eglDisplay { display.eglDisplay }
		{
		}

		Context
		(
			const Display& display, const Config& config,
			const std::vector<EGLint>& attribs
		):
			Context(display, config, Context { display }, attribs)
		{
		}

		Context
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

		~Context ()
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

		Context (Context&& context)
		:
			eglContext { context.eglContext },
			eglDisplay { context.eglDisplay }
		{
			context.eglContext = EGL_NO_CONTEXT;
			context.eglDisplay = EGL_NO_DISPLAY;
		}

		Context& operator = (Context&& context)
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

		Context (const Context& context) = delete;

		Context& operator = (const Context& context) = delete;

		void
		MakeCurrent (const Surface& drawSurface, const Surface& readSurface)
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

		void MakeCurrent (const Surface& surface)
		{
			MakeCurrent(surface, surface);
		}
	};
}

int main()
{
	const auto display = egl::Display { };

	for (const auto& config : display.Configs())
	{
		config.Attribute(EGL_CONFIG_CAVEAT);
	}

	const auto requiredAttribs = std::vector<EGLint>
	{
		EGL_SURFACE_TYPE    , EGL_PBUFFER_BIT    ,
		EGL_RENDERABLE_TYPE , EGL_OPENGL_ES2_BIT ,
		EGL_BUFFER_SIZE     , 32                 ,
		EGL_RED_SIZE        , 8                  ,
		EGL_GREEN_SIZE      , 8                  ,
		EGL_BLUE_SIZE       , 8                  ,
		EGL_ALPHA_SIZE      , 8                  ,
		EGL_DEPTH_SIZE      , 16                 ,
		EGL_STENCIL_SIZE    , 8                  ,
		EGL_SAMPLE_BUFFERS  , 1                  ,
		EGL_SAMPLES         , 4                  ,
		EGL_NONE
	};
	const auto configs = display.Configs(requiredAttribs);
	assert(configs.size() > 0);

	// const auto windowAttribs = std::vector<EGLint>
	// {
	// 	EGL_RENDER_BUFFER , EGL_SINGLE_BUFFER ,
	// 	EGL_RENDER_BUFFER , EGL_BACK_BUFFER   ,
	// };
	const auto pbufferAttribs = std::vector<EGLint>
	{
		EGL_WIDTH  , 512 ,
		EGL_HEIGHT , 512 ,
		EGL_NONE
	};
	auto surface = egl::Surface
	{
		display,
		configs.front(),
		pbufferAttribs
	};

	const auto contextAttribs = std::vector<EGLint>
	{
		EGL_CONTEXT_CLIENT_VERSION , 2 ,
		EGL_NONE
	};
	auto context = egl::Context
	{
		display,
		configs.front(),
		contextAttribs
	};

	context.MakeCurrent(surface);

	surface.SwapBuffers();

	return 0;
}
