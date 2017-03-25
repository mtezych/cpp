
#include "Device.h"

#include "Symbols.h"
#include "Instance.h"

namespace vk
{
	Device::Device
	(
		const Instance&           instance,
		const PhysicalDevice&     physicalDevice,
		const VkDeviceCreateInfo& deviceCreateInfo
	):
		vkDevice { VK_NULL_HANDLE },

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
			physicalDevice.vkPhysicalDevice,
			&deviceCreateInfo,
			nullptr,
			&vkDevice
		);
		assert(result == VK_SUCCESS);

		vkGetDeviceQueue = LoadDeviceProcedure<symbol::vkGetDeviceQueue>();
		vkDeviceWaitIdle = LoadDeviceProcedure<symbol::vkDeviceWaitIdle>();
		vkDestroyDevice  = LoadDeviceProcedure<symbol::vkDestroyDevice >();
	}

	Device::~Device()
	{
		const auto result = vkDeviceWaitIdle(vkDevice);
		assert(result == VK_SUCCESS);

		vkDestroyDevice(vkDevice, nullptr);
	}

	Queue
	Device::Queues(const uint32_t familyIndex, const uint32_t queueIndex) const
	{
		auto queue = VkQueue { VK_NULL_HANDLE };

		vkGetDeviceQueue(vkDevice, familyIndex, queueIndex, &queue);

		return Queue { *this, queue };
	}
}
