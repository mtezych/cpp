
#include "VulkanPipeline.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"
#include "VulkanPipelineCache.h"

#include <cassert>

namespace vk
{
	Pipeline::Pipeline()
	:
		device   { VK_NULL_HANDLE },
		pipeline { VK_NULL_HANDLE },

		bindPoint { VK_PIPELINE_BIND_POINT_MAX_ENUM },

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
		device   { device.device  },
		pipeline { VK_NULL_HANDLE },

		bindPoint { VK_PIPELINE_BIND_POINT_COMPUTE },

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
			device.device,
			cache.pipelineCache,
			1, &createInfo,
			nullptr,
			&pipeline
		);
		assert(result == VK_SUCCESS);
	}

	Pipeline::Pipeline
	(
		const Device&                       device,
		const PipelineCache&                cache,
		const VkGraphicsPipelineCreateInfo& createInfo
	):
		device   { device.device  },
		pipeline { VK_NULL_HANDLE },

		bindPoint { VK_PIPELINE_BIND_POINT_GRAPHICS },

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
			device.device,
			cache.pipelineCache,
			1, &createInfo,
			nullptr,
			&pipeline
		);
		assert(result == VK_SUCCESS);
	}

	Pipeline::~Pipeline()
	{
		if (pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device, pipeline, nullptr);
		}
	}
}