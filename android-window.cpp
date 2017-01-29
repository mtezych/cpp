
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

#ifdef __ANDROID__

#include <cassert>

#include <EGL/egl.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

// https://github.com/waynewolf/waynewolf.github.io
// https://source.android.com/devices/graphics/arch-sf-hwc.html

class Window
{
	android::sp<android::SurfaceComposerClient> surfaceComposerClient;
	android::sp<android::SurfaceControl>        surfaceControl;
	android::sp<ANativeWindow>                  androidNativeWindow;

public:
	Window()
	:
		surfaceComposerClient { nullptr },
		surfaceControl        { nullptr },
		androidNativeWindow   { nullptr }
	{
		auto status = android::status_t { android::UNKNOWN_ERROR };

		surfaceComposerClient = new android::SurfaceComposerClient { };
		assert(surfaceComposerClient != nullptr);

		status = surfaceComposerClient->initCheck();
		assert(status == android::NO_ERROR);

		surfaceControl = surfaceComposerClient->createSurface
		(
			android::String8 { "Android Surface Title" },
			512, 512,
			android::PIXEL_FORMAT_RGB_888
		);
		assert(surfaceControl != nullptr);

		const auto validity = surfaceControl->isValid();
		assert(validity);

		android::SurfaceComposerClient::openGlobalTransaction();
		{
			// setting layer establishes Z order of surfaces
			// larger values bring surfaces to front
			status = surfaceControl->setLayer(0x7FFFFFFF);
			assert(status == android::NO_ERROR);

			status = surfaceControl->show();
			assert(status == android::NO_ERROR);
		}
		android::SurfaceComposerClient::closeGlobalTransaction();

		androidNativeWindow = surfaceControl->getSurface();
	}

	EGLNativeWindowType get()
	{
		return androidNativeWindow.get();
	}
};

int main()
{
	auto window = Window { };

	const auto eglNativeWindow = window.get();
	assert(eglNativeWindow != nullptr);

	return 0;
}

#else

int main()
{
	return 0;
}

#endif