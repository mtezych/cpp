
#include <vulkan/QueryPool.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	QueryPool::CreateInfo::CreateInfo
	(
		const VkQueryType queryType, const uint32_t queryCount
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			nullptr,
			0,
			queryType, queryCount,
			VkQueryPipelineStatisticFlags { 0 },
		}
	{
		assert(queryType != VK_QUERY_TYPE_PIPELINE_STATISTICS);
	}

	QueryPool::CreateInfo::CreateInfo
	(
		const VkQueryType queryType, const uint32_t queryCount,
		const VkQueryPipelineStatisticFlags pipelineStatistics
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			nullptr,
			0,
			queryType, queryCount,
			pipelineStatistics,
		}
	{
		assert(queryType == VK_QUERY_TYPE_PIPELINE_STATISTICS);
	}

	QueryPool::QueryPool
	(
		const Device& device, const CreateInfo& createInfo
	):
		device      { &device        },
		vkQueryPool { VK_NULL_HANDLE }
	{
		const auto result = device.vkCreateQueryPool
		(
			device.vkDevice, &createInfo.createInfo, nullptr, &vkQueryPool
		);
		assert(result == VK_SUCCESS);
	}

	QueryPool::~QueryPool ()
	{
		if (vkQueryPool != VK_NULL_HANDLE)
		{
			device->vkDestroyQueryPool(device->vkDevice, vkQueryPool, nullptr);
		}
	}

	QueryPool::QueryPool (QueryPool&& queryPool)
	:
		device      { queryPool.device      },
		vkQueryPool { queryPool.vkQueryPool }
	{
		queryPool.device      = nullptr;
		queryPool.vkQueryPool = VK_NULL_HANDLE;
	}

	QueryPool& QueryPool::operator = (QueryPool&& queryPool)
	{
		if (vkQueryPool != VK_NULL_HANDLE)
		{
			device->vkDestroyQueryPool(device->vkDevice, vkQueryPool, nullptr);
		}

		device      = queryPool.device;
		vkQueryPool = queryPool.vkQueryPool;

		queryPool.device      = nullptr;
		queryPool.vkQueryPool = VK_NULL_HANDLE;

		return *this;
	}
}
