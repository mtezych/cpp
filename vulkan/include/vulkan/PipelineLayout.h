
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

		~PipelineLayout ();

		PipelineLayout (PipelineLayout&& pipelineLayout);
		PipelineLayout (const PipelineLayout& pipelineLayout) = delete;

		PipelineLayout& operator = (PipelineLayout&& pipelineLayout);
		PipelineLayout& operator = (const PipelineLayout& pipelineLayout) = delete;
	};
}

#endif
