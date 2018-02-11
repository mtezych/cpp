
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

#ifndef PLATFORM_ANDROID_PIXMAP
#define PLATFORM_ANDROID_PIXMAP

#include <ui/ANativeObjectBase.h>
#include <ui/PixelFormat.h>

#include <util/vec.h>

#include <vector>

// http://androidxref.com/7.1.1_r6/xref/frameworks/base/core/jni/com_google_android_gles_jni_EGLImpl.cpp#267
// https://android.googlesource.com/platform/frameworks/base/+/android-7.1.2_r36/core/jni/com_google_android_gles_jni_EGLImpl.cpp#267

namespace android
{
	class Pixmap
	{
	private:
		std::vector<uint8_t>        storage;
		mutable egl_native_pixmap_t androidPixmap;
		// @note:
		// Handles managing Android objects do not propagate const,
		// because EGL and Vulkan functions creating EGLSurface or VkSurface
		// are taking non-const pointers to Android objects.

	public:
		Pixmap (const util::uvec2& size);

		~Pixmap() = default;

		Pixmap (Pixmap&& pixmap) = default;
		Pixmap (const Pixmap& pixmap) = delete;

		Pixmap& operator = (Pixmap&& pixmap) = default;
		Pixmap& operator = (const Pixmap& pixmap) = delete;

		egl_native_pixmap_t* NativeHandle () const;
	};
}

#endif
