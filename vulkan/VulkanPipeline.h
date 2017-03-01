
#ifndef VULKAN_PIPELINE
#define VULKAN_PIPELINE

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

namespace vk
{
	struct VulkanDevice;
	struct VulkanPipelineCache;

	struct VulkanPipeline
	{
		VkDevice   device;
		VkPipeline pipeline;

		VkPipelineBindPoint bindPoint;

		PFN_vkCreateComputePipelines  vkCreateComputePipelines;
		PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
		PFN_vkDestroyPipeline         vkDestroyPipeline;

		VulkanPipeline();

		VulkanPipeline
		(
			const VulkanDevice&                device,
			const VulkanPipelineCache&         cache,
			const VkComputePipelineCreateInfo& createInfo
		);

		VulkanPipeline
		(
			const VulkanDevice&                 device,
			const VulkanPipelineCache&          cache,
			const VkGraphicsPipelineCreateInfo& createInfo
		);

		~VulkanPipeline();
	};
}

#endif
