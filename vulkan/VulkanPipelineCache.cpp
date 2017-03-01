
#include "VulkanPipelineCache.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"

#include <cassert>

namespace vk
{
	VulkanPipelineCache::VulkanPipelineCache()
	:
		device        { VK_NULL_HANDLE },
		pipelineCache { VK_NULL_HANDLE },

		vkCreatePipelineCache  { nullptr },
		vkDestroyPipelineCache { nullptr }
	{
	}

	VulkanPipelineCache::VulkanPipelineCache
	(
		const VulkanDevice&             device,
		const std::vector<const void*>& initialData
	):
		device        { device.device  },
		pipelineCache { VK_NULL_HANDLE },

		vkCreatePipelineCache
		{
			device.LoadDeviceProcedure<symbol::vkCreatePipelineCache>()
		},
		vkDestroyPipelineCache
		{
			device.LoadDeviceProcedure<symbol::vkDestroyPipelineCache>()
		}
	{
		const auto createInfo = VkPipelineCacheCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			nullptr,
			0,
			initialData.size(), initialData.data()
		};
		const auto result = vkCreatePipelineCache
		(
			device.device,
			&createInfo,
			nullptr,
			&pipelineCache
		);
		assert(result == VK_SUCCESS);
	}

	VulkanPipelineCache::~VulkanPipelineCache()
	{
		if (pipelineCache != VK_NULL_HANDLE)
		{
			vkDestroyPipelineCache(device, pipelineCache, nullptr);
		}
	}
}
