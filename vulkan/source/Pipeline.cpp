
#include <vulkan/Pipeline.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>
#include <vulkan/PipelineCache.h>

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

	Pipeline::~Pipeline ()
	{
		if (vkPipeline != VK_NULL_HANDLE)
		{
			device->vkDestroyPipeline(device->vkDevice, vkPipeline, nullptr);
		}
	}

	Pipeline::Pipeline (Pipeline&& pipeline)
	:
		device      { pipeline.device      },
		vkPipeline  { pipeline.vkPipeline  },
		vkBindPoint { pipeline.vkBindPoint }
	{
		pipeline.device      = nullptr;
		pipeline.vkPipeline  = VK_NULL_HANDLE;
		pipeline.vkBindPoint = VK_PIPELINE_BIND_POINT_MAX_ENUM;
	}

	Pipeline& Pipeline::operator = (Pipeline&& pipeline)
	{
		if (vkPipeline != VK_NULL_HANDLE)
		{
			device->vkDestroyPipeline(device->vkDevice, vkPipeline, nullptr);
		}

		device      = pipeline.device;
		vkPipeline  = pipeline.vkPipeline;
		vkBindPoint = pipeline.vkBindPoint;

		pipeline.device      = nullptr;
		pipeline.vkPipeline  = VK_NULL_HANDLE;
		pipeline.vkBindPoint = VK_PIPELINE_BIND_POINT_MAX_ENUM;

		return *this;
	}
}
