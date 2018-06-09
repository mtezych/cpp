
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

// POSIX
#include <unistd.h>
#include <fcntl.h>

// libDRM
#include "libdrm/intel/intel_bufmgr.h"
#include "libdrm/include/drm/i915_drm.h"

// C++ Standard Library
#include <cassert>
#include <cstring>
#include <string>

// Utils
#include <util/util.h>

namespace intel
{
	struct BufferManager
	{
		drm_intel_bufmgr* intelBufferManager;

		BufferManager (const int gpuFileDescriptor, const int batchSize)
		:
			intelBufferManager { nullptr }
		{
			intelBufferManager = drm_intel_bufmgr_gem_init
			(
				gpuFileDescriptor,
				batchSize
			);
			assert(intelBufferManager != nullptr);
		}

		~BufferManager ()
		{
			if (intelBufferManager != nullptr)
			{
				drm_intel_bufmgr_destroy(intelBufferManager);
			}
		}

		BufferManager (BufferManager&& bufferManager)
		:
			intelBufferManager { bufferManager.intelBufferManager }
		{
			bufferManager.intelBufferManager = nullptr;
		}

		BufferManager (const BufferManager& bufferManager) = delete;

		BufferManager& operator = (BufferManager&& bufferManager)
		{
			if (intelBufferManager != nullptr)
			{
				drm_intel_bufmgr_destroy(intelBufferManager);
			}

			intelBufferManager = bufferManager.intelBufferManager;

			bufferManager.intelBufferManager = nullptr;

			return *this;
		}

		BufferManager& operator = (const BufferManager& bufferManager) = delete;
	};

	enum class Tiling : uint32_t
	{
		None = I915_TILING_NONE,
		X    = I915_TILING_X,
		Y    = I915_TILING_X,
	};

	enum class Swizzle : uint32_t
	{
		NONE     = I915_BIT_6_SWIZZLE_NONE,
		_9       = I915_BIT_6_SWIZZLE_9,
		_9_10    = I915_BIT_6_SWIZZLE_9_10,
		_9_11    = I915_BIT_6_SWIZZLE_9_11,
		_9_10_11 = I915_BIT_6_SWIZZLE_9_10_11,
		_9_17    = I915_BIT_6_SWIZZLE_9_17,
		_9_10_17 = I915_BIT_6_SWIZZLE_9_10_17,
	};

	struct TilingMode
	{
		Tiling  tiling;
		Swizzle swizzle;
	};

	struct BufferObject
	{
		drm_intel_bo* intelBufferObject;

		BufferObject
		(
			const BufferManager& bufferManager,
			const std::string&   name,
			const unsigned long  size,
			const unsigned int   alignment
		):
			intelBufferObject { nullptr }
		{
			intelBufferObject = drm_intel_bo_alloc
			(
				bufferManager.intelBufferManager,
				name.c_str(), size, alignment
			);
			assert(intelBufferObject != nullptr);
		}

		~BufferObject ()
		{
			if (intelBufferObject != nullptr)
			{
				drm_intel_bo_unreference(intelBufferObject);
			}
		}

		BufferObject (BufferObject&& bo)
		:
			intelBufferObject { bo.intelBufferObject }
		{
			bo.intelBufferObject = nullptr;
		}

		BufferObject (const BufferObject& bo) = delete;

		BufferObject& operator = (BufferObject&& bo)
		{
			if (intelBufferObject != nullptr)
			{
				drm_intel_bo_unreference(intelBufferObject);
			}

			intelBufferObject = bo.intelBufferObject;

			bo.intelBufferObject = nullptr;

			return *this;
		}

		BufferObject& operator = (const BufferObject& bo) = delete;

		void SetTiling (Tiling tiling, const std::uint32_t stride)
		{
			auto tilingValue = util::enum_cast(tiling);

			const auto result = drm_intel_bo_set_tiling
			(
				intelBufferObject,
				&tilingValue, stride
			);
			assert(result == 0);
		}

		TilingMode GetTiling () const
		{
			auto tiling  = std::uint32_t { };
			auto swizzle = std::uint32_t { };

			const auto result = drm_intel_bo_get_tiling
			(
				intelBufferObject,
				&tiling, &swizzle
			);
			assert(result == 0);

			return TilingMode
			{
				static_cast<Tiling >(tiling ),
				static_cast<Swizzle>(swizzle),
			};
		}

		void Map ()
		{
			const auto writeEnable = true;

			const auto result = drm_intel_bo_map(intelBufferObject, writeEnable);
			assert(result == 0);
		}

		void Unmap ()
		{
			const auto result = drm_intel_bo_unmap(intelBufferObject);
			assert(result == 0);
		}

		void MapGTT ()
		{
			const auto result = drm_intel_gem_bo_map_gtt(intelBufferObject);
			assert(result == 0);
		}

		void UnmapGTT ()
		{
			const auto result = drm_intel_gem_bo_unmap_gtt(intelBufferObject);
			assert(result == 0);
		}

		int ExportToPrimeFD ()
		{
			auto bufferFileDescriptor = int { -1 };

			const auto result = drm_intel_bo_gem_export_to_prime
			(
				intelBufferObject, &bufferFileDescriptor
			);
			assert(result == 0);
			assert(bufferFileDescriptor > 0);

			return bufferFileDescriptor;
		}

		std::uint32_t Flink ()
		{
			auto bufferName = uint32_t { };

			const auto result = drm_intel_bo_flink
			(
				intelBufferObject, &bufferName
			);
			assert(result == 0);

			return bufferName;
		}
	};

	struct Context
	{
		drm_intel_context* intelContext;

		explicit
		Context (const BufferManager& bufferManager)
		:
			intelContext { nullptr }
		{
			intelContext = drm_intel_gem_context_create
			(
				bufferManager.intelBufferManager
			);
			assert(intelContext != nullptr);
		}

		~Context ()
		{
			if (intelContext != nullptr)
			{
				drm_intel_gem_context_destroy(intelContext);
			}
		}

		Context (Context&& context)
		:
			intelContext { context.intelContext }
		{
			context.intelContext = nullptr;
		}

		Context (const Context& context) = delete;

		Context& operator = (Context&& context)
		{
			if (intelContext != nullptr)
			{
				drm_intel_gem_context_destroy(intelContext);
			}

			intelContext = context.intelContext;

			context.intelContext = nullptr;

			return *this;
		}

		Context& operator = (const Context& context) = delete;
	};
}

int main()
{
	auto gpuFD = open("/dev/dri/renderD128", O_RDWR); // "/dev/dri/card0"
	assert(gpuFD > 0);
	{
		const auto bufferManager = intel::BufferManager
		{
			gpuFD,
			2048 * 1024, // batchSize
		};

		const auto context = intel::Context { bufferManager };

		auto bo = intel::BufferObject
		{
			bufferManager,
			"buffer-example", // name
			1024 * 1024,      // size
			4096,             // alignment
		};

		bo.SetTiling(intel::Tiling::None, 1024 /* stride */);

		const auto [tiling, swizzle] = bo.GetTiling();

		bo.Map();
		// bo.MapGTT();
		{
			std::memset(bo.intelBufferObject->virt, 0xFF, bo.intelBufferObject->size);
		}
		// bo.UnmapGTT();
		bo.Unmap();

		auto bufferFD = bo.ExportToPrimeFD();
		{
		}
		const auto result = close(bufferFD);
		assert(result == 0);

		auto bufferName = bo.Flink();
	}
	auto result = close(gpuFD);
	assert(result == 0);

	return 0;
}
