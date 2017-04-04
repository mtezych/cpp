
#include "Pipeline.h"

#include "Symbols.h"
#include "Device.h"
#include "PipelineCache.h"

#include <cassert>

namespace vk
{
	Pipeline::Pipeline
	(
		const Device&                      device,
		const PipelineCache&               cache,
		const VkComputePipelineCreateInfo& createInfo
	):
		device      { &device },
		vkPipeline  { VK_NULL_HANDLE },
		vkBindPoint { VK_PIPELINE_BIND_POINT_COMPUTE }
	{
		const auto result = device.vkCreateComputePipelines
		(
			device.vkDevice,
			cache.vkPipelineCache,
			1, &createInfo,
			nullptr,
			&vkPipeline
		);
		assert(result == VK_SUCCESS);
	}

	Pipeline::Pipeline
	(
		const Device&                       device,
		const PipelineCache&                cache,
		const VkGraphicsPipelineCreateInfo& createInfo
	):
		device      { &device },
		vkPipeline  { VK_NULL_HANDLE },
		vkBindPoint { VK_PIPELINE_BIND_POINT_GRAPHICS }
	{
		const auto result = device.vkCreateGraphicsPipelines
		(
			device.vkDevice,
			cache.vkPipelineCache,
			1, &createInfo,
			nullptr,
			&vkPipeline
		);
		assert(result == VK_SUCCESS);
	}

	Pipeline::~Pipeline()
	{
		if (vkPipeline != VK_NULL_HANDLE)
		{
			device->vkDestroyPipeline(device->vkDevice, vkPipeline, nullptr);
		}
	}
}
