
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

namespace amdgpu
{
	struct Device
	{
		amdgpu_device_handle amdDeviceHandle;

		struct Version
		{
			std::uint32_t major;
			std::uint32_t minor;
		};

		explicit
		Device (const int gpuFileDescriptor)
		:
			amdDeviceHandle { nullptr }
		{
			auto version = Version { };

			const auto result = amdgpu_device_initialize
			(
				gpuFileDescriptor,
				&version.major, &version.minor, &amdDeviceHandle
			);
			assert(result == 0);
			assert(amdDeviceHandle != nullptr);
		}

		~Device ()
		{
			if (amdDeviceHandle != nullptr)
			{
				const auto result = amdgpu_device_deinitialize(amdDeviceHandle);
				assert(result == 0);
			}
		}

		Device (Device&& device)
		:
			amdDeviceHandle { device.amdDeviceHandle }
		{
			device.amdDeviceHandle = nullptr;
		}

		Device (const Device& device) = delete;

		Device& operator = (Device&& device)
		{
			if (amdDeviceHandle != nullptr)
			{
				const auto result = amdgpu_device_deinitialize(amdDeviceHandle);
				assert(result == 0);
			}

			amdDeviceHandle = device.amdDeviceHandle;

			device.amdDeviceHandle = nullptr;

			return *this;
		}

		Device& operator = (const Device& device) = delete;
	};

	struct Context
	{
		amdgpu_context_handle amdContextHandle;

		explicit
		Context (const Device& device)
		:
			amdContextHandle { nullptr }
		{
			const auto result = amdgpu_cs_ctx_create
			(
				device.amdDeviceHandle, &amdContextHandle
			);
			assert(result == 0);
			assert(amdContextHandle != nullptr);
		}

		~Context ()
		{
			if (amdContextHandle != nullptr)
			{
				const auto result = amdgpu_cs_ctx_free(amdContextHandle);
				assert(result == 0);
			}
		}

		Context (Context&& context)
		:
			amdContextHandle { context.amdContextHandle }
		{
			context.amdContextHandle = nullptr;
		}

		Context (const Context& context) = delete;

		Context& operator = (Context&& context)
		{
			if (amdContextHandle != nullptr)
			{
				const auto result = amdgpu_cs_ctx_free(amdContextHandle);
				assert(result == 0);
			}

			amdContextHandle = context.amdContextHandle;

			context.amdContextHandle = nullptr;

			return *this;
		}

		Context& operator = (const Context& context) = delete;
	};

	struct BufferObject
	{
		amdgpu_bo_handle amdBufferObjectHandle;

		using AllocRequest = amdgpu_bo_alloc_request;

		BufferObject (const Device& device, AllocRequest allocRequest)
		:
			amdBufferObjectHandle { nullptr }
		{
			const auto result = amdgpu_bo_alloc
			(
				device.amdDeviceHandle,
				&allocRequest, &amdBufferObjectHandle
			);
			assert(result == 0);
			assert(amdBufferObjectHandle != nullptr);
		}

		~BufferObject ()
		{
			if (amdBufferObjectHandle != nullptr)
			{
				const auto result = amdgpu_bo_free(amdBufferObjectHandle);
				assert(result == 0);
			}
		}

		BufferObject (BufferObject&& bo)
		:
			amdBufferObjectHandle { bo.amdBufferObjectHandle }
		{
			bo.amdBufferObjectHandle = nullptr;
		}

		BufferObject (const BufferObject& bo) = delete;

		BufferObject& operator = (BufferObject&& bo)
		{
			if (amdBufferObjectHandle != nullptr)
			{
				const auto result = amdgpu_bo_free(amdBufferObjectHandle);
				assert(result == 0);
			}

			amdBufferObjectHandle = bo.amdBufferObjectHandle;

			bo.amdBufferObjectHandle = nullptr;

			return *this;
		}

		BufferObject& operator = (const BufferObject& bo) = delete;

		void* Map ()
		{
			void* ptr = nullptr;

			const auto result = amdgpu_bo_cpu_map(amdBufferObjectHandle, &ptr);
			assert(result == 0);

			return ptr;
		}

		void Unmap ()
		{
			const auto result = amdgpu_bo_cpu_unmap(amdBufferObjectHandle);
			assert(result == 0);
		}
	};
}

int main()
{
	auto gpuFD = open("/dev/dri/renderD128", O_RDWR); // "/dev/dri/card0"
	assert(gpuFD > 0);
	{
		const auto device = amdgpu::Device { gpuFD };

		const auto context = amdgpu::Context { device };

		const auto bo = amdgpu::BufferObject
		{
			device, amdgpu::BufferObject::AllocRequest
			{
				64,                     // size
				0,                      // alignment
				AMDGPU_GEM_DOMAIN_VRAM, // preferred heap
				0,                      // flags
			}
		};
	}
	auto result = close(gpuFD);
	assert(result == 0);

	return 0;
}
