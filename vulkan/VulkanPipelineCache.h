
#ifndef VULKAN_PIPELINE_CACHE
#define VULKAN_PIPELINE_CACHE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct VulkanDevice;

	struct VulkanPipelineCache
	{
		VkDevice        device;
		VkPipelineCache pipelineCache;

		PFN_vkCreatePipelineCache  vkCreatePipelineCache;
		PFN_vkDestroyPipelineCache vkDestroyPipelineCache;

		VulkanPipelineCache();

		VulkanPipelineCache
		(
			const VulkanDevice&             device,
			const std::vector<const void*>& initialData
		);

		~VulkanPipelineCache();
	};
}

#endif
