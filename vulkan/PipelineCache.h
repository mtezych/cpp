
#ifndef VULKAN_PIPELINE_CACHE
#define VULKAN_PIPELINE_CACHE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct PipelineCache
	{
		const Device*   device;
		VkPipelineCache vkPipelineCache;

		PFN_vkCreatePipelineCache  vkCreatePipelineCache;
		PFN_vkDestroyPipelineCache vkDestroyPipelineCache;

		PipelineCache();

		PipelineCache
		(
			const Device&                   device,
			const std::vector<const void*>& initialData
		);

		~PipelineCache();
	};
}

#endif
