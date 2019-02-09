
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
#include <cstdint>

// C++ Guideline Support Library
#include <gsl/span>

// C++ Utility Library
#include <util/util.h>

namespace amdgpu
{
	struct Version
	{
		std::uint32_t major;
		std::uint32_t minor;
	};

	enum class Engine : unsigned
	{
		GFX        = AMDGPU_HW_IP_GFX,
		COMPUTE    = AMDGPU_HW_IP_COMPUTE,
		DMA        = AMDGPU_HW_IP_DMA,
		UVD        = AMDGPU_HW_IP_UVD,
		VCE        = AMDGPU_HW_IP_VCE,
		UVD_ENCODE = AMDGPU_HW_IP_UVD_ENC,
		VCN_DECODE = AMDGPU_HW_IP_VCN_DEC,
		VCN_ENCODE = AMDGPU_HW_IP_VCN_ENC,
	};

	struct Range
	{
		uint64_t start;
		uint64_t end;
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

		amdgpu_gpu_info QueryGpuInfo () const
		{
			auto gpuInfo = amdgpu_gpu_info { };

			const auto result = amdgpu_query_gpu_info(amdDevice, &gpuInfo);
			assert(result == 0);

			return gpuInfo;
		}

		amdgpu_heap_info
		QueryHeapInfo (const std::uint32_t heap, const std::uint32_t flags) const
		{
			assert((heap == AMDGPU_GEM_DOMAIN_VRAM) || (heap == AMDGPU_GEM_DOMAIN_GTT));
			assert((flags == AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED) || (flags == 0));

			auto heapInfo = amdgpu_heap_info { };

			const auto result = amdgpu_query_heap_info
			(
				amdDevice,
				heap, flags,
				&heapInfo
			);
			assert(result == 0);

			return heapInfo;
		}

		std::vector<drm_amdgpu_info_hw_ip>
		QueryHardwareInfo (const Engine engine) const
		{
			auto instanceCount = uint32_t { };

			auto result = amdgpu_query_hw_ip_count
			(
				amdDevice,
				util::enum_cast(engine), &instanceCount
			);
			assert(result == 0);

			auto hardwareInfo = std::vector<drm_amdgpu_info_hw_ip>
			{
				instanceCount, drm_amdgpu_info_hw_ip { }
			};

			for (auto instance = 0u; instance < instanceCount; ++instance)
			{
				result = amdgpu_query_hw_ip_info
				(
					amdDevice,
					util::enum_cast(engine), instance,
					&hardwareInfo[instance]
				);
				assert(result == 0);
			}

			return hardwareInfo;
		}

		Range QueryVirtualAddressRange () const
		{
			auto range = Range { };

			const auto result = amdgpu_va_range_query
			(
				amdDevice,
				amdgpu_gpu_va_range::amdgpu_gpu_va_range_general,
				&range.start, &range.end
			);
			assert(result == 0);

			return range;
		}
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

		/*
		 *          GPU Virtual Address Range     Buffer Object
		 *              +-----------------+    +----------------+
		 *              |                 |    |                | ^
		 *              |                 |    |                | | offset
		 *              |                 |    |                | v
		 *              |                 |   /|----------------|
		 *              |                 |  / |                | ^
		 *              |                 | /  |                | |
		 *              |                 |/   |                | | size
		 * address ---> |-----------------|    |                | |
		 *              |                 |    |                | v
		 *              |                 |   /|----------------|
		 *              |                 |  / |                |
		 *              |                 | /  +----------------+
		 *              |                 |/
		 *              |-----------------|
		 *              |                 |
		 *              |                 |
		 *              +-----------------+
		 */
		void MapGPU (const uint64_t offset, const uint64_t size, const uint64_t address)
		{
			constexpr auto flags = uint64_t { 0 };

			const auto result = amdgpu_bo_va_op
			(
				amdBufferObject,
				offset,
				size,
				address,
				flags,
				AMDGPU_VA_OP_MAP
			);
			assert(result == 0);
		}

		void UnmapGPU (const uint64_t offset, const uint64_t size, const uint64_t address)
		{
			constexpr auto flags = uint64_t { 0 };

			const auto result = amdgpu_bo_va_op
			(
				amdBufferObject,
				offset,
				size,
				address,
				flags,
				AMDGPU_VA_OP_UNMAP
			);
			assert(result == 0);
		}

		void* MapCPU ()
		{
			void* ptr = nullptr;

			const auto result = amdgpu_bo_cpu_map(amdBufferObject, &ptr);
			assert(result == 0);

			return ptr;
		}

		void UnmapCPU ()
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

	struct VirtualAddressRange
	{
		amdgpu_va_handle amdVirtualAddressRange;

		uint64_t baseAddress;

		struct AllocRequest
		{
			const uint64_t size;
			const uint64_t alignment;
			const uint64_t requiredBaseAddress;
			const uint64_t flags;
		};

