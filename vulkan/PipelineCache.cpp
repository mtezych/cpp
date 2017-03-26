
#include "PipelineCache.h"

#include "Symbols.h"
#include "Device.h"

#include <cassert>

namespace vk
{
	PipelineCache::PipelineCache()
	:
		device          { nullptr },
		vkPipelineCache { VK_NULL_HANDLE }
	{
	}

	PipelineCache::PipelineCache
	(
		const Device&                   device,
		const std::vector<const void*>& initialData
	):
		device          { &device },
		vkPipelineCache { VK_NULL_HANDLE }
	{
		const auto createInfo = VkPipelineCacheCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			nullptr,
			0,
			initialData.size(), initialData.data()
		};
		const auto result = device.vkCreatePipelineCache
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkPipelineCache
		);
		assert(result == VK_SUCCESS);
	}

	PipelineCache::~PipelineCache()
	{
		if (vkPipelineCache != VK_NULL_HANDLE)
		{
			device->vkDestroyPipelineCache(device->vkDevice, vkPipelineCache, nullptr);
		}
	}
}
