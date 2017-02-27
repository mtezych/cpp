
#include "VulkanDevice.h"

#include "VulkanSymbols.h"
#include "VulkanInstance.h"

namespace vk
{
	VulkanDevice::VulkanDevice
	(
		const VulkanInstance& instance,
		const VulkanPhysicalDevice& physicalDevice,
		const VkDeviceCreateInfo& deviceCreateInfo
	):
		device { VK_NULL_HANDLE },

		vkCreateDevice
		{
			instance.LoadInstanceProcedure<symbol::vkCreateDevice>()
		},
		vkGetDeviceProcAddr
		{
			instance.LoadInstanceProcedure<symbol::vkGetDeviceProcAddr>()
		},

		vkGetDeviceQueue { nullptr },
		vkDeviceWaitIdle { nullptr },
		vkDestroyDevice  { nullptr }
	{
		const auto result = vkCreateDevice
		(
			physicalDevice.physicalDevice, &deviceCreateInfo, nullptr, &device
		);
		assert(result == VK_SUCCESS);

		vkGetDeviceQueue = LoadDeviceProcedure<symbol::vkGetDeviceQueue>();
		vkDeviceWaitIdle = LoadDeviceProcedure<symbol::vkDeviceWaitIdle>();
		vkDestroyDevice  = LoadDeviceProcedure<symbol::vkDestroyDevice >();
	}

	VulkanDevice::~VulkanDevice()
	{
		const auto result = vkDeviceWaitIdle(device);
		assert(result == VK_SUCCESS);

		vkDestroyDevice(device, nullptr);
	}

	VulkanQueue
	VulkanDevice::Queues(const uint32_t familyIndex, const uint32_t queueIndex) const
	{
		auto queue = VkQueue { VK_NULL_HANDLE };

		vkGetDeviceQueue(device, familyIndex, queueIndex, &queue);

		return VulkanQueue { *this, queue };
	}
}
