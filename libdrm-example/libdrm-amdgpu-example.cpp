
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

// C++ Guideline Support Library
#include <gsl/span>

namespace amdgpu
{
	struct Version
	{
		std::uint32_t major;
		std::uint32_t minor;
	};

	struct Device
	{
		amdgpu_device_handle amdDevice;

		explicit
		Device (const int gpuFileDescriptor)
		:
			amdDevice { nullptr }
		{
			auto version = Version { };

			const auto result = amdgpu_device_initialize
			(
				gpuFileDescriptor,
				&version.major, &version.minor, &amdDevice
			);
			assert(result == 0);
			assert(amdDevice != nullptr);
		}

		~Device ()
		{
			if (amdDevice != nullptr)
			{
				const auto result = amdgpu_device_deinitialize(amdDevice);
				assert(result == 0);
			}
		}

		Device (Device&& device)
		:
			amdDevice { device.amdDevice }
		{
			device.amdDevice = nullptr;
		}

		Device (const Device& device) = delete;

		Device& operator = (Device&& device)
		{
			if (amdDevice != nullptr)
			{
				const auto result = amdgpu_device_deinitialize(amdDevice);
				assert(result == 0);
			}

			amdDevice = device.amdDevice;

			device.amdDevice = nullptr;

			return *this;
		}

		Device& operator = (const Device& device) = delete;
	};

	struct Context
	{
		amdgpu_context_handle amdContext;

		explicit
		Context (const Device& device)
		:
			amdContext { nullptr }
		{
			const auto result = amdgpu_cs_ctx_create
			(
				device.amdDevice, &amdContext
			);
			assert(result == 0);
			assert(amdContext != nullptr);
		}

		~Context ()
		{
			if (amdContext != nullptr)
			{
				const auto result = amdgpu_cs_ctx_free(amdContext);
				assert(result == 0);
			}
		}

		Context (Context&& context)
		:
			amdContext { context.amdContext }
		{
			context.amdContext = nullptr;
		}

		Context (const Context& context) = delete;

		Context& operator = (Context&& context)
		{
			if (amdContext != nullptr)
			{
				const auto result = amdgpu_cs_ctx_free(amdContext);
				assert(result == 0);
			}

			amdContext = context.amdContext;

			context.amdContext = nullptr;

			return *this;
		}

		Context& operator = (const Context& context) = delete;
	};

	struct BufferObject
	{
		amdgpu_bo_handle amdBufferObject;

		using AllocRequest = amdgpu_bo_alloc_request;

		BufferObject (const Device& device, AllocRequest allocRequest)
		:
			amdBufferObject { nullptr }
		{
			const auto result = amdgpu_bo_alloc
			(
				device.amdDevice,
				&allocRequest, &amdBufferObject
			);
			assert(result == 0);
			assert(amdBufferObject != nullptr);
		}

		~BufferObject ()
		{
			if (amdBufferObject != nullptr)
			{
				const auto result = amdgpu_bo_free(amdBufferObject);
				assert(result == 0);
			}
		}

		BufferObject (BufferObject&& bo)
		:
			amdBufferObject { bo.amdBufferObject }
		{
			bo.amdBufferObject = nullptr;
		}

		BufferObject (const BufferObject& bo) = delete;

		BufferObject& operator = (BufferObject&& bo)
		{
			if (amdBufferObject != nullptr)
			{
				const auto result = amdgpu_bo_free(amdBufferObject);
				assert(result == 0);
			}

			amdBufferObject = bo.amdBufferObject;

			bo.amdBufferObject = nullptr;

			return *this;
		}

		BufferObject& operator = (const BufferObject& bo) = delete;

		void* Map ()
		{
			void* ptr = nullptr;

			const auto result = amdgpu_bo_cpu_map(amdBufferObject, &ptr);
			assert(result == 0);

			return ptr;
		}

		void Unmap ()
		{
			const auto result = amdgpu_bo_cpu_unmap(amdBufferObject);
			assert(result == 0);
		}
	};

	struct BufferObjectList
	{
		amdgpu_bo_list_handle amdBufferObjectList;

		BufferObjectList
		(
			const Device& device,
			const gsl::span<BufferObject> bufferObjects,
			const gsl::span<std::uint8_t> priorities
		):
			amdBufferObjectList { nullptr }
		{
			assert(bufferObjects.size() == priorities.size());

			static_assert(sizeof(amdgpu_bo_handle) == sizeof(BufferObject), "");

			const auto result = amdgpu_bo_list_create
			(
				device.amdDevice,
				static_cast<uint32_t>(bufferObjects.size()),
				reinterpret_cast<amdgpu_bo_handle*>(bufferObjects.data()),
				priorities.data(),
				&amdBufferObjectList
			);
			assert(result == 0);
			assert(amdBufferObjectList != nullptr);
		}

		BufferObjectList
		(
			const Device& device,
			const gsl::span<BufferObject> bufferObjects
		):
			amdBufferObjectList { nullptr }
		{
			static_assert(sizeof(amdgpu_bo_handle) == sizeof(BufferObject), "");

			const auto result = amdgpu_bo_list_create
			(
				device.amdDevice,
				static_cast<uint32_t>(bufferObjects.size()),
				reinterpret_cast<amdgpu_bo_handle*>(bufferObjects.data()),
				nullptr,
				&amdBufferObjectList
			);
			assert(result == 0);
			assert(amdBufferObjectList != nullptr);
		}

		~BufferObjectList ()
		{
			if (amdBufferObjectList != nullptr)
			{
				const auto result = amdgpu_bo_list_destroy(amdBufferObjectList);
				assert(result == 0);
			}
		}

		BufferObjectList (BufferObjectList&& boList)
		:
			amdBufferObjectList { boList.amdBufferObjectList }
		{
			boList.amdBufferObjectList = nullptr;
		}

		BufferObjectList (const BufferObjectList& boList) = delete;

		BufferObjectList& operator = (BufferObjectList&& boList)
		{
			if (amdBufferObjectList != nullptr)
			{
				const auto result = amdgpu_bo_list_destroy(amdBufferObjectList);
				assert(result == 0);
			}

			amdBufferObjectList = boList.amdBufferObjectList;

			boList.amdBufferObjectList = nullptr;

			return *this;
		}

		BufferObjectList& operator = (const BufferObjectList& boList) = delete;
	};
}

int main()
{
	auto gpuFD = open("/dev/dri/renderD128", O_RDWR); // "/dev/dri/card0"
	assert(gpuFD > 0);
	{
		const auto device = amdgpu::Device { gpuFD };

		const auto context = amdgpu::Context { device };

		auto bo = amdgpu::BufferObject
		{
			device, amdgpu::BufferObject::AllocRequest
			{
				64,                     // size
				0,                      // alignment
				AMDGPU_GEM_DOMAIN_VRAM, // preferred heap
				0,                      // flags
			}
		};

		const auto boList = amdgpu::BufferObjectList { device, { &bo, 1 } };
	}
	auto result = close(gpuFD);
	assert(result == 0);

	return 0;
}
