
#include <vulkan/DeviceMemory.h>

#include <vulkan/Symbols.h>
#include <vulkan/Device.h>

namespace vk
{
	DeviceMemory::CreateInfo::CreateInfo
	(
		const VkDeviceSize allocationSize,
		const uint32_t     memoryTypeIndex
	):
		createInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			allocationSize,
			memoryTypeIndex,
		}
	{
	}

	DeviceMemory::DeviceMemory (const Device& device, const CreateInfo& createInfo)
	:
		device         { &device        },
		vkDeviceMemory { VK_NULL_HANDLE }
	{
		const auto result = device.vkAllocateMemory
		(
			device.vkDevice,
			&createInfo.createInfo,
			nullptr,
			&vkDeviceMemory
		);
		assert(result == VK_SUCCESS);
	}

	DeviceMemory::~DeviceMemory ()
	{
		if (vkDeviceMemory != VK_NULL_HANDLE)
		{
			device->vkFreeMemory(device->vkDevice, vkDeviceMemory, nullptr);
		}
	}

	DeviceMemory::DeviceMemory (DeviceMemory&& deviceMemory)
	:
		device         { deviceMemory.device         },
		vkDeviceMemory { deviceMemory.vkDeviceMemory }
	{
		deviceMemory.device         = nullptr;
		deviceMemory.vkDeviceMemory = VK_NULL_HANDLE;
	}

	DeviceMemory& DeviceMemory::operator = (DeviceMemory&& deviceMemory)
	{
		if (vkDeviceMemory != VK_NULL_HANDLE)
		{
			device->vkFreeMemory(device->vkDevice, vkDeviceMemory, nullptr);
		}

		device         = deviceMemory.device;
		vkDeviceMemory = deviceMemory.vkDeviceMemory;

		deviceMemory.device         = nullptr;
		deviceMemory.vkDeviceMemory = VK_NULL_HANDLE;

		return *this;
	}
}
