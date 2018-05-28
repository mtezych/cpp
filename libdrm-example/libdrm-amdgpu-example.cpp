
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2018, mtezych
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

// POSIX
#include <unistd.h>
#include <fcntl.h>

// libDRM
#include "libdrm/amdgpu/amdgpu.h"
#include "libdrm/include/drm/amdgpu_drm.h"

// C++ Standard Library
#include <cassert>
#include <cstring>
#include <cstdint>

int main()
{
	auto gpuFD = open("/dev/dri/renderD128", O_RDWR); // "/dev/dri/card0"
	assert(gpuFD > 0);
	{
		auto result = int { -1 };

		struct Version
		{
			std::uint32_t major;
			std::uint32_t minor;
		};
		auto version = Version { };

		auto amdDeviceHandle = amdgpu_device_handle { };

		result = amdgpu_device_initialize
		(
			gpuFD,
			&version.major, &version.minor,
			&amdDeviceHandle
		);
		assert(result == 0);
		{
			auto amdContextHandle = amdgpu_context_handle { };

			result = amdgpu_cs_ctx_create(amdDeviceHandle, &amdContextHandle);
			assert(result == 0);
			{
				auto allocRequest = amdgpu_bo_alloc_request
				{
					64,                                // alloc_size
					0,                                 // phys_alignment
					AMDGPU_GEM_DOMAIN_VRAM,            // preferred_heap
					AMDGPU_GEM_CREATE_VRAM_CONTIGUOUS, // flags
				};
				auto amdBufferObjectHandle = amdgpu_bo_handle { };

				result = amdgpu_bo_alloc
				(
					amdDeviceHandle,
					&allocRequest, &amdBufferObjectHandle
				);
				assert(result == 0);
				{

				}
				result = amdgpu_bo_free(amdBufferObjectHandle);
				assert(result == 0);
			}
			result = amdgpu_cs_ctx_free(amdContextHandle);
			assert(result == 0);
		}
		result = amdgpu_device_deinitialize(amdDeviceHandle);
		assert(result == 0);
	}
	auto result = close(gpuFD);
	assert(result == 0);

	return 0;
}
