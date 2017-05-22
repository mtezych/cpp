
#include <vulkan/CommandPool.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	CommandPool::CommandPool
	(
		const Device& device, const uint32_t queueFamilyIndex
	):
		device        { &device },
		vkCommandPool { VK_NULL_HANDLE }
	{
		const auto createInfo = VkCommandPoolCreateInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			0,
			queueFamilyIndex
		};
		const auto result = device.vkCreateCommandPool
		(
			device.vkDevice, &createInfo, nullptr, &vkCommandPool
		);
		assert(result == VK_SUCCESS);
	}

	CommandPool::~CommandPool ()
	{
		if (vkCommandPool != VK_NULL_HANDLE)
		{
			device->vkDestroyCommandPool(device->vkDevice, vkCommandPool, nullptr);
		}
	}

	CommandPool::CommandPool (CommandPool&& commandPool)
	:
		device        { commandPool.device        },
		vkCommandPool { commandPool.vkCommandPool }
	{
		commandPool.device        = nullptr;
		commandPool.vkCommandPool = VK_NULL_HANDLE;
	}

	CommandPool& CommandPool::operator = (CommandPool&& commandPool)
	{
		if (vkCommandPool != VK_NULL_HANDLE)
		{
			device->vkDestroyCommandPool(device->vkDevice, vkCommandPool, nullptr);
		}

		device        = commandPool.device;
		vkCommandPool = commandPool.vkCommandPool;

		commandPool.device        = nullptr;
		commandPool.vkCommandPool = VK_NULL_HANDLE;

		return *this;
	}
}
