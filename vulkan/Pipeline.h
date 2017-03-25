
#ifndef VULKAN_PIPELINE
#define VULKAN_PIPELINE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct Device;
	struct PipelineCache;

	struct Pipeline
	{
		VkDevice   device;
		VkPipeline pipeline;

		VkPipelineBindPoint bindPoint;

		PFN_vkCreateComputePipelines  vkCreateComputePipelines;
		PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
		PFN_vkDestroyPipeline         vkDestroyPipeline;

		Pipeline();

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
