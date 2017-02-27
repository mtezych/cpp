
#include "VulkanCommandPool.h"

#include "VulkanSymbols.h"
#include "VulkanDevice.h"

namespace vk
{
	VulkanCommandPool::VulkanCommandPool
	(
		const VulkanDevice& device, const uint32_t queueFamilyIndex
	):
		commandPool { VK_NULL_HANDLE },
		device      { device.device  },

		vkCreateCommandPool
		{
			device.LoadDeviceProcedure<symbol::vkCreateCommandPool>()
		},
		vkDestroyCommandPool
		{
			device.LoadDeviceProcedure<symbol::vkDestroyCommandPool>()
		}
	{
		const auto createInfo = VkCommandPoolCreateInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			0,
			queueFamilyIndex
		};
		const auto result = vkCreateCommandPool
		(
			device.device, &createInfo, nullptr, &commandPool
		);
		assert(result == VK_SUCCESS);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		if (commandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(device, commandPool, nullptr);
		}
	}
}
