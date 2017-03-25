
#include "PipelineLayout.h"

#include "Symbols.h"
#include "Device.h"

#include <cassert>

namespace vk
{
	PipelineLayout::PipelineLayout()
	:
		device           { nullptr },
		vkPipelineLayout { VK_NULL_HANDLE },

		vkCreatePipelineLayout  { nullptr },
		vkDestroyPipelineLayout { nullptr }
	{
	}

	PipelineLayout::PipelineLayout
	(
		const Device&                             device,
		const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
		const std::vector<VkPushConstantRange>&   pushConstantRanges
	):
		device           { &device },
		vkPipelineLayout { VK_NULL_HANDLE },

		vkCreatePipelineLayout
		{
			device.LoadDeviceProcedure<symbol::vkCreatePipelineLayout>()
		},
		vkDestroyPipelineLayout
		{
			device.LoadDeviceProcedure<symbol::vkDestroyPipelineLayout>()
		}
	{
		const auto createInfo = VkPipelineLayoutCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			nullptr,
			0,
			uint32_t ( descriptorSetLayouts.size() ),
			descriptorSetLayouts.empty() ? nullptr : descriptorSetLayouts.data(),
			uint32_t (   pushConstantRanges.size() ),
			pushConstantRanges.empty() ? nullptr : pushConstantRanges.data(),
		};
		const auto result = vkCreatePipelineLayout
		(
			device.vkDevice,
			&createInfo,
			nullptr,
			&vkPipelineLayout
		);
		assert(result == VK_SUCCESS);
	}

	PipelineLayout::~PipelineLayout()
	{
		if (vkPipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(device->vkDevice, vkPipelineLayout, nullptr);
		}
	}
}