		VirtualAddressRange (const Device& device, AllocRequest allocRequest)
		:
			amdVirtualAddressRange { nullptr },
			baseAddress { 0 }
		{
			assert((allocRequest.flags == AMDGPU_VA_RANGE_32_BIT) || (allocRequest.flags == 0));

			const auto result = amdgpu_va_range_alloc
			(
				device.amdDevice,
				amdgpu_gpu_va_range::amdgpu_gpu_va_range_general,
				allocRequest.size,
				allocRequest.alignment,
				allocRequest.requiredBaseAddress,
				&baseAddress,
				&amdVirtualAddressRange,
				allocRequest.flags
			);
			assert(result == 0);
			assert(amdVirtualAddressRange != nullptr);
		}

		~VirtualAddressRange ()
		{
			if (amdVirtualAddressRange != nullptr)
			{
				const auto result = amdgpu_va_range_free(amdVirtualAddressRange);
				assert(result == 0);
			}
		}

		VirtualAddressRange (VirtualAddressRange&& va)
		:
			amdVirtualAddressRange { va.amdVirtualAddressRange },
			baseAddress            { va.baseAddress            }
		{
			va.amdVirtualAddressRange = nullptr;
			va.baseAddress            = 0;
		}

		VirtualAddressRange (const VirtualAddressRange& va) = delete;

		VirtualAddressRange& operator = (VirtualAddressRange&& va)
		{
			if (amdVirtualAddressRange != nullptr)
			{
				const auto result = amdgpu_va_range_free(amdVirtualAddressRange);
				assert(result == 0);
			}

			amdVirtualAddressRange = va.amdVirtualAddressRange;
			baseAddress            = va.baseAddress;

			va.amdVirtualAddressRange = nullptr;
			va.baseAddress            = 0;

			return *this;
		}

		VirtualAddressRange& operator = (const VirtualAddressRange& va) = delete;
	};
}

/*
 * AMD Developer -> Developer Guides, Manuals & ISA Documents -> Open GPU Documentation
 *   https://developer.amd.com/resources/developer-guides-manuals
 *
 *    - [R500]                        -> 3D Programming Guide
 *       http://developer.amd.com/wordpress/media/2013/10/R5xx_Acceleration_v1.5.pdf
 *
 * ~ TeraScale
 *
 *    - [R600][R700]                  -> 3D Programming Guide
 *       https://developer.amd.com/wordpress/media/2013/10/R6xx_R7xx_3D.pdf
 *
 *    - [Evergreen]                   -> 3D Register Reference Guide
 *       https://developer.amd.com/wordpress/media/2013/10/evergreen_3D_registers_v2.pdf
 *
 *    - [Northern Islands]            -> 3D Register Reference Guide
 *       https://developer.amd.com/wordpress/media/2013/10/cayman_3D_registers_v2.pdf
 *
 *    - [Evergreen][Northern Islands] -> 3D Programming Guide
 *       https://developer.amd.com/wordpress/media/2013/10/evergreen_cayman_programming_guide.pdf
 *
 * ~ GCN (Graphics Core Next)
 *
 *    - [Southern Islands]            -> 3D/Compute Register Reference Guide
 *       https://developer.amd.com/wordpress/media/2013/10/SI_3D_registers.pdf
 *
 *    - [Sea Islands]                 -> 3D/Compute Register Reference Guide
 *       https://developer.amd.com/wordpress/media/2013/10/CIK_3D_registers_v2.pdf
 *
 *    - [Southern Islands]            -> 3D/Compute Programming Guide
 *       https://developer.amd.com/wordpress/media/2013/10/si_programming_guide_v2.pdf
 */

int main()
{
	auto gpuFD = open("/dev/dri/renderD128", O_RDWR); // "/dev/dri/card0"
	assert(gpuFD > 0);
	{
		const auto device = amdgpu::Device { gpuFD };

		const auto  gpuInfo = device.QueryGpuInfo();
		const auto heapInfo = device.QueryHeapInfo(AMDGPU_GEM_DOMAIN_VRAM, 0);
		const auto   hwInfo = device.QueryHardwareInfo(amdgpu::Engine::COMPUTE);
		const auto    range = device.QueryVirtualAddressRange();

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

		const auto vaRange = amdgpu::VirtualAddressRange
		{
			device, amdgpu::VirtualAddressRange::AllocRequest
			{
				64, // size
				0,  // alignment,
				0,  // required base address
				0,  // flags
			}
		};

		bo.MapGPU(/* offset */ 0, /* size */ 64, vaRange.baseAddress);
		{
			void* ptr = bo.MapCPU();
			{
			}
			bo.UnmapCPU();
		}
		bo.UnmapGPU(/* offset */ 0, /* size */ 64, vaRange.baseAddress);
	}
	auto result = close(gpuFD);
	assert(result == 0);

	return 0;
}
