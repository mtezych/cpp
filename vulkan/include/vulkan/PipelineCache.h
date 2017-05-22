
#ifndef VULKAN_PIPELINE_CACHE
#define VULKAN_PIPELINE_CACHE

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct PipelineCache
	{
		const Device*   device;
		VkPipelineCache vkPipelineCache;

		PipelineCache
		(
			const Device&                   device,
			const std::vector<const void*>& initialData
		);

		~PipelineCache ();

		PipelineCache (PipelineCache&& pipelineCache);
		PipelineCache (const PipelineCache& pipelineCache) = delete;

		PipelineCache& operator = (PipelineCache&& pipelineCache);
		PipelineCache& operator = (const PipelineCache& pipelineCache) = delete;
	};
}

#endif
