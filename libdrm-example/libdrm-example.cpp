
// POSIX
#include <unistd.h>
#include <fcntl.h>

// libDRM
#include "libdrm/intel/intel_bufmgr.h"
#include "libdrm/include/drm/drm_fourcc.h"
#include "libdrm/include/drm/i915_drm.h"

// C++ Standard Library
#include <cassert>
#include <cstring>

int main()
{
	auto card0FD = open("/dev/dri/card0", O_RDWR);
	assert(card0FD != -1);
	{
		const auto batchSize = 2048 * 1024;
		auto drmIntelBufferManager = drm_intel_bufmgr_gem_init
		(
			card0FD,
			batchSize
		);
		assert(drmIntelBufferManager != nullptr);
		{
			const auto bufferSize = 1024ul * 1024ul;
			const auto bufferAlignment = 4096u;
			auto drmIntelBufferObject = drm_intel_bo_alloc
			(
				drmIntelBufferManager,
				"buffer-example",
				bufferSize,
				bufferAlignment
			);
			assert(drmIntelBufferObject != nullptr);
			{
				auto tilingMode = uint32_t{ I915_TILING_NONE };
				const auto bufferStride = uint32_t{ 1024 };
				auto result = drm_intel_bo_set_tiling
				(
					drmIntelBufferObject,
					&tilingMode,
					bufferStride
				);
				assert(result == 0);

				// result = drm_intel_gem_bo_map_gtt(drmIntelBufferObject);
				result = drm_intel_bo_map(drmIntelBufferObject, true);
				assert(result == 0);
				{
					std::memset(drmIntelBufferObject->virt, 0xFF, bufferSize);
				}
				result = drm_intel_bo_unmap(drmIntelBufferObject);
				// result = drm_intel_gem_bo_unmap_gtt(drmIntelBufferObject);
				assert(result == 0);

				auto bufferFD = int { -1 };
				result = drm_intel_bo_gem_export_to_prime
				(
					drmIntelBufferObject,
					&bufferFD
				);
				assert(bufferFD == -1);
				assert(result == 0);

				auto bufferName = uint32_t{};
				result = drm_intel_bo_flink(drmIntelBufferObject, &bufferName);
				assert(result == 0);
			}
			drm_intel_bo_unreference(drmIntelBufferObject);
		}
		drm_intel_bufmgr_destroy(drmIntelBufferManager);
	}
	auto result = close(card0FD);
	assert(result == 0);

	return 0;
}
