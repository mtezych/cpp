
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

#ifndef PLATFORM_ANDROID_WINDOW
#define PLATFORM_ANDROID_WINDOW

#include <android/native_window.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

#include <util/vec.h>

#include <cassert>

// https://github.com/waynewolf/waynewolf.github.io
// https://source.android.com/devices/graphics/arch-sf-hwc.html

namespace android
{
	class Window
	{
	private:
		android::sp<android::SurfaceComposerClient> surfaceComposerClient;
		android::sp<android::SurfaceControl>        surfaceControl;
		android::sp<android::Surface>               surface;

	public:
		Window (const util::uvec2& size);

		Window (Window&& window) = default;
		Window (const Window& window) = delete;

		Window& operator = (Window&& window) = default;
		Window& operator = (const Window& window) = delete;

		ANativeWindow* NativeHandle () const;

		void Clear (const util::vec4& color);

		template <typename Callable>
		void ReceiveMessages (const Callable& render) const
		{
			while (true)
			{
				render();
			}
		}
	};
}

#endif
