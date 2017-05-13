
#ifndef VULKAN_PIPELINE_LAYOUT
#define VULKAN_PIPELINE_LAYOUT

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
	struct Device;

	struct PipelineLayout
	{
		const Device*    device;
		VkPipelineLayout vkPipelineLayout;

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
