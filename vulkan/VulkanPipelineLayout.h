
#ifndef VULKAN_PIPELINE_LAYOUT
#define VULKAN_PIPELINE_LAYOUT

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct VulkanDevice;

	struct VulkanPipelineLayout
	{
		VkDevice        device;
		VkPipelineLayout pipelineLayout;

		PFN_vkCreatePipelineLayout  vkCreatePipelineLayout;
		PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;

		VulkanPipelineLayout();

		VulkanPipelineLayout
		(
			const VulkanDevice&                       device,
			const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
			const std::vector<VkPushConstantRange>&   pushConstantRanges
		);

		~VulkanPipelineLayout();
	};
}

#endif
