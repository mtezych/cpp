
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
#include <egl/Surface.h>
#include <egl/Context.h>

#include <cassert>

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
