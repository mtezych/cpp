
#ifndef VULKAN_PIPELINE
#define VULKAN_PIPELINE

#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;
	struct PipelineCache;

	struct Pipeline
	{
		const Device*       device;
		VkPipeline          vkPipeline;
		VkPipelineBindPoint vkBindPoint;

		Pipeline
		(
			const Device&                      device,
			const PipelineCache&               cache,
			const VkComputePipelineCreateInfo& createInfo
		);

		Pipeline
		(
			const Device&                       device,
			const PipelineCache&                cache,
			const VkGraphicsPipelineCreateInfo& createInfo
		);

		~Pipeline();
	};
}

#endif
