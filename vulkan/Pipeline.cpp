
#include "Pipeline.h"

#include "Symbols.h"
#include "Device.h"
#include "PipelineCache.h"

#include <cassert>

namespace vk
{
	Pipeline::Pipeline()
	:
		device      { nullptr },
		vkPipeline  { VK_NULL_HANDLE },
		vkBindPoint { VK_PIPELINE_BIND_POINT_MAX_ENUM },

		vkCreateComputePipelines  { nullptr },
		vkCreateGraphicsPipelines { nullptr },
		vkDestroyPipeline         { nullptr }
	{
	}

	Pipeline::Pipeline
	(
		const Device&                      device,
		const PipelineCache&               cache,
		const VkComputePipelineCreateInfo& createInfo
	):
		device      { &device },
		vkPipeline  { VK_NULL_HANDLE },
		vkBindPoint { VK_PIPELINE_BIND_POINT_COMPUTE },

		vkCreateComputePipelines
		{
			device.LoadDeviceProcedure<symbol::vkCreateComputePipelines>()
		},
		vkCreateGraphicsPipelines
		{
			nullptr
		},
		vkDestroyPipeline
		{
			device.LoadDeviceProcedure<symbol::vkDestroyPipeline>()
		}
	{
		const auto result = vkCreateComputePipelines
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
		vkBindPoint { VK_PIPELINE_BIND_POINT_GRAPHICS },

		vkCreateComputePipelines
		{
			nullptr
		},
		vkCreateGraphicsPipelines
		{
			device.LoadDeviceProcedure<symbol::vkCreateGraphicsPipelines>()
		},
		vkDestroyPipeline
		{
			device.LoadDeviceProcedure<symbol::vkDestroyPipeline>()
		}
	{
		const auto result = vkCreateGraphicsPipelines
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
			vkDestroyPipeline(device->vkDevice, vkPipeline, nullptr);
		}
	}
}