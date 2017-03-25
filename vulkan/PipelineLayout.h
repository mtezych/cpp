
#ifndef VULKAN_PIPELINE_LAYOUT
#define VULKAN_PIPELINE_LAYOUT

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct PipelineLayout
	{
		const Device*    device;
		VkPipelineLayout vkPipelineLayout;

		PFN_vkCreatePipelineLayout  vkCreatePipelineLayout;
		PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;

		PipelineLayout();

		PipelineLayout
		(
			const Device&                             device,
			const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
			const std::vector<VkPushConstantRange>&   pushConstantRanges
		);

		~PipelineLayout();
	};
}

#endif
