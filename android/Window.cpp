
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

#include "Window.h"

#include <vec.h>

#include <limits>

namespace android
{
	Window::Window(const util::uvec2& size)
	:
		surfaceComposerClient { nullptr },
		surfaceControl        { nullptr },
		surface               { nullptr }
	{
		auto status = android::status_t { android::UNKNOWN_ERROR };

		surfaceComposerClient = new android::SurfaceComposerClient { };
		assert(surfaceComposerClient != nullptr);

		status = surfaceComposerClient->initCheck();
		assert(status == android::NO_ERROR);

		surfaceControl = surfaceComposerClient->createSurface
		(
			android::String8 { "Android Surface Title" },
			size.width, size.height,
			android::PIXEL_FORMAT_RGBA_8888
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

		surface = surfaceControl->getSurface();
	}

	ANativeWindow* Window::NativeHandle() const
	{
		return surface.get();
	}

	void Window::Clear (const util::vec4& color)
	{
		auto buffer = ANativeWindow_Buffer { };

		auto result = surface->lock(&buffer, nullptr);
		assert(result == android::NO_ERROR);
		{
			assert(buffer.format == android::PIXEL_FORMAT_RGBA_8888);

			const auto data = static_cast<uint8_t*>(buffer.bits);

			for (auto y = int32_t { 0 }; y < buffer.height; ++y)
			{
				for (auto x = int32_t { 0 }; x < buffer.width; ++x)
				{
					constexpr auto scale = std::numeric_limits<uint8_t>::max();

					const auto sample = util::Vector4<uint8_t>
					{
						static_cast<uint8_t>(scale * color.red  ),
						static_cast<uint8_t>(scale * color.green),
						static_cast<uint8_t>(scale * color.blue ),
						static_cast<uint8_t>(scale * color.alpha),
					};

					data[y * buffer.stride * 4 + x * 4 + 0] = sample.red;
					data[y * buffer.stride * 4 + x * 4 + 1] = sample.green;
					data[y * buffer.stride * 4 + x * 4 + 2] = sample.blue;
					data[y * buffer.stride * 4 + x * 4 + 3] = sample.alpha;
				}
			}
		}
		result = surface->unlockAndPost();
		assert(result == android::NO_ERROR);
	}
}
